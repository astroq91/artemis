#include "vertex_buffer_description.hpp"
#include "artemis/core/log.hpp"
#include "vulkan/vulkan.hpp"

namespace artemis {
vk::Format get_format(VertexAttributeType type) {
    switch (type) {
    case VertexAttributeType::Float:
        return vk::Format::eR32Sfloat;
    case VertexAttributeType::Float2:
        return vk::Format::eR32G32Sfloat;
    case VertexAttributeType::Float3:
        return vk::Format::eR32G32B32Sfloat;
    case VertexAttributeType::Float4:
        return vk::Format::eR32G32B32A32Sfloat;
    default: {
        Log::get()->error("(vk) Vertex attribute type not handled.");
        return vk::Format::eUndefined;
    }
    }
}
VertexBufferDescription::VertexBufferDescription(
    uint32_t binding, uint32_t stride, vk::VertexInputRate input_rate,
    const std::vector<VertexAttributeDescription>& attributes) {
    binding_desc_ =
        vk::VertexInputBindingDescription(binding, stride, input_rate);
    attrib_descs_.resize(attributes.size());
    for (int i = 0; i < attributes.size(); i++) {
        attrib_descs_[i] = vk::VertexInputAttributeDescription(
            attributes[i].location, binding, get_format(attributes[i].type),
            attributes[i].offset);
    }
}
} // namespace artemis
