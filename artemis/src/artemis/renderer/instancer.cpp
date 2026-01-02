#include "instancer.hpp"
#include <algorithm>
#include <numeric>

namespace artemis {

uint64_t sort_key_mesh(ResourceHandle<MeshInstance> mesh_handle) {
    return static_cast<uint64_t>(mesh_handle.index) << 50;
}

void Instancer::sort() { sort_forward(); }
void Instancer::add_forward_instance(ResourceHandle<MeshInstance> mesh_handle,
                                     const glm::mat4& model) {
    forward_instances_.instances.emplace_back(model);
    forward_instances_.order.push_back(sort_key_mesh(mesh_handle));
}

void Instancer::sort_forward() {
    std::vector<size_t> indices(forward_instances_.instances.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(
        indices.begin(), indices.end(), [this](const auto& a, const auto& b) {
            return forward_instances_.order[a] < forward_instances_.order[b];
        });

    auto old_instances = forward_instances_.instances;
    auto old_order = forward_instances_.order;
    for (size_t i = 0; i < indices.size(); i++) {
        forward_instances_.instances[i] = old_instances[indices[i]];
        forward_instances_.order[i] = old_order[indices[i]];
    }
}
} // namespace artemis
