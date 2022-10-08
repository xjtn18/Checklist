#include <Animation.hpp>
#include <Entity.hpp>



// Color Animation

ColorAnimation::ColorAnimation(Entity *ent, sf::Color b, bool ao)
	: Animation<sf::Color>(ent->color, b),
		entity(ent),
		alpha_only(ao)
{
	setup();
}



void ColorAnimation::update(float y) {
	entity->set_color(get_intermediate_color(home, end, y, alpha_only));
}






// Position Animation

PositionAnimation::PositionAnimation(Entity *ent, sf::Vector2f b)
	: Animation<sf::Vector2f>(ent->get_position(), b),
		entity(ent)
{
	setup();
}



void PositionAnimation::update(float y) {
	entity->set_position(get_intermediate_position(home, end, y));
}

