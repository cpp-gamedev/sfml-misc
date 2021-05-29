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

context::context(std::string_view title)
	: sf::RenderWindow(sf::VideoMode(width, height), title.data(), style, context_settings()) {
	auto view = getView();
	view.setCenter(0.0f, 0.0f);
	setView(view);
}

bool context::running() const { return isOpen() && !m_closed; }

std::vector<sf::Event> context::poll() {
	sf::Event event;
	std::vector<sf::Event> ret;
	while (pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed: m_closed = true; break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) { m_closed = true; };
			break;
		default: ret.push_back(event); break;
		}
	}
	return ret;
}

context::drawer::drawer(context* ctx, sf::Color clear_colour) : ctx(ctx) {
	assert(ctx != nullptr);
	ctx->clear(clear_colour);
}

context::drawer::~drawer() { ctx->display(); }

void context::render(scene_t const& scene, sf::Color clear_colour) {
	clear(clear_colour);
	for (auto const& [_, drawable] : scene.m_map) {
		this->sf::RenderWindow::draw(*drawable);
	}
	display();
}
} // namespace misc
