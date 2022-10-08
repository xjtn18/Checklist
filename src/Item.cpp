#include <Item.hpp>



Item::Item(vb::Transform _tf, int _padding, const std::string &_message, bool item_checked)
	: HStack(_tf, _padding, {})
{
	button = new Button(tf, item_checked);
	button->set_color(COLOR_BUTTON);
	if (item_checked) button->set_alpha(ALPHA_DIM);
	content = new Text(_message);

	(item_checked) ? content->set_color(COLOR_DIM) : content->set_color(COLOR_WHITE);

	insert(-1, button);
	insert(-1, content);
}



void Item::check(){
	if (is_checked()){
		button->animate_color_to(COLOR_BUTTON, true);
		content->animate_color_to(COLOR_WHITE);
	} else {
		button->animate_color_to(COLOR_DIM, true);
		content->animate_color_to(COLOR_DIM);
	}

	button->press();
}


bool Item::is_checked(){
	return button->is_set();
}



void Item::update(float dt){
	HStack::update(dt);
}



void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	HStack::draw(target, states);
}

