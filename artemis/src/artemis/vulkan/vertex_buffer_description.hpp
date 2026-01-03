#pragma once
#include <cstddef>
#include <cstdint>
#include <vulkan/vulkan.hpp>
namespace artemis {

enum class VertexAttributeType { Float, Float2, Float3, Float4 };

struct VertexAttributeDescription {
    VertexAttributeType type;
    uint32_t location;
    uint32_t offset;
};

class VertexBufferDescription {
  public:
    VertexBufferDescription(
        uint32_t binding, uint32_t stride, vk::VertexInputRate input_rate,
        const std::vector<VertexAttributeDescription>& attributes);

  private:
    vk::VertexInputBindingDescription binding_desc_;
    std::vector<vk::VertexInputAttributeDescription> attrib_descs_;
};
} // namespace artemis
