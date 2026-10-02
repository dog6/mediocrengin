#include "AVGNG/Core/physics/PhysicsBody.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Debug.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include <imgui/imgui.h>

#include <algorithm>
#include <cfloat>
#include <functional>

#include "AVGNG/Assets/Math.hpp"

namespace {
    // A collision with an approach speed below this value gives no bounce.
    // Make it more than 2 * gravity * dt (about 0.33 at 60 FPS and 9.81 gravity).
    constexpr float RESTITUTION_THRESHOLD = 0.5f;

    // A body is "slow" when its speeds are below these values.
    constexpr float SLEEP_LINEAR_SPEED  = 0.05f;
    constexpr float SLEEP_ANGULAR_SPEED = 0.05f;

    // The body must stay slow and in contact for this time (seconds) before it sleeps.
    constexpr float SLEEP_TIME = 0.5f;

    // A move larger than this value (world units) wakes a sleeping body.
    constexpr float WAKE_MOVE_DISTANCE = 0.0005f;

    // The contact normal points from this body to the other collider.
    // A floor below the body gives a normal.y close to -1.
    // A contact with normal.y below this value counts as ground.
    constexpr float GROUND_NORMAL_Y = -0.7f;
}

using namespace ng::Core;

PhysicsBody::PhysicsBody() {
    // "owner" is not set yet in the constructor. Update() caches "tf" later.
    this->linearVelocity = glm::vec3(0, 0, 0);
}

PhysicsBody::~PhysicsBody() {}

void PhysicsBody::OnInspectorGUI() {

    // Give this component its own ID scope. This stops label conflicts.
    ImGui::PushID(this);

    ImGui::Text("PhysicsBody Component [%p]", (void*)this);

    // Edit a copy of the vector. Then call the setter, so that any setter logic runs.
    auto DragVec3 = [](const char* label, const glm::vec3& value, float speed, auto setter) {
        glm::vec3 v = value;
        if (ImGui::DragFloat3(label, glm::value_ptr(v), speed, 0.0f, 0.0f, "%.3f")) {
            setter(v);
        }
    };

    if (ImGui::Checkbox("Is Kinematic", &isKinematic)) Wake();

    // Mass, restitution, and friction
    ImGui::Separator();
    ImGui::TextDisabled("Material");

    if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, FLT_MAX, "%.3f")) Wake();
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("A mass of 0 or less gives a body that does not move in a collision.");
    }

    ImGui::SliderFloat("Restitution", &restitution, 0.0f, 1.0f, "%.2f");
    ImGui::DragFloat("Friction", &friction, 0.01f, 0.0f, FLT_MAX, "%.3f");

    // Linear motion
    ImGui::Separator();
    ImGui::TextDisabled("Linear");

    DragVec3("Velocity", linearVelocity, 0.1f,
             [this](const glm::vec3& v) { SetLinearVelocity(v); });

    // Update() sets the linear acceleration from gravity each frame.
    // A change here has no effect, so the control is read-only.
    ImGui::BeginDisabled();
    DragVec3("Acceleration", linearAcceleration, 0.1f,
             [this](const glm::vec3& v) { SetLinearAcceleration(v); });
    ImGui::EndDisabled();

    DragVec3("Gravity", gravityForce, 0.1f,
             [this](const glm::vec3& v) { SetGravity(v); });

    if (ImGui::Button("Earth Gravity")) {
        SetGravity(glm::vec3(0.0f, -9.81f, 0.0f));
    }

    // Angular motion
    ImGui::Separator();
    ImGui::TextDisabled("Angular");

    DragVec3("Angular Velocity", angularVelocity, 0.01f,
             [this](const glm::vec3& v) { SetAngularVelocity(v); });

    DragVec3("Angular Acceleration", angularAcceleration, 0.01f,
             [this](const glm::vec3& v) { SetAngularAcceleration(v); });

    // Tools for testing
    ImGui::Separator();

    if (ImGui::Button("Stop")) {
        SetLinearVelocity(glm::vec3(0.0f));
        SetAngularVelocity(glm::vec3(0.0f));
    }
    ImGui::SameLine();
    ImGui::Text("Speed: %.3f", glm::length(linearVelocity));

    ImGui::Text("Sleeping: %s", sleeping ? "yes" : "no");
    ImGui::SameLine();
    if (ImGui::Button("Wake")) Wake();

    ImGui::PopID();
}

void PhysicsBody::ApplyPositionCorrection(const glm::vec3& delta)
{
    if (!tf) return;

    // Change this line if your Transform uses different function names.
    tf->SetPosition(tf->GetPosition() + delta);
}

void PhysicsBody::HandleTwoPhysicsBodyCollision(PhysicsBody* otherPB,
                                                const glm::vec3& normal,
                                                float penetration)
{
    float invM1 = this->GetInverseMass();
    float invM2 = otherPB->GetInverseMass();

    glm::vec3 v1 = linearVelocity;
    glm::vec3 v2 = otherPB->linearVelocity;

    // A positive value means the bodies move toward each other.
    float approachSpeed = glm::dot(v1 - v2, normal);

    if (otherPB->sleeping) {
        if (approachSpeed > RESTITUTION_THRESHOLD) {
            // A hard hit. Wake the other body.
            otherPB->Wake();
        }
        else {
            // A soft contact. Treat the sleeping body as a fixed object.
            // With invM2 = 0, the position correction does not move it.
            invM2 = 0.0f;
        }
    }

    float invSum = invM1 + invM2;
    if (invSum <= 0.0f) return;

    // Push the bodies apart first. This way, bodies that already
    // separate can still leave the overlap.
    glm::vec3 correction = normal * (penetration / invSum);
    this->ApplyPositionCorrection(-correction * invM1);
    otherPB->ApplyPositionCorrection(correction * invM2);

    if (approachSpeed <= 0.0f) return;

    // A slow impact gives no bounce.
    float e = (approachSpeed < RESTITUTION_THRESHOLD)
                  ? 0.0f
                  : std::min(this->GetRestitution(), otherPB->GetRestitution());

    // Impulse size along the normal
    float j = (1.0f + e) * approachSpeed / invSum;

    // Write the members directly. The public setters call Wake().
    linearVelocity = v1 - j * invM1 * normal;
    if (invM2 > 0.0f) {
        otherPB->linearVelocity = v2 + j * invM2 * normal;
    }
}

void PhysicsBody::HandleOnePhysicsBodyCollision(const glm::vec3& normal,
                                                float penetration)
{
    // A kinematic body does not react to collisions.
    if (this->GetInverseMass() <= 0.0f) return;

    // Push this body out of the static collider.
    this->ApplyPositionCorrection(-normal * penetration);

    // A positive value means this body moves into the other collider.
    float approachSpeed = glm::dot(linearVelocity, normal);
    if (approachSpeed <= 0.0f) return;

    // A slow impact gives no bounce.
    float e = (approachSpeed < RESTITUTION_THRESHOLD) ? 0.0f : this->GetRestitution();

    // Change only the velocity part that points into the surface.
    linearVelocity -= (1.0f + e) * approachSpeed * normal;
}

void PhysicsBody::HandlePhysics() {
    Collider* ownedCol = this->owner->GetComponent<Collider>();
    if (!ownedCol || !ownedCol->IsOverlapping()) return;

    Collider* other = ownedCol->GetOtherCollider();
    if (!other) return;

    touchedThisFrame = true;

    glm::vec3 normal = ownedCol->GetContactNormal();
    float penetration = ownedCol->GetPenetration();

    PhysicsBody* otherPB = other->owner->GetComponent<PhysicsBody>();
    if (otherPB) {
        // A sleeping body does not run HandlePhysics, so the awake body
        // must resolve the pair. If both bodies are awake, only the body
        // with the lower address resolves the pair.
        bool resolve = otherPB->sleeping || std::less<PhysicsBody*>{}(this, otherPB);
        if (resolve) {
            HandleTwoPhysicsBodyCollision(otherPB, normal, penetration);
        }
    } else {
        HandleOnePhysicsBodyCollision(normal, penetration);
    }
}

void PhysicsBody::UpdateSleepState(float dt) {

    // Only a body in contact can go to sleep. This stops a body from
    // sleeping in the air at the top of a bounce.
    if (!touchedThisFrame) {
        sleepTimer = 0.0f;
        return;
    }

    bool slow = glm::length(linearVelocity)  < SLEEP_LINEAR_SPEED &&
                glm::length(angularVelocity) < SLEEP_ANGULAR_SPEED;

    if (!slow) {
        sleepTimer = 0.0f;
        return;
    }

    sleepTimer += dt;
    if (sleepTimer >= SLEEP_TIME) {
        sleeping = true;
        linearVelocity = glm::vec3(0.0f);
        angularVelocity = glm::vec3(0.0f);

        // Record what the body rests on. ShouldWake() uses this data.
        RecordSleepState();
    }
}

// Call this when the body falls asleep.
// It records the position of the body and the support below it.
void PhysicsBody::RecordSleepState()
{
    sleepPosition    = tf->GetWorldPosition();
    supportCollider  = nullptr;
    supportTransform = nullptr;
    supportPosition  = glm::vec3(0.0f);

    Collider* col   = owner->GetComponent<Collider>();
    Collider* other = col ? col->GetOtherCollider() : nullptr;
    if (!other) return;

    supportCollider  = other;
    supportTransform = other->owner->GetComponent<Transform>();
    if (supportTransform) {
        supportPosition = supportTransform->GetWorldPosition();
    }
}

// Call this each frame while the body sleeps.
// It uses stored data and the collision result. It does not run a new collision test.
// It returns true when the body must wake.
bool PhysicsBody::ShouldWake()
{
    // 1. Something moved this body (script, inspector, or parent).
    if (glm::distance(tf->GetWorldPosition(), sleepPosition) > WAKE_MOVE_DISTANCE) {
        return true;
    }

    // 2. The body has no contact now. The support is gone.
    Collider* col = owner->GetComponent<Collider>();
    if (!col || !col->IsOverlapping()) {
        return true;
    }

    // 3. The body touches a different collider than before.
    if (col->GetOtherCollider() != supportCollider) {
        return true;
    }

    // 4. The support moved.
    if (supportTransform &&
        glm::distance(supportTransform->GetWorldPosition(), supportPosition) > WAKE_MOVE_DISTANCE) {
        return true;
    }

    return false;
}

// Update physics body
void PhysicsBody::Update(float dt) {

    // Cache the transform if the pointer is null
    if (!this->tf) {
        if (this->owner != nullptr) {
            this->tf = this->owner->GetComponent<Transform>();
        }
        if (!this->tf) return;
    }

    // A sleeping body does a cheap wake test. It stops here if it stays asleep.
    if (sleeping) {
        if (!ShouldWake()) return;
        Wake();
    }

    touchedThisFrame = false;

    // Write the members directly. The public setters call Wake().
    if (!isKinematic) {
        // Update linearAcceleration using gravity force
        linearAcceleration = gravityForce;

        // Update velocity using acceleration: v = v0 + a * dt
        linearVelocity += linearAcceleration * dt;
    }

    // This function sets touchedThisFrame if a contact exists.
    HandlePhysics();

    UpdateSleepState(dt);
    if (sleeping) return;

    // Update position using velocity: pos = pos0 + v * dt
    this->tf->SetPosition(this->tf->GetPosition() + linearVelocity * dt);

    // Update angular velocity using angular acceleration: w = w0 + alpha * dt
    angularVelocity += angularAcceleration * dt;

    // Update rotation using angular velocity: rot = rot0 + w * dt
    this->tf->SetRotation(this->tf->GetRotation() + angularVelocity * dt);
}

// Setters
// A change from outside wakes the body.

void PhysicsBody::SetLinearVelocity(glm::vec3 velocity)
{
    this->linearVelocity = velocity;
    Wake();
}

void PhysicsBody::SetLinearAcceleration(glm::vec3 accel)
{
    this->linearAcceleration = accel;
    Wake();
}

void PhysicsBody::SetAngularVelocity(glm::vec3 velocity)
{
    this->angularVelocity = velocity;
    Wake();
}

void PhysicsBody::SetAngularAcceleration(glm::vec3 accel)
{
    this->angularAcceleration = accel;
    Wake();
}

// Player helpers

// Sets the X and Z velocity. The Y velocity stays under the control of gravity.
// The body wakes only when the value changes. A player that stands still can sleep.
void PhysicsBody::SetHorizontalVelocity(float x, float z)
{
    if (linearVelocity.x == x && linearVelocity.z == z) return;

    linearVelocity.x = x;
    linearVelocity.z = z;
    Wake();
}

// The contact normal points from this body to the other collider.
// A floor below the body gives a normal with a large negative Y value.
// The collider stores one contact only. A wall contact can hide a floor contact.
bool PhysicsBody::IsGrounded()
{
    if (!owner) return false;

    Collider* col = owner->GetComponent<Collider>();
    if (!col || !col->IsOverlapping()) return false;

    return col->GetContactNormal().y < GROUND_NORMAL_Y;
}

bool PhysicsBody::Jump(float speed)
{
    if (!IsGrounded()) return false;

    linearVelocity.y = speed;
    Wake();
    return true;
}

// Saving & Loading

void PhysicsBody::Save(nlohmann::json &j)
{
    Debug::Log(DEBUG, "Saving PhysicsBody %p...", (void*)this);

    // Save physics body data
    j["physicsBody"]["linearVelocity"] = ng::Assets::Vec3ToJson(linearVelocity);
    j["physicsBody"]["linearAcceleration"] = ng::Assets::Vec3ToJson(linearAcceleration);
    j["physicsBody"]["angularVelocity"] = ng::Assets::Vec3ToJson(angularVelocity);
    j["physicsBody"]["angularAcceleration"] = ng::Assets::Vec3ToJson(angularAcceleration);
    j["physicsBody"]["gravity"] = ng::Assets::Vec3ToJson(gravityForce);

    j["physicsBody"]["restitution"] = restitution;
    j["physicsBody"]["mass"] = mass;
    j["physicsBody"]["friction"] = friction;
    j["physicsBody"]["isKinematic"] = isKinematic;

    Debug::Log(DEBUG, "Finished saving PhysicsBody %p", (void*)this);
}

void PhysicsBody::Load(const nlohmann::json &j)
{
    if (!j.contains("physicsBody")) return;
    const nlohmann::json& t = j.at("physicsBody");

    linearVelocity = ng::Assets::ReadVec3(t, "linearVelocity", linearVelocity);
    linearAcceleration = ng::Assets::ReadVec3(t, "linearAcceleration", linearAcceleration);
    angularVelocity = ng::Assets::ReadVec3(t, "angularVelocity", angularVelocity);
    angularAcceleration = ng::Assets::ReadVec3(t, "angularAcceleration", angularAcceleration);
    gravityForce = ng::Assets::ReadVec3(t, "gravity", gravityForce);

    restitution = ng::Assets::ReadFloat(t, "restitution", restitution);
    mass = ng::Assets::ReadFloat(t, "mass", mass);
    friction = ng::Assets::ReadFloat(t, "friction", friction);
    isKinematic = t.value("isKinematic", isKinematic);

    // A loaded body starts awake.
    Wake();
}