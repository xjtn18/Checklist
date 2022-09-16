#include <Input.hpp>
#include <Program.hpp>


void handle_global_input(sf::Event& event, Program& p){
	// User clicked the windows close button
	if (event.type == sf::Event::Closed){
		p.stop();
		return;

	} else if (event.type == sf::Event::KeyPressed){
		switch (event.key.code){

		case sf::Keyboard::Escape: // back out / end program
			p.stop();
			break;
		}
	}
}


