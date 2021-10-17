#include <cassert>
#include <cstdint>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <context.hpp>
#include <delta_time.hpp>
#include <gfx/follow_eye.hpp>
#include <maths/maths.hpp>
#include <pini/pini.hpp>
#include <world_clock/gui.hpp>
#include <world_clock/io.hpp>
#include "SFML/Graphics/Color.hpp"
#include "types.hpp"
#include "world_clock/world_clock.hpp"
#include "world_clock/world_hour.hpp"

using namespace misc;

namespace {
using key_t = sf::Keyboard::Key;

class input_t {
  public:
	void press(key_t key) { m_held.insert(key); }
	void release(key_t key) noexcept { m_held.erase(key); }
	void reset() noexcept { m_held.clear(); }
	bool held(key_t key) const noexcept { return m_held.find(key) != m_held.end(); }
	template <typename... T>
	bool any_held(T... t) const noexcept {
		return (held(t) || ...);
	}

	void update(std::vector<sf::Event> const& events) {
		for (auto const& event : events) {
			switch (event.type) {
			case sf::Event::KeyPressed: press(event.key.code); break;
			case sf::Event::KeyReleased: release(event.key.code); break;
			default: break;
			}
		}
	}

  private:
	std::set<key_t> m_held;
};

class clock_ticker_t {
  public:
	clock_ticker_t(input_t const* input, world_clock_t* clock) noexcept : m_input(input), m_clock(clock) {}

	void operator()(sec_t dt) noexcept {
		if (m_input->any_held(key_t::Enter, key_t::R)) { update(flag::zeroing); }
		static constexpr f32 translation_scale = 10.0f;
		if (test(flag::zeroing)) {
			static constexpr f32 zeroing_scale = 5.0f;
			hour_t const delta = zeroing_scale * translation_scale * dt.count();
			if (m_offset.hour() > delta) {
				add(-delta);
			} else {
				add(-m_offset.hour());
				update(flag::none, flag::zeroing);
			}
		} else {
			f32 delta{};
			if (m_input->any_held(key_t::Down, key_t::Left, key_t::S, key_t::A)) { delta += -1.0f; }
			if (m_input->any_held(key_t::Up, key_t::Right, key_t::W, key_t::D)) { delta += 1.0f; }
			add(translation_scale * delta * dt.count());
		}
		auto next_blink = [this]() { return maths::random_range(m_blink.interval.first.count(), m_blink.interval.second.count()); };
		if (m_blink.next_blink_in <= sec_t{}) {
			m_blink.next_blink_in = sec_t{next_blink()};
			m_blink.stop_blink_in = m_blink.duration;
			update(flag::blinking);
		}
		if (test(flag::blinking)) {
			m_blink.stop_blink_in -= dt;
			if (m_blink.stop_blink_in <= sec_t{}) {
				m_blink.next_blink_in = sec_t{next_blink()};
				update(flag::none, flag::blinking);
			}
		} else {
			m_blink.next_blink_in -= dt;
		}
	}

	bool blink() const noexcept { return test(flag::blinking); }

	struct {
		pair_t<sec_t> interval = {sec_t{3.0f}, sec_t{8.0f}};
		sec_t duration = sec_t{0.1f};

		sec_t next_blink_in{};
		sec_t stop_blink_in{};
	} m_blink;

  private:
	using flags_t = u8;
	enum flag : flags_t { none = 0, zeroing = 1 << 0, blinking = 1 << 1 };

	input_t const* m_input;
	world_clock_t* m_clock;
	world_hour_t m_offset;

	flags_t m_flags = flag::none;

	bool test(flags_t f) const noexcept { return (m_flags & f) == f; }
	void update(flags_t set, flags_t unset = 0) noexcept {
		m_flags |= set;
		m_flags &= ~unset;
	}

	void add(hour_t offset) noexcept {
		(*m_clock) += offset;
		m_offset += offset;
	}
};
} // namespace

u32 random_number(u32 min, u32 max) {
	std::random_device r;
	std::default_random_engine re(r());
	std::uniform_int_distribution<u32> uniform_dist(min, max);
	return uniform_dist(re);
}

bool load_timezones(std::filesystem::path const& filename, world_clock_t& clock) {
	pn::pini timezones;
	if (!timezones.load_file(filename)) { return false; }

	for (auto& pair : timezones) {
		u32 color = random_number(255, UINT32_MAX);
		color |= 0xff;
		std::cout << pair.first << " " << pair.second << " " << std::hex << color << '\n';
		clock.add(pair.first, color, timezones.get_double(pair.first));
	}
	return true;
}
u32 hex_to_int(std::string_view const& hex_value) {
	u32 assigned_color;
	std::stringstream ss;
	ss << std::hex << hex_value;
	ss >> assigned_color;
	return assigned_color;
}
sf::Color generate_color(std::filesystem::path const& themefile, world_clock_t const& clock) {
	pn::pini colors;
	if (!colors.load_file(themefile)) { return sf::Color::White; }
	world_hour_t primary_hour = clock.begin()->hour;
	std::string hour_str = std::to_string(static_cast<int>(primary_hour.hour()));
	u32 assigned_color = hex_to_int(colors.get_string(hour_str));
	sf::Color clock_color(assigned_color);
	return clock_color;
}
int main() {
	misc::context_t ctx("World Clock");
	ctx.setVerticalSyncEnabled(true);
	world_clock_t clock;
	load_timezones("src/timezones.pini", clock);
	std::cout << clock << '\n';
	input_t input;
	delta_time_t dt;
	clock_ticker_t tick{&input, &clock};
	while (ctx.running()) {
		input.update(ctx.poll());
		tick(++dt);
		if (auto drawer = ctx.drawer(generate_color("src/colors.pini", clock))) {
			world_clock_drawer_t::in_t in;
			in.blink = tick.blink();
			in.mouse_pos = ctx.mouse_pos();
			world_clock_drawer_t{}(drawer, clock, in);
		}
	}
	std::cout << "\nCompleted successfully\n";
}
