#include <Button.hpp>


int Button::size = 25;


Button::Button(vb::Transform tf, bool s)
	: inner_circle(new BorderedCircle(tf, Button::size-6, 0)),
		outer_circle(new BorderedCircle(tf, Button::size, 2)),
		set(s)
{
}


bool Button::is_set(){
	return set;
}


void Button::press(){
	set = !set;
}



void Button::update(float dt){
	inner_circle->circle.setFillColor(color);
	inner_circle->tf = tf;
	inner_circle->tf.x += 4;
	inner_circle->tf.y += 4;
	inner_circle->update(dt);


	outer_circle->circle.setOutlineColor(color);
	outer_circle->tf = tf;
	outer_circle->update(dt);

	tf.w = Button::size*2-2*2;
	tf.h = Button::size*2-2*2;

	Entity::update(dt); // step through animations
}



void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (set) inner_circle->draw(target, states);
	outer_circle->draw(target, states);
}

