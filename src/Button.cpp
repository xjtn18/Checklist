#include <Button.hpp>


int Button::size = 25;


Button::Button(vb::Transform tf)
	: inner_circle(new BorderedCircle(tf, Button::size-6, 0)),
		outer_circle(new BorderedCircle(tf, Button::size, 2)),
		activated(false)
{
	set_color(COLOR_GREEN);

	//inner_circle->set_color(COLOR_GREEN);
	//outer_circle->set_color(COLOR_GREEN);
}


bool Button::is_activated(){
	return activated;
}


void Button::press(){
	activated = !activated;
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
}



void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (activated) inner_circle->draw(target, states);
	outer_circle->draw(target, states);
}

