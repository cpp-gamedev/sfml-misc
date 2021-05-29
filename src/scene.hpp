#pragma once
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <SFML/Graphics/Drawable.hpp>

namespace misc {
class scene_t {
  public:
	// Associate a drawable with a string
	// Destroy an associated drawable
	// Check if a drawable is associated

	template <typename T, typename... Args>
	T& spawn(std::string id, Args&&... args) {
		static_assert(std::is_base_of_v<sf::Drawable, T>, "T must derive from sf::Drawable");
		auto t = std::make_unique<T>(std::forward<Args>(args)...);
		auto& ret = *t;
		m_map[std::move(id)] = std::move(t);
		return ret;
	}

	sf::Drawable* find(std::string const& id) const {
		if (auto it = m_map.find(id); it != m_map.end()) { return it->second.get(); }
		return nullptr;
	}

	template <typename T>
	T* find(std::string const& id) const {
		if (auto drawable = find(id)) { return dynamic_cast<T*>(drawable); }
		return nullptr;
	}

	void unspawn(std::string const& id) { m_map.erase(id); }

  private:
	std::unordered_map<std::string, std::unique_ptr<sf::Drawable>> m_map;

	friend class context;
};
} // namespace misc
