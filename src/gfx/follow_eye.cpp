#include <context.hpp>
#include <gfx/follow_eye.hpp>
#include <maths/maths.hpp>

namespace misc {
namespace {
struct circle_params_t {
	sf::Vector2f position{};
	sf::Color fill = sf::Color::White;
	sf::Color outline = sf::Color::Transparent;
	f32 radius = 25.0f;
	u32 points = 64;
};

void draw_circle(drawer_t& drawer, circle_params_t const& params) {
	sf::CircleShape circle;
	circle.setFillColor(params.fill);
	circle.setOutlineColor(params.outline);
	circle.setPointCount(params.points);
	circle.setRadius(params.radius);
	circle.setOrigin(params.radius, params.radius);
	circle.setPosition(params.position.x, -params.position.y);
	drawer.draw(circle);
}
} // namespace

sf::Vector2f follow_eye_t::update(sf::Vector2f target) noexcept { return (dir = maths::norm(target - pos)); }
void follow_eye_t::draw(drawer_t& drawer, bool blink) {
	circle_params_t params;
	params.radius = 40.0f * scale;
	params.outline = sf::Color(0x221100ff);
	params.position = pos;
	draw_circle(drawer, params);
	if (!blink) {
		auto const radius = params.radius * 0.25f;
		auto const offset = dir * (params.radius - radius) * 0.80f; // 20% padding
		params.radius *= 0.25f;
		params.outline = sf::Color::Transparent;
		params.fill = sf::Color::Black;
		params.position += offset;
		draw_circle(drawer, params);
	}
}
} // namespace misc
