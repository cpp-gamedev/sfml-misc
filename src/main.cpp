#include <cassert>
#include <iostream>
#include <context.hpp>
#include <scene.hpp>

/*
- poll
- update / tick (based on dt and new events)
- render / draw
*/

using namespace misc;

int main() {
	misc::context ctx("SFML Misc");
	misc::scene_t scene;
	scene.spawn<sf::CircleShape>("circle", 50.0f, 30);
	ctx.setVerticalSyncEnabled(true);
	// sf::CircleShape circle(50.0f, 30);
	auto p_circle = scene.find<sf::CircleShape>("circle");
	assert(p_circle != nullptr);
	auto& circle = *p_circle;
	circle.setFillColor(sf::Color::Yellow);
	circle.setOutlineColor(sf::Color::Blue);
	circle.setOutlineThickness(3.0f);
	circle.setOrigin(50.0f, 50.0f);
	while (ctx.running()) {
		auto const poll = ctx.poll();
		// tick
		ctx.render(scene, sf::Color(30, 30, 30, 255));
		// draw stuff
	}
	std::cout << "\nCompleted successfully\n";
}
