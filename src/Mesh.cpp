
#include <AVGNG/Mesh.hpp>


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

   

}