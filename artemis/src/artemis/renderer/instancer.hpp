#pragma once
#include "artemis/assets/resource_library.hpp"
#include "glm/glm.hpp"
namespace artemis {

struct MeshInstance {
    alignas(16) glm::mat4 model;
};

class Instancer {
  public:
    void sort();
    void add_forward_instance(ResourceHandle mesh_handle,
                              const glm::mat4& model);
    const std::vector<MeshInstance> get_forward_instances() const {
        return forward_instances_;
    };

  private:
    void sort_forward();

  private:
    std::vector<MeshInstance> forward_instances_;
    std::vector<uint64_t> forward_instances_order_;
};
} // namespace artemis
