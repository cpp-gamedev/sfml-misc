#include <gfx/factory.hpp>
#include <maths/maths.hpp>

using namespace misc::maths;

void misc::transform(sf::Transformable& out, sf::Vector2f bounds, transform_t const& tr) {
	out.setOrigin(tr.n_pivot * bounds);
	out.setPosition(screen(tr.position));
	out.setRotation(tr.orientation.deg());
	out.setScale(tr.scale);
}

template <>
void misc::setup_shape(sf::CircleShape& shape, shape_params_t<sf::CircleShape> const& params, transform_t const& tr) {
	shape.setFillColor(params.fill);
	shape.setOutlineColor(params.outline.colour);
	shape.setOutlineThickness(params.outline.size);
	shape.setPointCount(params.points);
	shape.setRadius(params.radius);
	transform(shape, {params.radius * 2, params.radius * 2}, tr);
}

template <>
void misc::setup_shape(sf::RectangleShape& shape, shape_params_t<sf::RectangleShape> const& params, transform_t const& tr) {
	shape.setFillColor(params.fill);
	shape.setOutlineColor(params.outline.colour);
	shape.setOutlineThickness(params.outline.size);
	shape.setSize(params.size);
	transform(shape, params.size, tr);
}
