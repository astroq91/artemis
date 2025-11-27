#include "renderer.hpp"
namespace artemis {
Renderer::Renderer(VulkanContext* context) : context_(context) {}
void Renderer::begin_frame() {}
void Renderer::end_frame() {}
} // namespace artemis
