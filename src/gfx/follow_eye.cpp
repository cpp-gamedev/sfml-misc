#include <context.hpp>
#include <gfx/follow_eye.hpp>
#include <gfx/shape.hpp>
#include <maths/maths.hpp>

namespace misc {
sf::Vector2f follow_eye_t::update(sf::Vector2f target) noexcept { return (dir = maths::normalize(target - pos)); }
void follow_eye_t::draw(drawer_t& drawer, bool blink) {
	shape_t<sf::CircleShape> circle(drawer);
	circle.radius = 40.0f * scale;
	circle.outline = {sf::Color(0x221100ff), 2.0f};
	circle.position = pos;
	circle.draw();
	if (!blink) {
		auto const radius = circle.radius * 0.25f;
		auto const offset = dir * (circle.radius - radius) * 0.80f; // 20% padding
		circle.radius = radius;
		circle.fill = sf::Color::Black;
		circle.outline.size = 0.0f;
		circle.position += offset;
		circle.draw();
	}
}
} // namespace misc
