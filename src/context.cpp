#include <cassert>
#include <context.hpp>
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

drawer_t::drawer_t(context_t* ctx, sf::Color clear_colour) : ctx(ctx) { ctx->clear(clear_colour); }
drawer_t::~drawer_t() { ctx->display(); }
} // namespace misc
