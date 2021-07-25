#pragma once
#include <maths/maths.hpp>

namespace misc {
// rotation: right-handed i x j (+radians counter-clockwise), identity facing up (+y)
struct orient_t {
	inline static maths::dir2_t const identity = {0.0f, 1.0f};

	maths::dir2_t dir_n = identity;

	orient_t() = default;
	orient_t(sf::Vector2f dir) noexcept : dir_n(maths::normalize(dir)) {}
	orient_t(maths::rad_t rad) noexcept : dir_n(maths::dir(rad)) {}

	maths::rad_t rad() const noexcept { return maths::rad(dir_n); }
	f32 deg() const noexcept { return maths::deg(rad()); }
};
} // namespace misc
