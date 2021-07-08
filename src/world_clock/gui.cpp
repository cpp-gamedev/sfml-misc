#include <cassert>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <context.hpp>
#include <world_clock/gui.hpp>

namespace misc {
namespace {
using in_t = world_clock_drawer_t::in_t;

constexpr f32 max_rotation = 360.0f;

f32 rotation(hour_t hour) noexcept { return max_rotation * hour / total_hours; }

sf::Vector2f to_screen(sf::Vector2f world) noexcept { return {world.x, -world.y}; }

void init(sf::Transformable& out, sf::Vector2f origin, sf::Vector2f position = {}, f32 rotation = {}) {
	out.setOrigin(origin);
	out.setPosition(to_screen(position));
	out.setRotation(rotation);
}

void init(sf::Sprite& out, sf::Texture const& texture, sf::Vector2f scale, sf::Color colour) {
	out.setTexture(texture);
	out.setScale(scale);
	out.setColor(colour);
}

void init(sf::RectangleShape& out_hand, u32 colour, world_hour_t hour, in_t const& in) {
	auto const size = sf::Vector2f(in.hand_height * 0.08f, in.hand_height) * in.scale;
	sf::Vector2f const origin = {size.x * in.hand_pivot.x, size.y * in.hand_pivot.y};
	out_hand.setFillColor(sf::Color(colour));
	out_hand.setSize(size);
	init(out_hand, origin, in.centre, rotation(hour.hour()));
}

void init(sf::Sprite& out_hand, u32 colour, world_hour_t hour, in_t const& in) {
	auto const size = in.hand_tex->getSize();
	sf::Vector2f const origin = {f32(size.x) * in.hand_pivot.x, f32(size.y) * in.hand_pivot.y};
	sf::Vector2f const scale = {in.hand_height / f32(size.y), in.hand_height / f32(size.y)};
	init(out_hand, origin, in.centre, rotation(hour.hour()));
	init(out_hand, *in.hand_tex, scale * in.scale, sf::Color(colour));
}

void init(sf::CircleShape& out_face, f32 radius, in_t const& in) {
	out_face.setFillColor(in.face_tint);
	out_face.setRadius(radius);
	out_face.setPointCount(50);
	init(out_face, {radius, radius}, in.centre);
}

void init(sf::Sprite& out_face, in_t const& in) {
	auto const origin = in.face_tex->getSize() / 2U;
	f32 const radius = in.hand_height * (1.0f + in.face_pad);
	sf::Vector2f const scale = {radius / f32(origin.x), radius / f32(origin.y)};
	init(out_face, {f32(origin.x), f32(origin.y)}, in.centre);
	init(out_face, *in.face_tex, scale * in.scale, sf::Color::White);
}

void init(sf::CircleShape& out_marker, world_hour_t hour, f32 clock_radius, in_t const& in) {
	f32 const radians = rotation(hour.hour()) * 3.14f / 180.0f;
	sf::Vector2f const direction = {std::sin(radians), std::cos(radians)};
	sf::Vector2f const offset = direction * clock_radius * 0.9f;
	f32 const marker_radius = in.marker_radius * in.scale;
	out_marker.setFillColor(in.marker_tint);
	out_marker.setRadius(marker_radius);
	init(out_marker, {marker_radius, marker_radius}, in.centre + offset, rotation(hour.hour()));
}

struct facade_t {
	drawer_t& drawer;
	in_t const& in;

	template <typename T, typename... Args>
	void draw(Args&&... args) const {
		T t;
		init(t, std::forward<Args>(args)..., in);
		drawer.draw(t);
	}

	void draw_clock() const {
		if (in.face_tex) {
			draw<sf::Sprite>();
		} else {
			f32 const radius = in.hand_height * (1.0f + in.face_pad);
			draw<sf::CircleShape>(radius);
			static constexpr int count = 8;
			for (int i = 0; i < count; ++i) {
				hour_t const hour = total_hours * f32(i) / f32(count);
				draw<sf::CircleShape>(world_hour_t(hour), radius);
			}
		}
	}

	void draw_hand(world_clock_t::entry_t const& entry) const {
		if (in.hand_tex) {
			draw<sf::Sprite>(entry.colour, entry.hour);
		} else {
			draw<sf::RectangleShape>(entry.colour, entry.hour);
		}
	}
};
} // namespace

void world_clock_drawer_t::operator()(drawer_t& drawer, world_clock_t const& clock, in_t const& in) {
	facade_t f{drawer, in};
	f.draw_clock();
	for (auto const& entry : clock) { f.draw_hand(entry); }
}
} // namespace misc
