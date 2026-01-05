#pragma once
#include "artemis/core/transform.hpp"
namespace artemis {
struct Camera {
    glm::mat4 projection;
    glm::mat4 view;

    Camera() = default;
    Camera(Transform& transform, float fov, float aspect, float near,
           float far) {
        view = glm::inverse(transform.get_mat4());
        projection = glm::perspectiveRH_ZO(fov, aspect, near, far);
    }
};
} // namespace artemis
