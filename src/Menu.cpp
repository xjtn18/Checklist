#include <Menu.hpp>
#include <Program.hpp>
#include <Text.hpp>
#include <Shapes.hpp>
#include <algorithm>
#include <sstream>
#include <iomanip>



Menu::Menu(vb::Transform _tf, int _padding)
	: VStack(_tf, _padding, {})
{
	select_rect.setFillColor(sf::Color(60,60,60));

	// Hard code the items in the checklist
	add_item("Jonathan");
	add_item("Blow");
	add_item("is");
	add_item("the");
	add_item("GOAT.");

	VStack::update(0.0f);
}


void Menu::add_item(const std::string message){
	insert(-1, new HStack(tf, 10, std::vector<Entity*>{new BorderedCircle(tf, 40, 2), new Text(message)}));
}


void Menu::engage(bool value){
	engaged = value;
}



void Menu::select_move(vb::Direc direction){
	select_index += direction;
	bool clamped = vb::clamp(select_index, 0, size);
}


void Menu::move_selector(vb::Direc dir){
	entities[select_index]->engage(false);
	select_move(dir);
	entities[select_index]->engage(true);
}


void Menu::set_select(const int new_index){
	select_index = new_index;
	vb::clamp(select_index, 0, size);
}


void Menu::set_selector(int index){
	entities[select_index]->engage(false);
	set_select(index);
	entities[select_index]->engage(true);
}


sf::Vector2f Menu::get_position_of_item(const int index){
	return sf::Vector2f(entities[index]->tf.x, entities[index]->tf.y);
}



void Menu::update(float dt){
	select_rect.setPosition(get_position_of_item(select_index));
	select_rect.setSize(sf::Vector2f(entities[select_index]->tf.w, entities[select_index]->tf.h));

	for (auto &anim : animations){
		anim.step(dt);
	}

   VStack::update(dt);

}


void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (engaged) target.draw(select_rect);
	VStack::draw(target, states);
}



bool Menu::handler(sf::Event& event, Program& p){
	if (event.type == sf::Event::KeyPressed){
		switch (event.key.code){

		case sf::Keyboard::J: // move rack selector down
			if (LSHIFT_IS_DOWN) set_selector(size-1);
			else move_selector(DOWN);
			return true;


		case sf::Keyboard::K: // move rack selector up
			if (LSHIFT_IS_DOWN) set_selector(0);
			else move_selector(UP);
			return true;


		case sf::Keyboard::Enter: // duplicate currently selected alarm
			if (!elements_currently_static){

				for (auto &anim : animations){
					anim.boomerang();
				}

				auto it = entities.begin() + select_index;
				if (animations[0].forw){ // use the first animation as a proxy for the rest
					// @NOTE: this only works when we know all the animations complete at the same time.
					std::rotate(it, it+1, entities.end());
				} else {
					std::rotate(it, --entities.end(), entities.end());
				}


			} else {
				animations.clear();
				for (int i = select_index; i < size; ++i){
					sf::Vector2f current_item_position = get_position_of_item(i);
					sf::Vector2f last_item_position = get_position_of_item((i == select_index) ? size-1 : i-1);
					PositionAnimation anim = PositionAnimation(entities[i], current_item_position, last_item_position);
					anim.rate = 1.5;
					anim.f 			= [](float x) { return pow(-pow(1/(x/2 + 0.5) - 1, 3) + 1, 3); }; // fluid transition
					anim.inversef 	= [](float x) { return 2/((cbrt(cbrt(-x) + 1)) + 1) - 1; }; // its inverse
					anim.start();
					animations.push_back(std::move(anim));
				}

				auto it = entities.begin() + select_index;
				std::rotate(it, it+1, entities.end());

				animations[0].lambda_complete = [&](){
					elements_currently_static = true;
				};

				elements_currently_static = false;
			}

			return true;
		}
	}
	return false;
}


