#pragma once
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/index_buffer.hpp"
#include "artemis/vulkan/vertex_buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
namespace artemis {
class Mesh {
  public:
    Mesh() = default;
    Mesh(VulkanContext* context_, DeferredQueue* deferred_queue,
         CommandBuffer* cmd_buf, void* vertices, size_t vertices_size,
         void* indices, size_t index_count, size_t index_size)
        : vb_(context_, deferred_queue, cmd_buf, vertices, vertices_size),
          ib_(context_, deferred_queue, cmd_buf, indices, index_count,
              index_size) {}

    VertexBuffer& get_vertex_buffer() { return vb_; }
    IndexBuffer& get_index_buffer() { return ib_; }

  private:
    VertexBuffer vb_;
    IndexBuffer ib_;
};
} // namespace artemis
