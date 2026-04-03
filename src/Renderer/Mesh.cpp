
#include "AVGNG/Renderer/Mesh.hpp"
#include "AVGNG/Renderer/ShaderLoader.hpp"

using namespace ng::Core;

namespace ng::Graphics {

    static void CheckGLError(const char* location) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            Debug::Log(LogLevel::ERROR, "OpenGL error %d at %s", err, location);
        }
    }

    Mesh::Mesh() {
        this->positions = std::vector<glm::vec3>();
        this->normals = std::vector<glm::vec3>();
        this->texCoords = std::vector<glm::vec2>();
        this->indices = std::vector<unsigned int>();
        this->EBO = 0;
        this->VBO = 0;
        this->VAO = 0;
        this->material = nullptr;
        this->filepath = "";
    }

    Mesh::Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords, std::vector<unsigned int> inds)
    {
        for (size_t i = 0; i < positions.size(); i++) {
            Vertex v = Vertex();
            v.position = positions[i];
            v.normal = (i < normals.size()) ? normals[i] : glm::vec3(0.0f, 0.0f, 1.0f);
            v.texCoord = (i < texCoords.size()) ? texCoords[i] : glm::vec2(0.0f, 0.0f);
            vertices.push_back(v);
        }

        this->positions = positions;
        this->normals = normals;
        this->texCoords = texCoords;
        this->indices = inds;
        this->material = nullptr;

        indices = inds;
        SetupMesh();
    }

    Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds)
    {
        this->vertices = verts;
        this->indices = inds;

        // Extract positions, normals, texCoords for backward compatibility if needed
        for (const auto& v : verts) {
            this->positions.push_back(v.position);
            this->normals.push_back(v.normal);
            this->texCoords.push_back(v.texCoord);
        }

        this->EBO = 0;
        this->VBO = 0;
        this->VAO = 0;

        SetupMesh();
    }

    void Mesh::SetupMesh()
    {

        Debug::Log(LogLevel::DEBUG, "SetupMesh called with %d vertices, %d indices",
            (int)vertices.size(), (int)indices.size());

        if (vertices.empty() || indices.empty()) {
            Debug::Log(LogLevel::ERROR, "SetupMesh: Empty vertices or indices!");
            return;

        }

        Shader shader = ng::Assets::ShaderLoader::LoadDefaultShader();
        this->material->SetShader(shader);
        Debug::Log(DEV, "Vertex[0] texCoord: (%f, %f)", vertices[0].texCoord.x, vertices[0].texCoord.y);

        glGenVertexArrays(1, &VAO);
        CheckGLError("glGenVertexArrays");

        glGenBuffers(1, &VBO);
        CheckGLError("glGenBuffers VBO");

        glGenBuffers(1, &EBO);
        CheckGLError("glGenBuffers EBO");

        Debug::Log(LogLevel::DEBUG, "Generated VAO: %d, VBO: %d, EBO: %d", VAO, VBO, EBO);

        glBindVertexArray(VAO);
        CheckGLError("glBindVertexArray");

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        CheckGLError("glBindBuffer VBO");

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
            &vertices[0], GL_STATIC_DRAW);
        CheckGLError("glBufferData VBO");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        CheckGLError("glBindBuffer EBO");

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
            &indices[0], GL_STATIC_DRAW);
        CheckGLError("glBufferData EBO");

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void*)offsetof(Vertex, position));
        CheckGLError("glVertexAttribPointer 0");
        glEnableVertexAttribArray(0);
        CheckGLError("glEnableVertexAttribArray 0");

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void*)offsetof(Vertex, normal));
        CheckGLError("glVertexAttribPointer 1");
        glEnableVertexAttribArray(1);
        CheckGLError("glEnableVertexAttribArray 1");

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void*)offsetof(Vertex, texCoord));
        CheckGLError("glVertexAttribPointer 2");
        glEnableVertexAttribArray(2);
        CheckGLError("glEnableVertexAttribArray 2");

        glBindVertexArray(0);
        CheckGLError("glBindVertexArray 0");

        Debug::Log(LogLevel::DEBUG, "SetupMesh completed");


    }

    void Mesh::UseShader(ng::Graphics::Camera& camera, ng::Core::Transform& transform)
    {

        Shader* shader = material->GetShader();
        if (shader == nullptr) {
            Debug::Log(ERROR, "No shader assigned to material!");
            return;
        }

        shader->Use();

        // Get matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(1280, 720);
        glm::mat4 model = transform.GetModelMatrix();

        // Set matrices
        shader->SetMat4("view", view);
        shader->SetMat4("projection", projection);
        shader->SetMat4("model", model);

        // Set material properties
        // Even better: handle multiple texture types
        MaterialData* m = material->GetMaterialData();

        // Populate material data
        if (m == nullptr) {
            Debug::Log(ERROR, "MaterialData is null in MeshRenderer::Draw");
            return;
        }

        // Find textures
        Texture* diffuseTexture = m->FindTexture(TextureType::DIFFUSE);
        Texture* specularTexture = m->FindTexture(TextureType::SPECULAR);
        Texture* normalTexture = m->FindTexture(TextureType::NORMAL);
        Texture* emissiveTexture = m->FindTexture(TextureType::EMISSIVE);
        Texture* alphaTexture = m->FindTexture(TextureType::ALPHA);
        Texture* metallicTexture = m->FindTexture(TextureType::METALLIC);

        // Material properties
        shader->SetVec3("Albedo", m->Albedo);
        shader->SetVec3("AmbientColor", m->Ambient);
        shader->SetVec3("DiffuseColor", m->Diffuse);
        shader->SetVec3("SpecularColor", m->Specular);
        shader->SetVec3("EmissiveColor", m->Emissive);
        shader->SetFloat("Shininess", m->Shininess);
        shader->SetFloat("IOR", m->IOR);
        shader->SetFloat("Opacity", m->Opacity);
        shader->SetFloat("Metallic", m->Metallicness);
        // Lighting 
        shader->SetVec3("sunDirection", glm::normalize(glm::vec3(-0.3f, -1.0f, -0.5f)));
        shader->SetVec3("sunColor", glm::vec3(1.0f, 0.95f, 0.8f));
        shader->SetVec3("viewPos", camera.GetPosition());

        // Diffuse texture (texture unit 0)
        if (diffuseTexture != nullptr && diffuseTexture->id > 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseTexture->id);
            shader->SetBool("hasDiffuseMap", true);
        }
        else {
            shader->SetBool("hasDiffuseMap", false);
        }
        shader->SetInt("diffuseMap", 0);


        // Specular texture (texture unit 1) 
        if (specularTexture != nullptr && specularTexture->id > 0) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularTexture->id);
            shader->SetBool("hasSpecularMap", true);
        }
        else {
            shader->SetBool("hasSpecularMap", false);
        }
        shader->SetInt("specularMap", 1);
       // Emissive map (texture unit 3)
        if (emissiveTexture != nullptr && emissiveTexture->id > 0) {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, emissiveTexture->id);
            shader->SetBool("hasEmissiveMap", true);
        }
        else shader->SetBool("hasEmissiveMap", false);
        shader->SetInt("emissiveMap", 3);
       // Normal map (texture unit 3)
        if (normalTexture != nullptr && normalTexture->id > 0) {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, normalTexture->id);
            shader->SetBool("hasNormalMap", true);
        }
        else shader->SetBool("hasNormalMap", false);
        shader->SetInt("normalMap", 2);
       
        // Alpha map (texture unit 4)
        if (alphaTexture != nullptr && alphaTexture->id > 0) {
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, alphaTexture->id);
            shader->SetBool("hasAlphaMap", true);
        }
        else {
            shader->SetBool("hasAlphaMap", false);
        }
        shader->SetInt("alphaMap", 4);

        if (metallicTexture != nullptr && metallicTexture->id > 0) {
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, metallicTexture->id);
            shader->SetBool("hasMetallicMap", true);
        }
        else {
            shader->SetBool("hasMetallicMap", false);
        }
        shader->SetInt("metallicMap", 4);


    }

   

}