#pragma once

#include <Entity.hpp>
#include <Shapes.hpp>




struct Button : public Entity {
	BorderedCircle *inner_circle, *outer_circle;
	bool activated;
	static int size;

	Button(vb::Transform);
	bool is_activated();
	void press();

	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

