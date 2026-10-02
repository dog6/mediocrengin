#include "AVGNG/Assets/SceneJsonSerializer.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include "AVGNG/Core/Debug.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>

using json = nlohmann::json;
using namespace ng::Core;

namespace ng::Assets {

    static IComponent* GetOrAddComponentForKey(GameObject* go, const std::string& key)
    {
        if (key == "transform") return go->GetComponent<Transform>(); 
        if (key == "meshRenderer") return go->AddComponent<ng::Graphics::MeshRenderer>();
        if (key == "physicsBody") return go->AddComponent<PhysicsBody>();
        if (key == "collider") return go->AddComponent<Collider>();
        // Add additional engine components here
        return nullptr;
    }

    bool SceneJsonSerializer::SerializeSceneToJson(Scene& scene, std::string_view filepath)
    {

        // get active scene name
        std::string sceneName = scene.GetName();

        // create json object to store data
        json jsonData;
        json& sceneJSON = jsonData["scene"];
        sceneJSON["sceneName"] = sceneName;

        // serialize active scene camera data
        ng::Graphics::Camera* cam = scene.GetActiveCamera();
        if (cam != nullptr) {
            sceneJSON["sceneCamera"]["position"] = Vec3ToJson(cam->GetPosition());
            sceneJSON["sceneCamera"]["target"]   = Vec3ToJson(cam->GetTarget());
        } else {
            Debug::Log(WARN, "Scene '%s' has no active camera during export", sceneName.c_str());
        }

        // serialize gameObjects in scene
        json gameObjectsJSON = json::array();
        for (auto* go : scene.GetGameObjects()) {
            if (!go) continue;

            json goJSON = json::object();
            goJSON["name"]   = go->name;
            goJSON["active"] = go->isActive;

            json componentsJSON = json::array();
            for (auto* comp : go->GetAttachedComponents()) {
                if (!comp) continue;

                json compJSON = json::object();
                comp->Save(compJSON);
                componentsJSON.push_back(std::move(compJSON));
            }

            goJSON["components"] = std::move(componentsJSON);
            gameObjectsJSON.push_back(std::move(goJSON));
        }

        // add gameObjectsJSON object to sceneJSON object
        sceneJSON["gameObjects"] = std::move(gameObjectsJSON);

        // resolve path
        std::filesystem::path scenePath(filepath);
        if (scenePath.extension() != ".json") {
            scenePath += ".json";
        }

        // write scene file
        std::string jsonContent = jsonData.dump(4, ' ', false, json::error_handler_t::ignore);
        if (!FileReader::WriteFile(scenePath.string(), jsonContent)) {
            Debug::Log(ERROR, "Failed to write scene file: %s", scenePath.string().c_str());
            return false;
        }

        // log about it
        Debug::Log(LOG, "Successfully saved scene '%s' to '%s'", sceneName.c_str(), scenePath.string().c_str());
        return true;
    }

    ng::Core::Scene* SceneJsonSerializer::DeserializeSceneFromJson(std::string_view filePath, ng::Graphics::Camera* camera, glm::uvec2& viewportSize)
    {

        // read file content into ifs
        std::ifstream ifs(filePath.data());
        if (!ifs.is_open()) {
            Debug::Log(ERROR, "Failed to open scene file: %s", filePath.data());
            return nullptr;
        }

        // parse file content into json object
        json jf = json::parse(ifs, nullptr, false);
        if (jf.is_discarded() || !jf.contains("scene")) {
            Debug::Log(ERROR, "Invalid or corrupted scene file format: %s", filePath.data());
            return nullptr;
        }

        // deserialize json object into new scene object
        const json& sceneJSON = jf.at("scene");
        std::string sceneName = sceneJSON.value("sceneName", "Untitled Scene");

        // Restore Scene Camera state safely
        if (camera != nullptr && sceneJSON.contains("sceneCamera")) {
            const json& camJSON = sceneJSON.at("sceneCamera");
            camera->SetPosition(ReadVec3(camJSON, "position", camera->GetPosition()));
            camera->SetTarget(ReadVec3(camJSON, "target", camera->GetTarget()));
        }

        // Allocate new target scene
        Scene* scene = new Scene(camera, viewportSize, sceneName.c_str());

        if (!sceneJSON.contains("gameObjects") || !sceneJSON.at("gameObjects").is_array()) {
            return scene;
        }

        // Deserialize GameObjects
        for (const auto& goJSON : sceneJSON.at("gameObjects")) {
            if (!goJSON.is_object()) continue;

            GameObject* go = scene->CreateGameObject(goJSON.value("name", "GameObject"));
            go->isActive = goJSON.value("active", true);

            if (!goJSON.contains("components") || !goJSON.at("components").is_array()) {
                continue;
            }

            for (const auto& compWrapperJSON : goJSON.at("components")) {
                if (!compWrapperJSON.is_object()) continue;

                for (auto it = compWrapperJSON.begin(); it != compWrapperJSON.end(); ++it) {
                    const std::string& compKey = it.key();

                    IComponent* comp = GetOrAddComponentForKey(go, compKey);
                    if (comp == nullptr) {
                        Debug::Log(WARN, "Unrecognized component '%s' on GameObject '%s'", compKey.c_str(), go->name.c_str());
                        continue;
                    }

                    // Pass the full wrapper JSON object to the component deserializer.
                    // Each Load() method checks for its own key inside this object.
                    comp->Load(compWrapperJSON);
                }
            }
        }

        Debug::Log(LOG, "Successfully loaded scene '%s' from '%s'", sceneName.c_str(), filePath.data());
        return scene;
    }

}