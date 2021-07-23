#pragma once
#include <SFML/System/Vector2.hpp>
#include <types.hpp>

namespace misc {
struct drawer_t;

struct follow_eye_t {
	sf::Vector2f dir = {0.0f, 1.0f};
	sf::Vector2f pos = {0.0f, 0.0f};
	f32 scale = 1.0f;

	sf::Vector2f update(sf::Vector2f target) noexcept;
	void draw(drawer_t& drawer, bool blink);
};
} // namespace misc
