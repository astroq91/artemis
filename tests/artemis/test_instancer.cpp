#include <gtest/gtest.h>
#include "artemis/assets/resource_library.hpp"
#include "artemis/core/log.hpp"
#include "artemis/renderer/instancer.hpp"

using namespace artemis;

TEST(artemis, test_forward_instancer) {
    Log::init();
    Instancer instancer;
    ResourceHandle handle1(1, 1);
    ResourceHandle handle2(2, 1);
    ResourceHandle handle3(3, 1);
    ResourceHandle handle4(4, 1);
    instancer.add_forward_instance(handle1, glm::mat4(1.0f));
    instancer.add_forward_instance(handle3, glm::mat4(3.0f));
    instancer.add_forward_instance(handle4, glm::mat4(4.0f));
    instancer.add_forward_instance(handle2, glm::mat4(2.0f));
    instancer.sort();
    auto instances = instancer.get_forward_instances();
    ASSERT_EQ(instances[0].model, glm::mat4(1.0f));
    ASSERT_EQ(instances[1].model, glm::mat4(2.0f));
    ASSERT_EQ(instances[2].model, glm::mat4(3.0f));
    ASSERT_EQ(instances[3].model, glm::mat4(4.0f));
}
