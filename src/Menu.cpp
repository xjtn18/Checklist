#include <Menu.hpp>
#include <Program.hpp>
#include <Shapes.hpp>
#include <Button.hpp>
#include <Text.hpp>

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <unordered_set>
#include <fstream>



Menu::Menu(vb::Transform _tf, int _padding)
	: VStack(_tf, _padding, {})
{
	sf::Color scol = COLOR_ORANGE;
	scol.a = 20;
	select_rect.setFillColor(scol);
	VStack::update(0.0f);
}


void Menu::add_item(const std::string message, bool item_set){
	//insert(-1, new HStack(tf, 10, std::vector<Entity*>{new Button(tf, item_set), text}));
	insert(-1, new Item(tf, 10, message, item_set));
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


sf::Vector2f Menu::get_list_index_position(const int index){
	return sf::Vector2f(tf.x, tf.y + entities[index]->tf.h * index + (padding * index));
}


Item& Menu::item(int index){
	return *((Item*)entities[index]);
}


void Menu::toggle_item(const int &index){
	item(index).check();
}


bool Menu::item_is_checked(const int &index){
	//HStack* it = (HStack*)entities[index];
   //return ((Button*)it->entities[0])->is_set();
	return item(index).is_checked();
}


int Menu::get_index_to_first_checked_item(){
	for (int i = 0; i < size; ++i){
		if (item(i).is_checked()){ // 0, because the button the first element in the item hstack.
			return i;
		}
	}
	return size; // no checked item was found; return the last index.
}


int Menu::get_index_to_last_unchecked_item(){
	return get_index_to_first_checked_item()-1;
}



bool Menu::is_item_currently_animating(const int index){
	//if (index < 0 || index >= size) return false;
	return entities[index]->pos_anim.is_alive();
}




void Menu::check_item(){
	int delay = 0;
	int delay_seperation = 3;
	bool last_item_was_static = false;

	int last_unchecked_index = get_index_to_last_unchecked_item();
	if (select_index != last_unchecked_index){
		for (int i = select_index; i <= last_unchecked_index; ++i){
			sf::Vector2f destination = get_list_index_position((i == select_index) ? last_unchecked_index : i-1);
			int anim_delay = 0;

			last_item_was_static = !is_item_currently_animating(i) || last_item_was_static;
			delay += delay_seperation;
			if (last_item_was_static){
				anim_delay = delay;
			}

			entities[i]->animate_position_to(destination, anim_delay);
		}
	}

	toggle_item(select_index);

	auto it = entities.begin() + select_index;
	std::rotate(it, it+1, entities.begin() + last_unchecked_index + 1);
}



void Menu::uncheck_item(){
	int delay = 0;
	int delay_seperation = 3;
	bool last_item_was_static = false;

	int first_checked_index = get_index_to_first_checked_item();
	if (select_index != first_checked_index){
		for (int i = select_index; i >= first_checked_index; --i){
			sf::Vector2f destination = get_list_index_position((i == select_index) ? first_checked_index : i+1);
			int anim_delay = 0;

			last_item_was_static = !is_item_currently_animating(i) || last_item_was_static;
			delay += delay_seperation;
			if (last_item_was_static){
				anim_delay = delay;
			}

			entities[i]->animate_position_to(destination, anim_delay);
		}
	}

	toggle_item(select_index);

	auto it = entities.begin() + select_index;
	std::rotate(entities.begin() + first_checked_index, it, it + 1);
}




void Menu::uncheck_all(){
	for (int i = 0; i < size; ++i){
		if (item_is_checked(i)) toggle_item(i);
	}
}




std::string Menu::serialize(){
	std::string serial;

	for (int i = 0; i < size; ++i){
		Item it = item(i);
		serial += (it.is_checked()) ? "1" : "0";
		serial += ".";
		serial += it.content->txt.getString();
		serial += "\n";
	}

	return serial;
}


void Menu::save(){
	std::ofstream file("lists/" + name + ".list");
	file << serialize();
	file.close();
}


void Menu::load(std::string list_name){
	name = list_name; // set the checklist name

	std::ifstream file("lists/" + list_name + ".list");
	std::string line, token;

	while (std::getline(file, line)){

		std::istringstream ss(line);

		std::getline(ss, token, '.');
		std::string state_str = token;
		bool state = (state_str == "1") ? true : false;

		std::getline(ss, token, '\n');
		std::string message = token;

		add_item(message, state);
	}
}




void Menu::update(float dt){
	select_rect.setPosition(entities[select_index]->get_position());
	select_rect.setSize(sf::Vector2f(entities[select_index]->tf.w, entities[select_index]->tf.h));

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

			if (item_is_checked(select_index)){ // UNCHECKING AN ITEM
				uncheck_item();

			} else { // CHECKING AN ITEM
				check_item();
			}

			elements_currently_static = false;


			return true;


		case sf::Keyboard::F:
			uncheck_all();
			return true;

		}

	}

	return false;
}

