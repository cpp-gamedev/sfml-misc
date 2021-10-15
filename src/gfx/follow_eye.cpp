#include <context.hpp>
#include <gfx/follow_eye.hpp>
#include <gfx/shape.hpp>
#include <maths/maths.hpp>

namespace misc {
sf::Vector2f follow_eye_t::update(sf::Vector2f target) noexcept { return (dir = maths::normalize(target - pos)); }
void follow_eye_t::draw(drawer_t& drawer, bool blink) {
	shape_t<sf::CircleShape> eye(drawer);
	eye.radius = 40.f * scale;
	eye.position = pos;
	if (!blink) {
		eye.outline = {sf::Color(0x221100ff), 2.0f};
		eye.draw();
		// pupil
		auto const offset = dir * (eye.radius * 0.75f) * 0.80f; // 20% padding
		eye.radius *= 0.3f;
		eye.fill = sf::Color::Black;
		eye.outline = {sf::Color(0x221100ff), 0.5f};
		eye.position += offset;
		eye.draw();
	} else {
		eye.scale = {1.0f, 0.1f};
		eye.fill = sf::Color::Black;
		eye.draw();
	}
}
} // namespace misc
