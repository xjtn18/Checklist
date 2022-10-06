#include <Item.hpp>



Item::Item(vb::Transform _tf, const std::string &_message, int _padding, bool item_set)
	: HStack(_tf, _padding, {}),
		is_checked(false)
{
	button = new Button(tf, item_set);

	Text *text = new Text(_message);
	(item_set) ? text->set_color(COLOR_DIM) : text->set_color(COLOR_WHITE);

	insert(-1, button);
	insert(-1, message);
}



void Item::check(){
	button->press();
}



void Item::update(float dt){
	dlog("hit");
	button->set_color(color);
	message->set_color(color);

	HStack::update(dt);
}



void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(*button);
}

