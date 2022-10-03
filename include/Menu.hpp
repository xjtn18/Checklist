#pragma once
#include <Entity.hpp>
#include <Stack.hpp>
#include <Animation.hpp>


struct Menu : public VStack {
	//
	// A vertical arrangment of buttons.
	//
	bool editing = false;
	int select_index = 0;

	sf::RectangleShape select_rect;
	std::vector<PositionAnimation> animations;
	std::vector<ColorAnimation> color_animations;


	Menu(){}
	Menu(vb::Transform, int);
	void engage(bool);
	void move_selector(vb::Direc);
	void set_selector(int);
	void add(Program &p);
	void trigger(std::vector<int>);
	void set_all_idle();
	void select_move(vb::Direc);
	void set_select(const int);
	void add_item(std::string);
	sf::Vector2f get_position_of_item(const int);
	sf::Vector2f get_position_of_entity(const int);
	void toggle_item(const int&);
	void check_item();
	void uncheck_item();
	bool item_is_checked(const int&);
	int get_index_to_first_checked_item();
	int get_index_to_last_unchecked_item();
	void process_animations(float);
	void delete_stale_animations();
	void uncheck_all();
	bool is_item_currently_animating(const int);

	void update(float);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool handler(sf::Event& event, Program &p);
};




