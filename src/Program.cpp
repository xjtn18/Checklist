#include <Program.hpp>
#include <Entity.hpp>
#include <Text.hpp>
#include <Image.hpp>
#include <Speaker.hpp>
#include <Stack.hpp>
#include <Shapes.hpp>
#include <Input.hpp>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
using namespace vb;



Program::Program()
{
	// setup any static members
	aud::load_all(); // load all the program sounds
	load_fonts();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	window_ptr = new sf::RenderWindow(sf::VideoMode(WINW, WINH), "Auto Sorting Checklist", sf::Style::Titlebar | sf::Style::Close, settings);

	// Set the Icon
	sf::Image icon;
	icon.loadFromFile("res/images/clock-logo.png");
	window_ptr->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	window_ptr->setFramerateLimit(120);

	// Load all UI entities at program start that can be created now,
	// regardless of whether or not they appear in the initial pane (draw_list).
	
	sf::FloatRect bounds;

	// grid lines
	float grid_line_thickness = 2;
	auto yaxis = new LineShape({CENTER_WIN_X, CENTER_WIN_Y, grid_line_thickness, WINH});
	auto xaxis = new LineShape({CENTER_WIN_X, CENTER_WIN_Y, WINW, grid_line_thickness});

	checklist = new Menu(vb::Transform{25,25,0,0}, 10);

	set_pane_main();
}



void Program::set_pane_main(){
	draw_list = {
		checklist
	};

	engage_with(checklist);
}


void Program::engage_with(Entity *ent){
	if (engaged) engaged->engage(false);
	last_engaged = engaged;
	engaged = ent;
	engaged->engage(true);
}




void Program::cleanup(){
	// cleanup up audio resources
	aud::cleanup();

	delete window_ptr;
}


void Program::stop(){
	running = false;
}



void Program::quit(){
	//
	// Close the program window and clean up heap
	//
	window_ptr->close(); // close window
	cleanup();
}




void Program::update_frame(float dt){
	// update UI entities
	for (Entity *ent : draw_list){
		ent->update(dt);
	}
}


void Program::draw_frame(sf::RenderWindow& window){
	for (sf::Drawable *ent : draw_list){
		window.draw(*ent);
	}
}



void Program::mainloop(){
	//
	// Main program loop
	//

	sf::RenderWindow& window = *(window_ptr);
	sf::Clock clock;

	while (running && window.isOpen()) {
		window.clear(VBC_BG); // clear last frame and set bg color

		bool event_processed = false;
		sf::Event event;

		while (window.pollEvent(event)) {
			// @NOTE: A single character keystroke produces both a KeyPressed and a TextEntered event; be aware of that.
			// @NOTE: keystrokes shared by the current entity and the universals should prioritize the entity.
			// So entity handlers should be called first.
			
			// @FIXME: some events are processed by both the entity and the global handler.
			if (!event_processed){
				if (engaged) event_processed = engaged->handler(event, *this);
			}
			if (!event_processed){
				handle_global_input(event, *this);
			}

		}
		// All inputs were polled for this frame.

		update_frame( clock.restart().asSeconds() );
		draw_frame(window);

		window.display(); // display completed frame

	}
}


