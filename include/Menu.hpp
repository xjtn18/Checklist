#pragma once
#include <Entity.hpp>
#include <Stack.hpp>
#include <Animation.hpp>
#include <Item.hpp>


struct Menu : public VStack {
	//
	// A vertical arrangment of buttons.
	//
	bool editing = false;
	int select_index = 0;
	std::string name;

	sf::RectangleShape select_rect;


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
	void add_item(std::string, bool = false);
	sf::Vector2f get_list_index_position(const int);
	void toggle_item(const int&);
	void check_item();
	void uncheck_item();
	Item& item(int);
	bool item_is_checked(const int&);
	int get_index_to_first_checked_item();
	int get_index_to_last_unchecked_item();
	void uncheck_all();
	bool is_item_currently_animating(const int);

	std::string serialize();
	void save();
	void load(std::string);

	void update(float);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool handler(sf::Event& event, Program &p);
};




