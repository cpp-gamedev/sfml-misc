#pragma once
#include <SFML/Graphics.hpp>
#include <gfx/orient.hpp>
#include <typelist.hpp>

namespace misc {
// world space: centred origin, +x right, +y up
struct transform_t {
	sf::Vector2f position{};
	orient_t orientation;
	sf::Vector2f scale = {1.0f, 1.0f};
	maths::npt_t n_pivot = {0.5f, 0.5f};
};

struct common_params_t {
	sf::Color fill = sf::Color::White;
	struct {
		sf::Color colour = sf::Color::Transparent;
		f32 size = 0.0f;
	} outline;
};

using valid_shapes_t = typelist<sf::RectangleShape, sf::CircleShape>;
template <typename T>
constexpr bool is_valid_shape_v = contains_type<valid_shapes_t, T>();

template <typename T>
struct shape_params_t;

template <>
struct shape_params_t<sf::RectangleShape> : common_params_t {
	sf::Vector2f size = {20.0f, 20.0f};
};

template <>
struct shape_params_t<sf::CircleShape> : common_params_t {
	u32 points = 64;
	f32 radius = 10.0f;
};

// screen space: +x right, +y down
inline sf::Vector2f screen(sf::Vector2f world, sf::Vector2f offset = {}) noexcept { return offset + sf::Vector2f{world.x, -world.y}; }

void transform(sf::Transformable& out, sf::Vector2f bounds, transform_t const& tr = {});

template <typename T>
void setup_shape(T& shape, shape_params_t<T> const& params, transform_t const& tr = {});

// impl

template <>
void setup_shape(sf::CircleShape& shape, shape_params_t<sf::CircleShape> const& params, transform_t const& tr);
template <>
void setup_shape(sf::RectangleShape& shape, shape_params_t<sf::RectangleShape> const& params, transform_t const& tr);
} // namespace misc
