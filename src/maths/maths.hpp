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

constexpr f32 pi = 3.1415;
sf::Vector2f const n_up = {0.0f, 1.0f};
sf::Vector2f const n_down = {0.0f, -1.0f};
sf::Vector2f const n_left = {-1.0f, 0.0f};
sf::Vector2f const n_right = {1.0f, 0.0f};

constexpr rad_t rad(f32 degrees) noexcept;
constexpr f32 deg(rad_t rad) noexcept;

f32 sqr_mag(sf::Vector2f vec) noexcept;
f32 mag(sf::Vector2f vec) noexcept;
f32 mag_or(sf::Vector2f vec, f32 fallback) noexcept;
sf::Vector2f norm(sf::Vector2f vec) noexcept;
sf::Vector2f safe_norm(sf::Vector2f vec) noexcept;

sf::Vector2f dir(rad_t rad) noexcept;
rad_t rad(sf::Vector2f n_dir) noexcept;

template <typename Ret, typename In>
Ret cast(sf::Vector2<In> in) noexcept;

template <typename T>
T random_range(T lo, T hi) noexcept;

// impl

constexpr rad_t rad(f32 degrees) noexcept { return degrees * pi / 180.0f; }
constexpr f32 deg(rad_t radians) noexcept { return 180.0f * radians / pi; }
inline f32 sqr_mag(sf::Vector2f vec) noexcept { return vec.x * vec.x + vec.y * vec.y; }
inline f32 mag(sf::Vector2f vec) noexcept { return std::sqrt(sqr_mag(vec)); }

inline f32 mag_or(sf::Vector2f vec, f32 fallback) noexcept {
	f32 const ret = std::sqrt(sqr_mag(vec));
	return ret == 0.0f ? fallback : ret;
}

inline sf::Vector2f norm(sf::Vector2f vec) noexcept {
	auto const m = mag(vec);
	return {vec.x / m, vec.y / m};
}

inline sf::Vector2f safe_norm(sf::Vector2f vec) noexcept {
	auto const m = mag_or(vec, 1.0f);
	return {vec.x / m, vec.y / m};
}

inline sf::Vector2f dir(rad_t rad) noexcept { return {std::sin(rad), std::cos(rad)}; }
inline rad_t rad(sf::Vector2f n_dir) noexcept {
	if (n_dir.y == 0.0f) { return rad(n_dir.x == 1.0f ? -90.0f : 90.0f); }
	auto const atan = std::atan(n_dir.x / n_dir.y);
	return n_dir.y > 0.0f ? atan : atan + rad(180.0f);
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
