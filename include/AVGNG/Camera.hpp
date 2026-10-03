#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ng::Graphics {

        class Camera {

            private:
                glm::vec3 position;
                glm::vec3 target;
                glm::vec3 up;

            public:
       

                Camera() {
                    position = glm::vec3(0.0f, 0.0f, 5.0f);  // Camera 5 units back
                    target = glm::vec3(0.0f, 0.0f, 0.0f);    // Looking at origin
                    up = glm::vec3(0.0f, 1.0f, 0.0f);        // Y is up
                }

                // Getters / Setters
                glm::mat4 GetViewMatrix() {
                    return glm::lookAt(position, target, up);
                }

                glm::mat4 GetProjectionMatrix(float viewport_width, float viewport_height) {
                    return glm::perspective(glm::radians(45.0f), viewport_width / viewport_height, 0.1f, 100.0f);
                }

                glm::vec3 GetPosition() { return position; }
                void SetPosition(const glm::vec3 pos) { this->position = pos; }

                glm::vec3 GetTarget() { return target;  }
                void SetTarget(const glm::vec3& target) { this->target = target; }

                glm::vec3 GetUp() { return this->up; }
				void SetUp(const glm::vec3& up) { this->up = up; }


        };

}