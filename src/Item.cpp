#include <Item.hpp>



Item::Item(vb::Transform _tf, const std::string &_message, int _padding)
	: HStack(_tf, _padding, {}),
		is_checked(false)
{
	button = new Button(tf);
	new Text(_message);

	insert(-1, button);
	insert(-1, message);
}


void Item::check(){
	button->press();
}
