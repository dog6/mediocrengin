#include "AVGNG/Scripting/GraphicsRegistrar.hpp"

#include "AVGNG/Graphics/Mesh.hpp"
#include "AVGNG/Graphics/Material.hpp"
#include "AVGNG/Graphics/MeshRenderer.hpp"
#include "AVGNG/Graphics/Camera.hpp"
#include "AVGNG/Graphics/ShaderLoader.hpp"
#include "AVGNG/Graphics/Shader.hpp"

#include <glm/glm.hpp>

using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Scripting {

    static void RegisterMesh(sol::state& lua) {
        lua.new_usertype<Mesh>("Mesh",
            "filepath", &Mesh::filepath,
            "material", &Mesh::GetMaterial
        );
    }

    static void RegisterMaterial(sol::state& lua) {
        lua.new_usertype<Material>("Material",
            "SetShader", &Material::SetShader,
            "GetShader", &Material::GetShader,
            "SetMaterialData", [](Material& material, const MaterialData& src) {
                MaterialData* dst = material.GetMaterialData();
                if (dst == nullptr) return;

                dst->name         = src.name;
                dst->Albedo       = src.Albedo;
                dst->Ambient      = src.Ambient;
                dst->Diffuse      = src.Diffuse;
                dst->Specular     = src.Specular;
                dst->Emissive     = src.Emissive;
                dst->IOR          = src.IOR;
                dst->Shininess    = src.Shininess;
                dst->Opacity      = src.Opacity;
                dst->Metallicness = src.Metallicness;
            },
            "GetMaterialData", &Material::GetMaterialData
        );
    }

    static void RegisterMeshRenderer(sol::state& lua) {
        lua.new_usertype<MeshRenderer>("MeshRenderer",
            sol::base_classes, sol::bases<IComponent>(),
            "LoadMesh", &MeshRenderer::LoadMesh,
            "SetTexture", &MeshRenderer::SetTexture,
            "SetTextureTiling", &MeshRenderer::SetTextureTiling,
            "GetMesh", &MeshRenderer::GetMesh
        );
    }
    
    static void RegisterCamera(sol::state& lua) {
        lua.new_usertype<Camera>("Camera",
            // SetPosition supports both (x, y, z) and glm::vec3
            "SetPosition", sol::overload(
                [](Camera& cam, float x, float y, float z) { cam.SetPosition(glm::vec3(x, y, z)); },
                [](Camera& cam, const glm::vec3& pos) { cam.SetPosition(pos); }
            ),
            "GetPosition", [](Camera& cam) {
                glm::vec3 camPos = cam.GetPosition();
                return std::make_tuple(camPos.x, camPos.y, camPos.z);
            },

            // SetTarget supports both (x, y, z) and glm::vec3
            "SetTarget", sol::overload(
                [](Camera& cam, float x, float y, float z) { cam.SetTarget(glm::vec3(x, y, z)); },
                [](Camera& cam, const glm::vec3& target) { cam.SetTarget(target); }
            ),
            "GetTarget", [](Camera& cam) {
                glm::vec3 camTarget = cam.GetTarget();
                return std::make_tuple(camTarget.x, camTarget.y, camTarget.z);
            }, 

            // SetUpwardDirection supports both (x, y, z) and glm::vec3
            "SetUpwardDirection", sol::overload(
                [](Camera& cam, float x, float y, float z) { cam.SetUp(glm::vec3(x, y, z)); },
                [](Camera& cam, const glm::vec3& up) { cam.SetUp(up); }
            ),
            "GetUpwardDirection", [](Camera& cam) {
                glm::vec3 camUp = cam.GetUp();
                return std::make_tuple(camUp.x, camUp.y, camUp.z);
            }
        );
    }

    static void RegisterShaderLoader(sol::state& lua) {
        lua.new_usertype<ng::Assets::ShaderLoader>("ShaderLoader",
            "LoadShaderFromFile", &ng::Assets::ShaderLoader::LoadShader,
            "LoadDefaultShader", &ng::Assets::ShaderLoader::LoadDefaultShader
        );
    }

    void GraphicsRegistrar::Register(sol::state& lua) {
        Debug::Log(LOG, "Registering Mesh bindings with lua..");
        RegisterMesh(lua);
        Debug::Log(LOG, "Registering Mesh bindings with lua..");
        RegisterMaterial(lua);
        Debug::Log(LOG, "Registering MeshRenderer bindings with lua..");
        RegisterMeshRenderer(lua);
        Debug::Log(LOG, "Registering Camera bindings with lua..");
        RegisterCamera(lua);
        Debug::Log(LOG, "Registering ShaderLoader bindings with lua..");
        RegisterShaderLoader(lua);
    }

}