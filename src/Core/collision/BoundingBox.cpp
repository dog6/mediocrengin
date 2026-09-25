#include "AVGNG/Core/collision/BoundingBox.hpp"
#include "AVGNG/Graphics/Camera.hpp"
#include "AVGNG/Graphics/Vertex.hpp"

using namespace ng::Core;

    ng::Graphics::Vertex MakeVertex(float x, float y, float z)
    {
        ng::Graphics::Vertex v;
        v.position = glm::vec3(x, y, z);
        v.normal   = glm::vec3(0.0f, 0.0f, 1.0f);
        v.texCoord = glm::vec2(0.0f, 0.0f);
        return v;
    }

  // 8 corners of a unit cube, centered on the origin
    std::vector<ng::Graphics::Vertex> BuildCubeVertices()
    {
        return {
            MakeVertex(-0.5f, -0.5f, -0.5f), // 0
            MakeVertex( 0.5f, -0.5f, -0.5f), // 1
            MakeVertex( 0.5f,  0.5f, -0.5f), // 2
            MakeVertex(-0.5f,  0.5f, -0.5f), // 3
            MakeVertex(-0.5f, -0.5f,  0.5f), // 4
            MakeVertex( 0.5f, -0.5f,  0.5f), // 5
            MakeVertex( 0.5f,  0.5f,  0.5f), // 6
            MakeVertex(-0.5f,  0.5f,  0.5f)  // 7
        };
    }

    // 12 edges, 2 indices for each edge
    std::vector<unsigned int> BuildCubeLineIndices()
    {
        return {
            0,1, 1,2, 2,3, 3,0,   // back face
            4,5, 5,6, 6,7, 7,4,   // front face
            0,4, 1,5, 2,6, 3,7    // edges that connect the faces
        };
    }

BoundingBox::BoundingBox()
{

    // Create mesh
    this->mesh = new ng::Graphics::Mesh(BuildCubeVertices(), BuildCubeLineIndices());
    this->mesh->drawMode = GL_LINES;

}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::ComputeFromVertices(const std::vector<ng::Graphics::Vertex>& vertices)
{
    if (vertices.empty()) return;

    bbMin = vertices[0].position;
    bbMax = vertices[0].position;

    for (const ng::Graphics::Vertex& v : vertices)
    {
        bbMin = glm::min(bbMin, v.position);
        bbMax = glm::max(bbMax, v.position);
    }
}

void BoundingBox::Draw(ng::Graphics::Camera& cam, Transform& tf)
{
     glm::vec3 center = (bbMin + bbMax) * 0.5f;
    glm::vec3 size   = bbMax - bbMin;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), center)
                    * glm::scale(glm::mat4(1.0f), size);
}

bool BoundingBox::Overlaps(const glm::vec3& minA, const glm::vec3& maxA, const glm::vec3& minB, const glm::vec3& maxB)
{
    return (minA.x <= maxB.x && maxA.x >= minB.x) &&
           (minA.y <= maxB.y && maxA.y >= minB.y) &&
           (minA.z <= maxB.z && maxA.z >= minB.z);
}
