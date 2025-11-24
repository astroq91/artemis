#pragma once
#include "artemis/vulkan/shader.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace artemis {
struct ResourceHandle {
    size_t index;
    uint32_t version;
};

struct ResourceHandleHasher {
    size_t operator()(const ResourceHandle& p) const { return p.index; }
};

class ResourceLibrary {
  public:
    ResourceHandle insert_shader(std::unique_ptr<Shader>&& resource) {
        if (!shaders_free_list_.empty()) {
            uint32_t index = shaders_free_list_.back();
            shaders_free_list_.pop_back();
            shaders_[index].resource = std::move(resource);
            return ResourceHandle{
                .index = index,
                .version = shaders_[index].version,
            };
        }

        shaders_.emplace_back(Slot{
            .resource = std::move(resource),
            .version = 1,
        });
        return ResourceHandle{
            .index = shaders_.size() - 1,
            .version = 1,
        };
    }

    Shader* get_shader(const ResourceHandle& handle) const {
        if (handle.index >= shaders_.size() ||
            handle.version != shaders_[handle.index].version) {
            return nullptr;
        }

        return shaders_[handle.index].resource.get();
    }

  private:
    template <typename T> struct Slot {
        std::unique_ptr<T> resource;
        uint32_t version;
    };

    std::vector<Slot<Shader>> shaders_;
    std::vector<uint32_t> shaders_free_list_;
};
} // namespace artemis
