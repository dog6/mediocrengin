#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ng::Graphics {

        class Camera {

            public:
                glm::vec3 position;
                glm::vec3 target;
                glm::vec3 up;

                Camera() {
                    position = glm::vec3(0.0f, 0.0f, 5.0f);  // Camera 5 units back
                    target = glm::vec3(0.0f, 0.0f, 0.0f);    // Looking at origin
                    up = glm::vec3(0.0f, 1.0f, 0.0f);        // Y is up
                }

                glm::mat4 GetViewMatrix() {
                    return glm::lookAt(position, target, up);
                }

                glm::mat4 GetProjectionMatrix(float width, float height) {
                    return glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
                }

        };

}