#pragma once
#include <chrono>
#include <types.hpp>

namespace misc {
using clock_t = std::chrono::steady_clock;
using sec_t = std::chrono::duration<f32>;

struct delta_time_t {
	clock_t::time_point point = clock_t::now();

	sec_t operator++() noexcept {
		auto const now = clock_t::now();
		auto const ret = now - point;
		point = now;
		return ret;
	}
};
} // namespace misc
