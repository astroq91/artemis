#pragma once
#include "artemis/assets/resource_library.hpp"
#include "glm/glm.hpp"
namespace artemis {

struct MeshInstance {
    alignas(16) glm::mat4 model;
};

template <typename T> struct InstanceCollection {
    std::vector<T> instances;
    std::vector<uint64_t> order;
};

class Instancer {
  public:
    void sort();
    void add_forward_instance(ResourceHandle<MeshInstance> mesh_handle,
                              const glm::mat4& model);
    const std::vector<MeshInstance> get_forward_instances() const {
        return forward_instances_.instances;
    };

  private:
    void sort_forward();

  private:
    InstanceCollection<MeshInstance> forward_instances_;
};
} // namespace artemis
