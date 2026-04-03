#pragma once

#include "AVGNG/Renderer/MaterialData.hpp"
#include "AVGNG/Renderer/Shader.hpp"


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

        };
    
}