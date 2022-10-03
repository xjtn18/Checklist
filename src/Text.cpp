#include <Text.hpp>


int Text::DIM_ALPHA = 40;



Text::Text(std::string _content, vb::Transform _tf, int _fontsize, sf::Font& _font)
	: Entity(_tf)
{
	txt = sf::Text(_content, _font, _fontsize);
	sf::FloatRect bounds = txt.getLocalBounds();

	max_bounds = sf::Text(")", _font, _fontsize).getLocalBounds();

	tf.w = bounds.width + bounds.left;
	tf.h = max_bounds.height;

	set_color(COLOR_WHITE);
	//center_yaxis();
}


void Text::set_text(const std::string& new_text){
	txt.setString(new_text);
}

void Text::center_xaxis(){
	sf::FloatRect bounds = txt.getLocalBounds();
	txt.setOrigin((int)(bounds.width/2 + bounds.left), (int) max_bounds.top);
}

void Text::center_yaxis(){
	//sf::FloatRect bounds = txt.getLocalBounds();
	txt.setOrigin(0, (int) max_bounds.height/2 + max_bounds.top - 1);
}

void Text::dimmer(){
	// Toggles text between dimmed & opaque.
	sf::Color color = txt.getFillColor();
	color.a = (color.a == Text::DIM_ALPHA) ? 255 : Text::DIM_ALPHA;
	set_color(color);
}

void Text::update(float dt){
	txt.setPosition(tf.x, tf.y);
	txt.setFillColor(color);
}


void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(txt);
}

