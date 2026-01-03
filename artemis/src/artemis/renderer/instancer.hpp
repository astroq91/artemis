#pragma once
#include "artemis/assets/resource_library.hpp"
#include "artemis/renderer/mesh.hpp"
#include "artemis/vulkan/vertex_buffer_description.hpp"
#include "glm/glm.hpp"

namespace artemis {

struct MeshInstance {
    alignas(16) glm::mat4 model;
};

constexpr size_t k_mesh_instance_binding = 1;
const VertexBufferDescription k_mesh_instance_desc(
    k_mesh_instance_binding, sizeof(MeshInstance),
    vk::VertexInputRate::eInstance,
    {
        {.type = VertexAttributeType::Float4, .location = 1, .offset = 0},
        {.type = VertexAttributeType::Float4,
         .location = 2,
         .offset = sizeof(glm::vec4)},
        {.type = VertexAttributeType::Float4,
         .location = 3,
         .offset = 2 * sizeof(glm::vec4)},
        {.type = VertexAttributeType::Float4,
         .location = 4,
         .offset = 3 * sizeof(glm::vec4)},
    });

struct InstanceInfo {
    ResourceHandle<Mesh> mesh_handle;
};

template <typename T> struct InstanceCollection {
    std::vector<T> instances;
    std::vector<InstanceInfo> order;
};

class Instancer {
  public:
    void sort();
    void clear();
    void add_forward_instance(ResourceHandle<Mesh> mesh_handle,
                              const glm::mat4& model);
    const InstanceCollection<MeshInstance>& get_forward_instances() const {
        return forward_instances_;
    };

  private:
    void sort_forward();

  private:
    InstanceCollection<MeshInstance> forward_instances_;
};
} // namespace artemis
