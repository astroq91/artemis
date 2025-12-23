#pragma once
#include "artemis/vulkan/shader.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>

namespace artemis {
template <typename T> struct ResourceHandle {
    size_t index;
    uint32_t version;
};

template <typename T> class ResourcePool {
  public:
    ResourceHandle<T> insert(std::unique_ptr<T>&& resource) {
        if (free_list_.empty()) {
            uint32_t idx = free_list_.back();
            free_list_.pop_back();
            slots_[idx].resource = std::move(resource);
            return {
                .index = idx,
                .version = slots_[idx].version,
            };
        }

        slots_.emplace_back({.resource = std::move(resource), .version = 1});
        return {
            .index = slots_.size() - 1,
            .version = 1,
        };
    }

    T* get(const ResourceHandle<T>& handle) {
        if (handle.index >= slots_.size() ||
            handle.version != slots_[handle.index].version) {
            return nullptr;
        }
        return slots_[handle.index].resource.get();
    }

    void erase(const ResourceHandle<T>& handle) {
        if (handle.index >= slots_.size() ||
            handle.version != slots_[handle.index].version) {
            return;
        }

        slots_[handle.index].resource.reset();
        slots_[handle.index].version++;
        free_list_.push_back(handle.index);
    }

  private:
    struct Slot {
        std::unique_ptr<T> resource;
        uint32_t version;
    };

    std::vector<Slot> slots_;
    std::vector<uint32_t> free_list_;
};

class ResourceLibrary {
  public:
    ResourcePool<Shader>& get_shader_pool() { return shader_pool_; }

  private:
    ResourcePool<Shader> shader_pool_;
};
} // namespace artemis
