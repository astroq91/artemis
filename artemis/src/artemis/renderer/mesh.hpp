#pragma once
#include "artemis/vulkan/index_buffer.hpp"
#include "artemis/vulkan/vertex_buffer.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
namespace artemis {
class Mesh {
  public:
    Mesh() = default;
    Mesh(VulkanContext* context_, void* vertices, size_t vertices_size,
         void* indices, size_t indices_size) {
        vb_ = VertexBuffer(context_, vertices, vertices_size);
        ib_ = IndexBuffer(context_, indices, indices_size);
    }

    VertexBuffer& get_vertex_buffer() { return vb_; }
    IndexBuffer& get_index_buffer() { return ib_; }

  private:
    VertexBuffer vb_;
    IndexBuffer ib_;
};
} // namespace artemis
