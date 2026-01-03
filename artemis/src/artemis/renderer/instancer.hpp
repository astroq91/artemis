#pragma once
#include "artemis/assets/resource_library.hpp"
#include "artemis/renderer/mesh.hpp"
#include "glm/glm.hpp"

namespace artemis {

struct MeshInstance {
    alignas(16) glm::mat4 model;
};

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
