#pragma once
#include <types.hpp>

namespace misc {
using hour_t = f32;

constexpr hour_t total_hours = 24.0f;

class world_hour_t {
  public:
	constexpr explicit world_hour_t(hour_t hour = {}) noexcept : m_hour(mod(hour)) {}

	constexpr hour_t hour() const noexcept { return m_hour; }

	constexpr bool operator==(world_hour_t const& rhs) const noexcept { return m_hour == rhs.m_hour; }

	constexpr world_hour_t operator-() noexcept { return world_hour_t{-m_hour}; }
	constexpr world_hour_t& operator+=(hour_t rhs) noexcept { return (m_hour = mod(m_hour + rhs), *this); }
	constexpr world_hour_t& operator-=(hour_t rhs) noexcept { return (m_hour = mod(m_hour - rhs), *this); }

	friend constexpr world_hour_t operator+(world_hour_t l, hour_t r) { return l += r; }
	friend constexpr world_hour_t operator-(world_hour_t l, hour_t r) { return l -= r; }

  private:
	static constexpr hour_t abs(hour_t in) noexcept { return in < 0.0f ? -in : in; }
	static constexpr bool equal(hour_t l, hour_t r, hour_t epsilon = 0.1f) noexcept { return abs(l - r) < epsilon; }
	static constexpr hour_t mod(hour_t in) noexcept {
		while (in > total_hours) { in -= total_hours; }
		while (in < 0.0f) { in += total_hours; }
		return in;
	}

	hour_t m_hour;
};
} // namespace misc
