#pragma once
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
namespace artemis {
struct Transform {
    void set_position(const glm::vec3& position) {
        position_ = position;
        is_dirty_ = true;
    }
    void set_rotation(const glm::quat& rotation) {
        rotation_ = rotation;
        is_dirty_ = true;
    }
    void set_scale(const glm::vec3& scale) {
        scale_ = scale;
        is_dirty_ = true;
    }

    const glm::mat4& get_mat4() {
        if (is_dirty_) {
            matrix_ = glm::scale(glm::mat4(1.0f), scale_) *
                      glm::toMat4(rotation_) *
                      glm::translate(glm::mat4(1.0f), position_);
            is_dirty_ = false;
        }
        return matrix_;
    }

  private:
    bool is_dirty_ = true;
    glm::vec3 position_{0.0f};
    glm::quat rotation_{0.0f, 0.0f, 0.0f, 1.0f};
    glm::vec3 scale_{1.0f};
    glm::mat4 matrix_{1.0f};
};
} // namespace artemis
