#include <Entity.hpp>

struct Text : public Entity {
	sf::Text txt;
	sf::FloatRect max_bounds;

	Text(std::string content,
		vb::Transform _tf = vb::Transform::Zero,
		int _fontsize = 55,
		sf::Font& _font = FONT_LIBMONO);

	void set_text(const std::string& new_text);
	void set_color(sf::Color c);
	void center_xaxis();
	void center_yaxis();

	void update(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

