#pragma once

#include <Stack.hpp>
#include <Button.hpp>
#include <Text.hpp>


struct Item : public HStack {
	Button *button;
	Text *content;

	Item(vb::Transform, int, const std::string &, bool);
	void check();
	bool is_checked();

	void update(float);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


