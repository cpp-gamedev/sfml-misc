#include <cassert>
#include <iostream>
#include <context.hpp>
#include <scene.hpp>

#include <set>
#include <delta_time.hpp>
#include <world_clock/gui.hpp>
#include <world_clock/io.hpp>

/*
- poll
- update / tick (based on dt and new events)
- render / draw
*/

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

void tick(world_clock_t& out_clock, input_t const& input, sec_t dt) noexcept {
	static constexpr f32 scale = 10.0f;
	f32 delta{};
	if (input.any_held(key_t::Down, key_t::Left, key_t::S, key_t::A)) { delta += -1.0f; }
	if (input.any_held(key_t::Up, key_t::Right, key_t::W, key_t::D)) { delta += 1.0f; }
	out_clock += scale * delta * dt.count();
}
} // namespace

int main() {
	misc::context_t ctx("World Clock");
	ctx.setVerticalSyncEnabled(true);
	world_clock_t clock;
	clock.add("PDT", 0xff8800ff, -7.0f);
	clock.add("IST", 0x44ddffff, 5.5f);
	clock.add("CET", 0x11ee44ff, 2.0f);
	std::cout << clock << '\n';
	input_t input;
	delta_time_t dt;
	while (ctx.running()) {
		input.update(ctx.poll());
		tick(clock, input, ++dt);
		if (auto drawer = ctx.drawer()) { world_clock_drawer_t{}(drawer, clock, {}); }
	}
	std::cout << "\nCompleted successfully\n";
}
