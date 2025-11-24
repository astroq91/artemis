#pragma once

#include <type_traits>
namespace artemis {
template <typename T> constexpr bool is_flag_enum = false;

template <typename E>
concept flag_enum = std::is_enum_v<E> && is_flag_enum<E>;

template <flag_enum E> constexpr E operator|(E lhs, E rhs) {
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template <flag_enum E> constexpr E operator&(E lhs, E rhs) {
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
}

template <flag_enum E> constexpr E operator^(E lhs, E rhs) {
    using U = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
}

template <flag_enum E> constexpr E operator~(E e) {
    using U = std::underlying_type_t<E>;
    return static_cast<E>(~static_cast<U>(e));
}

template <flag_enum E> constexpr E& operator|=(E& lhs, E rhs) {
    return lhs = lhs | rhs;
}

template <flag_enum E> constexpr E& operator&=(E& lhs, E rhs) {
    return lhs = lhs & rhs;
}

} // namespace artemis
