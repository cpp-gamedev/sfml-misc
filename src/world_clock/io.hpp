#pragma once
#include <iomanip>
#include <sstream>
#include <world_clock/world_clock.hpp>

namespace misc {
inline std::ostream& operator<<(std::ostream& out, world_hour_t wt) { return out << wt.hour(); }

inline std::ostream& operator<<(std::ostream& out, world_clock_t const& clock) {
	for (auto const& entry : clock) { out << entry.name << ": " << entry.hour << '\n'; }
	return out;
}
} // namespace misc
