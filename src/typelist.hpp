#pragma once
#include <type_traits>

namespace misc {
template <typename... T>
struct typelist;

template <typename T, typename... Ts>
struct typelist<T, Ts...> {
	using head_t = T;
};

template <typename Typelist>
struct typelist_pop;
template <typename Typelist>
using typelist_pop_t = typename typelist_pop<Typelist>::type;

template <typename T, typename... Ts>
struct typelist_pop<typelist<T, Ts...>> {
	using type = typelist<Ts...>;
};

template <typename Typelist>
constexpr bool is_empty_list_v = std::is_same_v<Typelist, typelist<>>;

template <typename T, typename Typelist>
constexpr bool is_head_v = std::is_same_v<T, typename Typelist::head_t>;

template <typename Typelist, typename T>
constexpr bool contains_type() noexcept {
	if constexpr (is_empty_list_v<Typelist>) {
		return false;
	} else if constexpr (is_head_v<T, Typelist>) {
		return true;
	} else {
		return contains_type<typelist_pop_t<Typelist>, T>();
	}
}
} // namespace misc
