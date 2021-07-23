#pragma once
#include <context.hpp>
#include <gfx/factory.hpp>

namespace misc {
template <typename T>
	requires is_valid_shape_v<T>
class shape_t : public shape_params_t<T>, public transform_t {
  public:
	using type = T;

	shape_t(drawer_t& drawer) : m_drawer(drawer) {}

	T const& shape() const noexcept { return m_t; }
	T& shape() noexcept { return m_t; }

	void draw() {
		setup_shape(m_t, *this, *this);
		m_drawer.draw(m_t);
	}

  private:
	drawer_t& m_drawer;
	T m_t;
};
} // namespace misc
