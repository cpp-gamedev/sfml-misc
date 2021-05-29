#pragma once
#include <string_view>
#include <vector>
#include <SFML/Graphics.hpp>
#include <types.hpp>

namespace misc {
class scene_t;

class context : public sf::RenderWindow {
  public:
	static constexpr s32 width = 1280;
	static constexpr s32 height = 720;
	static constexpr sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;

	using sf::RenderWindow::draw;

	struct drawer {
		drawer(context* ctx, sf::Color clear_colour);
		~drawer();

	  private:
		context* ctx;
	};

	context() = default;
	context(std::string_view title);

	bool running() const;
	std::vector<sf::Event> poll();

	void render(scene_t const& scene, sf::Color clear_colour = sf::Color::Black);

	drawer draw(sf::Color clear_colour = sf::Color::Black) { return drawer(this, clear_colour); }

  private:
	bool m_closed = false;
};
} // namespace misc
