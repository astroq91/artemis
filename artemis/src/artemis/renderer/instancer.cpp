#include "instancer.hpp"
#include <algorithm>
#include <numeric>

namespace artemis {

uint64_t sort_key_mesh(ResourceHandle mesh_handle) {
    return static_cast<uint64_t>(mesh_handle.index) << 50;
}

void Instancer::sort() { sort_forward(); }
void Instancer::add_forward_instance(ResourceHandle mesh_handle,
                                     const glm::mat4& model) {
    forward_instances_.emplace_back(model);
    forward_instances_order_.push_back(sort_key_mesh(mesh_handle));
}

void Instancer::sort_forward() {
    std::vector<size_t> indices(forward_instances_.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::sort(
        indices.begin(), indices.end(), [this](const auto& a, const auto& b) {
            return forward_instances_order_[a] < forward_instances_order_[b];
        });

    for (size_t i = 0; i < indices.size(); i++) {
        while (indices[i] != i) {
            size_t target = indices[i];

            // TODO: Maybe order_?
            std::swap(forward_instances_[i], forward_instances_[target]);
            std::swap(indices[i], indices[target]);
        }
    }
}
} // namespace artemis
