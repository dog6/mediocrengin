#pragma once

#include "AVGNG/Graphics/MaterialData.hpp"
#include "AVGNG/Graphics/Shader.hpp"


namespace ng::Graphics {

        class Shader;

        class Material
        {
            private:

                ng::Graphics::MaterialData data;
                ng::Graphics::Shader shader;
                static ng::Graphics::Shader s_defaultShader;

            public:

                Material();
                ~Material();

                // Getters & Setters
                void SetMaterialData(MaterialData materialData);
                ng::Graphics::MaterialData* GetMaterialData() { return &data; }

                void SetShader(Shader& shader) { this->shader = shader; }
                ng::Graphics::Shader* GetShader();

                void SetAlbedoColor(glm::vec3 col);
                void SetAmbientColor(glm::vec3 col);
                void SetDiffuseColor(glm::vec3 col);
                void SetSpecularColor(glm::vec3 col);
                void SetEmissiveColor(glm::vec3 col);
        };
    
}