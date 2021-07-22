#pragma once
#include <cstdint>
#include <utility>

namespace misc {
using s32 = std::uint32_t;
using u8 = std::uint8_t;
using u32 = std::uint32_t;
using f32 = float;

template <typename T, typename U = T>
using pair_t = std::pair<T, U>;
} // namespace misc
