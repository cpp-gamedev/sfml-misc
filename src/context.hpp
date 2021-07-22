#pragma once
#include <string_view>
#include <vector>
#include <SFML/Graphics.hpp>
#include <types.hpp>

namespace misc {
struct drawer_t;

class context_t : public sf::RenderWindow {
  public:
	static constexpr s32 width = 1280;
	static constexpr s32 height = 720;
	static constexpr sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;

	using sf::RenderWindow::draw;

	context_t() = default;
	context_t(std::string_view title);

	bool running() const;
	std::vector<sf::Event> poll();

	drawer_t drawer(sf::Color clear_colour = sf::Color::Black);

	sf::Vector2f mouse_pos() const noexcept;

  private:
	bool m_closed = false;
};

struct drawer_t {
	drawer_t(context_t* ctx, sf::Color clear_colour);
	~drawer_t();

	explicit operator bool() const noexcept { return ctx != nullptr; }
	template <typename... Args>
	void draw(Args&&... args) const;

  private:
	context_t* ctx;
};

// impl

inline drawer_t context_t::drawer(sf::Color clear_colour) { return drawer_t(this, clear_colour); }
template <typename... Args>
void drawer_t::draw(Args&&... args) const {
	ctx->draw(std::forward<Args>(args)...);
}
} // namespace misc
