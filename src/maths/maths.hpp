#pragma once
#include <cmath>
#include <random>
#include <type_traits>
#include <SFML/System/Vector2.hpp>
#include <types.hpp>

namespace misc::maths {
using rad_t = f32;

template <bool V, typename...>
constexpr bool value_v = V;

// normalized direction (magnitude = 1)
using dir2_t = sf::Vector2f;
// point in normalized coords ([0-1], [0-1])
using npt_t = sf::Vector2f;

constexpr f32 pi = 3.1415;
dir2_t const up_n = {0.0f, 1.0f};
dir2_t const down_n = {0.0f, -1.0f};
dir2_t const left_n = {-1.0f, 0.0f};
dir2_t const right_n = {1.0f, 0.0f};

constexpr rad_t rad(f32 degrees) noexcept;
constexpr f32 deg(rad_t rad) noexcept;

template <typename T>
sf::Vector2<T> operator*(sf::Vector2<T> lhs, sf::Vector2<T> rhs) noexcept;

f32 sqr_mag(sf::Vector2f vec) noexcept;
f32 mag(sf::Vector2f vec) noexcept;
f32 mag_or(sf::Vector2f vec, f32 fallback) noexcept;
sf::Vector2f normalize(sf::Vector2f vec) noexcept;
dir2_t normalize_safe(sf::Vector2f vec) noexcept;

dir2_t dir(rad_t rad) noexcept;
rad_t rad(dir2_t dir_n) noexcept;

template <typename Ret, typename In>
Ret cast(sf::Vector2<In> in) noexcept;

template <typename T>
T random_range(T lo, T hi) noexcept;

// impl

constexpr rad_t rad(f32 degrees) noexcept { return degrees * pi / 180.0f; }
constexpr f32 deg(rad_t radians) noexcept { return 180.0f * radians / pi; }

template <typename T>
sf::Vector2<T> operator*(sf::Vector2<T> lhs, sf::Vector2<T> rhs) noexcept {
	return {lhs.x * rhs.x, lhs.y * rhs.y};
}

inline f32 sqr_mag(sf::Vector2f vec) noexcept { return vec.x * vec.x + vec.y * vec.y; }
inline f32 mag(sf::Vector2f vec) noexcept { return std::sqrt(sqr_mag(vec)); }

inline f32 mag_or(sf::Vector2f vec, f32 fallback) noexcept {
	f32 const ret = std::sqrt(sqr_mag(vec));
	return ret == 0.0f ? fallback : ret;
}

inline sf::Vector2f normalize(sf::Vector2f vec) noexcept {
	auto const m = mag(vec);
	return {vec.x / m, vec.y / m};
}

inline dir2_t normalize_safe(sf::Vector2f vec) noexcept {
	if (sqr_mag(vec) == 0.0f) { return up_n; }
	return normalize(vec);
}

inline dir2_t dir(rad_t rad) noexcept { return {std::sin(rad), std::cos(rad)}; }
inline rad_t rad(dir2_t dir_n) noexcept {
	if (dir_n.y == 0.0f) { return rad(dir_n.x == 1.0f ? -90.0f : 90.0f); }
	auto const atan = std::atan(dir_n.x / dir_n.y);
	return dir_n.y > 0.0f ? atan : atan + rad(180.0f);
}

template <typename Ret, typename In>
Ret cast(sf::Vector2<In> in) noexcept {
	using type = decltype(std::declval<Ret>().x);
	return Ret{static_cast<type>(in.x), static_cast<type>(in.y)};
}

template <typename T>
T random_range(T lo, T hi) noexcept {
	thread_local std::random_device rd;
	thread_local std::default_random_engine eng{rd()};
	if constexpr (std::is_integral_v<T>) {
		std::uniform_int_distribution<T> dist(lo, hi);
		return dist(eng);
	} else if constexpr (std::is_floating_point_v<T>) {
		std::uniform_real_distribution<T> dist(lo, hi);
		return dist(eng);
	} else {
		static_assert(value_v<false, T>, "Invalid type");
	}
}
} // namespace misc::maths
