#pragma once

#include <Stack.hpp>
#include <Button.hpp>
#include <Text.hpp>


struct Item : public HStack {
	bool is_checked;
	Button *button;
	Text *message;

	Item(vb::Transform, const std::string&, int);
	void check();

	void update(float);
};


