#include "AVGNG/Graphics/Mesh.hpp"
#include "AVGNG/Graphics/ShaderLoader.hpp"

using namespace ng::Core;

namespace ng::Graphics {

    static void CheckGLError(const char* location) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            Debug::Log(LogLevel::ERROR, "OpenGL error %d at %s", err, location);
        }
    }

    bool Mesh::ValidateMeshData()
    {
        Debug::Log(LogLevel::DEBUG, "Mesh::SetupMesh called with %d vertices, %d indices",
            (int)vertices.size(), (int)indices.size());

        if (vertices.empty() || indices.empty()) {
            Debug::Log(LogLevel::ERROR, "SetupMesh: Empty vertices or indices!");
            return false;
        }

        return true;
    }

    void Mesh::EnsureMaterialExists()
    {
        // Ensure material exists
        if (this->material == nullptr) {
            this->material = new Material();
        }

        // Load default shader if material doesn't already have one
        if (this->material->GetShader() == nullptr) {
            Shader shader = ng::Assets::ShaderLoader::LoadDefaultShader();
            this->material->SetShader(shader);
        }

        // Set default material colors to White so multiplying by textures doesn't render Black
        MaterialData* m = this->material->GetMaterialData();
        if (m != nullptr) {
            if (m->Albedo == glm::vec3(0.0f))  m->Albedo  = glm::vec3(1.0f);
            if (m->Diffuse == glm::vec3(0.0f)) m->Diffuse = glm::vec3(1.0f);
            if (m->Ambient == glm::vec3(0.0f)) m->Ambient = glm::vec3(1.0f);
        }
    }

    void Mesh::CreateGPUBuffers()
    {
        // Create the objects OpenGL needs to store this mesh on the GPU.
        // A VAO stores the format of the vertex data.
        // A VBO stores the vertex data itself (positions, normals, UVs).
        // An EBO stores the index data. This tells OpenGL the order to draw vertices in.
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the VAO. All following vertex setup calls will apply to this VAO.
        glBindVertexArray(VAO);

        // Bind the VBO, then copy the vertex data into it.
        // GL_STATIC_DRAW tells OpenGL this data will not change often.
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
            &vertices[0], GL_STATIC_DRAW);

        // Bind the EBO, then copy the index data into it.
        // This data tells OpenGL which vertices to connect into triangles, and in what order
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
            &indices[0], GL_STATIC_DRAW);
    }

    void Mesh::SetupVertexAttributes()
    {
        // Position Attribute
        // Tell OpenGL how to read the position data from the Vertex struct.
        // Attribute 0 holds 3 float values, starting at the 'position' field of each Vertex.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Normal Attribute
        // Tell OpenGL how to read the normal data from the Vertex struct.
        // Attribute 1 holds 3 float values, starting at the 'normal' field of each Vertex.
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // UV / TexCoord Attribute
        // Tell OpenGL how to read the texture coordinate data from the Vertex struct.
        // Attribute 2 holds 2 float values, starting at the 'texCoord' field of each Vertex.
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);
    }

    void Mesh::SetTransformUniforms(Shader* shader, ng::Graphics::Camera& camera, ng::Core::Transform& transform)
    {
        // Use the same viewport size as the gizmo code.
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        const float width  = static_cast<float>(viewport[2]);
        const float height = static_cast<float>(viewport[3]);

        if (height <= 0.0f) {
            return;
        }

        glm::mat4 view       = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(width, height);
        glm::mat4 model      = transform.GetWorldMatrix();

        shader->SetMat4("view", view);
        shader->SetMat4("projection", projection);
        shader->SetMat4("model", model);
    }

    void Mesh::SetMaterialUniforms(Shader *shader, MaterialData *m)
    {
        
        if (m == nullptr) {
            Debug::Log(LogLevel::ERROR, "MaterialData is null in Mesh::UseShader");
            return;
        }

        // Set Base Material Attributes
        shader->SetVec3("Albedo", m->Albedo);
        shader->SetVec3("AmbientColor", m->Ambient);
        shader->SetVec3("DiffuseColor", m->Diffuse);
        shader->SetVec3("SpecularColor", m->Specular);
        shader->SetVec3("EmissiveColor", m->Emissive);
        shader->SetFloat("Shininess", m->Shininess);
        shader->SetFloat("IOR", m->IOR);
        shader->SetFloat("Opacity", m->Opacity);
        shader->SetFloat("Metallic", m->Metallicness);
    }

    void Mesh::SetLightinguniforms(Shader *shader, MaterialData *m, ng::Graphics::Camera &camera)
    {
        // Hardcoded sun direction/color at the moment. No lighting support.
        shader->SetVec3("sunDirection", glm::normalize(glm::vec3(-0.3f, -1.0f, -0.5f)));
        shader->SetVec3("sunColor", glm::vec3(1.0f, 0.95f, 0.8f));
        shader->SetVec3("viewPos", camera.GetPosition());
    }

    void Mesh::BindMaterialTextures(Shader *shader, MaterialData *m)
    {
        Texture* diffuseTexture = m->FindTexture(TextureType::DIFFUSE);
        if (diffuseTexture != nullptr && diffuseTexture->id > 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseTexture->id);
            shader->SetBool("hasDiffuseMap", true);
        } else {
            shader->SetBool("hasDiffuseMap", false);
        }
        shader->SetInt("diffuseMap", 0);

        // Texture 1: Specular
        Texture* specularTexture = m->FindTexture(TextureType::SPECULAR);
        if (specularTexture != nullptr && specularTexture->id > 0) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularTexture->id);
            shader->SetBool("hasSpecularMap", true);
        } else {
            shader->SetBool("hasSpecularMap", false);
        }
        shader->SetInt("specularMap", 1);

        // Texture 2: Normal Map
        Texture* normalTexture = m->FindTexture(TextureType::NORMAL);
        if (normalTexture != nullptr && normalTexture->id > 0) {
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, normalTexture->id);
            shader->SetBool("hasNormalMap", true);
        } else {
            shader->SetBool("hasNormalMap", false);
        }
        shader->SetInt("normalMap", 2);

        // Texture 3: Emissive Map
        Texture* emissiveTexture = m->FindTexture(TextureType::EMISSIVE);
        if (emissiveTexture != nullptr && emissiveTexture->id > 0) {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, emissiveTexture->id);
            shader->SetBool("hasEmissiveMap", true);
        } else {
            shader->SetBool("hasEmissiveMap", false);
        }
        shader->SetInt("emissiveMap", 3);

        // Texture 4: Alpha Map
        Texture* alphaTexture = m->FindTexture(TextureType::ALPHA);
        if (alphaTexture != nullptr && alphaTexture->id > 0) {
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, alphaTexture->id);
            shader->SetBool("hasAlphaMap", true);
        } else {
            shader->SetBool("hasAlphaMap", false);
        }
        shader->SetInt("alphaMap", 4);

        // Texture 5: Metallic Map
        Texture* metallicTexture = m->FindTexture(TextureType::METALLIC);
        if (metallicTexture != nullptr && metallicTexture->id > 0) {
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, metallicTexture->id);
            shader->SetBool("hasMetallicMap", true);
        } else {
            shader->SetBool("hasMetallicMap", false);
        }
        shader->SetInt("metallicMap", 5);
    }

    Mesh::Mesh()
    {
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

    Mesh::Mesh(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texCoords, std::vector<unsigned int> inds) {
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

        SetupMesh();
    }

    Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds) {
        this->vertices = verts;
        this->indices = inds;

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

    void Mesh::SetupMesh() {
       
        if (!ValidateMeshData()) {
            Debug::Log(ERROR, "Failed to validate mesh data during Mesh::SetupMesh");
            return;
        }

        EnsureMaterialExists();
        CreateGPUBuffers();
        SetupVertexAttributes();

        // Unbind the VAO. This stops any future calls from changing this VAO by accident.
        glBindVertexArray(0);

    }

    void Mesh::UseShader(ng::Graphics::Camera& camera, ng::Core::Transform& transform) {
     
        if (material == nullptr) {
            Debug::Log(LogLevel::ERROR, "Mesh has no material during UseShader!");
            return;
        }

        Shader* shader = material->GetShader();
        if (shader == nullptr) {
            Debug::Log(LogLevel::ERROR, "No shader assigned to material!");
            return;
        }

        Debug::Log(VERBOSE, "Using shader ID %d..", shader->ID);
        shader->Use();
        MaterialData* mat = material->GetMaterialData();

        Debug::Log(VERBOSE, "Applying shader uniforms to shader %d..", shader->ID);

        SetTransformUniforms(shader, camera, transform);

        // Albedo, Diffuse, etc.
        SetMaterialUniforms(shader, mat);

        // Lighting Parameters
        SetLightinguniforms(shader, mat, camera);

        // Texture 0: Diffuse
        BindMaterialTextures(shader, mat);
    }

}