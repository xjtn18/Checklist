#pragma once
#include <SFML/System.hpp>
#include <functional>
#include <cmath>

#include <vb.hpp>
#include <Animation.hpp>
#include <Entity.hpp>
#include <Menu.hpp>




struct Program {
	bool running = true;

	// window
	sf::RenderWindow *window_ptr;


	// UI entities
	Menu *checklist = nullptr;
	Entity *engaged = nullptr;
	Entity *last_engaged = nullptr;


	// program state
	std::vector<Entity*> draw_list;


	// sectors
	int padding;


	Program();
	void stop();
	void quit();
	void cleanup();
	void set_pane_rack_chooser();
	void set_pane_main();
	void engage_with(Entity*);
	void update_frame(float);
	void draw_frame(sf::RenderWindow&);
	void mainloop();

};


