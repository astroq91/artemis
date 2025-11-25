#pragma once

namespace artemis {
/**
 * Defines the base class that all events inherit from.
 */
struct Event {
    virtual ~Event() = default;
};
} // namespace artemis
