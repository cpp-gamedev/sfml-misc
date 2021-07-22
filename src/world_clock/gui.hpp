#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <world_clock/world_clock.hpp>

namespace sf {
class Texture;
}

namespace misc {
struct drawer_t;

struct world_clock_drawer_t {
	struct in_t {
		sf::Vector2f centre{};
		sf::Vector2f hand_pivot = {0.5f, 0.9f};
		sf::Vector2f mouse_pos{};
		sf::Color face_tint = sf::Color(0xccccbbff);
		sf::Color marker_tint = sf::Color(0x555555ff);
		sf::Texture const* face_tex{};
		sf::Texture const* hand_tex{};
		f32 hand_height = 250.0f;
		f32 marker_radius = 10.0f;
		f32 face_pad = 0.2f;
		f32 scale = 1.0f;
		bool blink = false;
	};

	void operator()(drawer_t& drawer, world_clock_t const& clock, in_t const& in);
};
} // namespace misc
