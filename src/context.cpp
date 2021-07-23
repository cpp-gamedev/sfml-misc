#include <cassert>
#include <context.hpp>
#include <maths/maths.hpp>
#include <scene.hpp>

namespace misc {
namespace {
sf::ContextSettings context_settings() {
	sf::ContextSettings ret;
	ret.antialiasingLevel = 8;
	return ret;
}
} // namespace

context_t::context_t(std::string_view title) : sf::RenderWindow(sf::VideoMode(width, height), title.data(), style, context_settings()) {
	auto view = getView();
	view.setCenter(0.0f, 0.0f);
	setView(view);
}

bool context_t::running() const { return isOpen() && !m_closed; }

std::vector<sf::Event> context_t::poll() {
	sf::Event event;
	std::vector<sf::Event> ret;
	while (pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed: m_closed = true; break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) { m_closed = true; };
			ret.push_back(event);
			break;
		default: ret.push_back(event); break;
		}
	}
	return ret;
}

sf::Vector2f context_t::mouse_pos() const noexcept {
	static auto const offset = sf::Vector2f(f32(width) * 0.5f, f32(height) * 0.5f);
	auto ret = maths::cast<sf::Vector2f>(sf::Mouse::getPosition(*this));
	ret -= offset;
	return {ret.x, -ret.y};
}

drawer_t::drawer_t(context_t* ctx, sf::Color clear_colour) : ctx(ctx) { ctx->clear(clear_colour); }
drawer_t::~drawer_t() { ctx->display(); }
} // namespace misc
