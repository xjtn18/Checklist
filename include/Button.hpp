#pragma once

#include <Entity.hpp>
#include <Shapes.hpp>




struct Button : public Entity {
	BorderedCircle *inner_circle, *outer_circle;
	bool set;
	static int size;

	Button(vb::Transform, bool = false);
	bool is_set();
	void press();

	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

