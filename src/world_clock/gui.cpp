#include <cassert>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <context.hpp>
#include <gfx/follow_eye.hpp>
#include <gfx/shape.hpp>
#include <world_clock/gui.hpp>

namespace misc {
namespace {
using in_t = world_clock_drawer_t::in_t;

constexpr f32 max_rotation = 360.0f;

maths::rad_t rotation(hour_t hour) noexcept { return maths::rad(max_rotation * hour / total_hours); }

void init_hand(shape_t<sf::RectangleShape>& out, u32 colour, world_hour_t hour, in_t const& in) {
	out.fill = sf::Color(colour);
	out.size = sf::Vector2f(in.hand_height * 0.08f, in.hand_height) * in.scale;
	out.position = in.centre;
	out.orientation = rotation(hour.hour());
	out.n_pivot = in.hand_pivot;
}

void init_marker(shape_t<sf::CircleShape>& out, world_hour_t hour, f32 clock_radius, in_t const& in) {
	sf::Vector2f const offset = maths::dir(rotation(hour.hour())) * clock_radius * 0.9f; // 10% padding
	out.fill = in.marker_tint;
	out.radius = in.marker_radius * in.scale;
	out.position = in.centre + offset;
}

struct facade_t {
	drawer_t& drawer;
	in_t const& in;

	void draw_eyes(sf::Vector2f target, bool blink) const {
		follow_eye_t left;
		left.pos = {-80.0f, 50.0f};
		left.pos *= in.scale;
		left.update(target);
		left.draw(drawer, blink);
		follow_eye_t right;
		right.pos = {-left.pos.x, left.pos.y};
		right.update(target);
		right.draw(drawer, blink);
	}

	void draw_clock() const {
		f32 const radius = in.hand_height * (1.0f + in.face_pad);
		shape_t<sf::CircleShape> face(drawer);
		face.fill = in.face_tint;
		face.radius = radius;
		face.position = in.centre;
		face.draw();
		static constexpr int count = 8;
		for (int i = 0; i < count; ++i) {
			hour_t const hour = total_hours * f32(i) / f32(count);
			shape_t<sf::CircleShape> marker(drawer);
			init_marker(marker, world_hour_t(hour), radius, in);
			marker.draw();
		}
	}

	void draw_hand(world_clock_t::entry_t const& entry) const {
		shape_t<sf::RectangleShape> hand(drawer);
		init_hand(hand, entry.colour, entry.hour, in);
		hand.draw();
	}
};
} // namespace

void world_clock_drawer_t::operator()(drawer_t& drawer, world_clock_t const& clock, in_t const& in) {
	facade_t f{drawer, in};
	f.draw_clock();
	f.draw_eyes(in.mouse_pos, in.blink);
	for (auto const& entry : clock) { f.draw_hand(entry); }
}
} // namespace misc
