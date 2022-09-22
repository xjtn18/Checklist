#include <Menu.hpp>
#include <Program.hpp>
#include <Item.hpp>
#include <Text.hpp>
#include <Shapes.hpp>
#include <Button.hpp>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <unordered_set>



Menu::Menu(vb::Transform _tf, int _padding)
	: VStack(_tf, _padding, {})
{
	select_rect.setFillColor(sf::Color(60,60,60));

	// Hard code the items in the checklist
	add_item("allow concurrent checks");
	add_item("integrate text boxs to edit items");
	add_item("change the color scheme");
	add_item("lerp the text opacity of checked/unchecked items");

	add_item("watch the minions: rise of gru (important)");
	add_item("Sawayama solitaire is much more tactical.");
	add_item("It's a great spin on klondike");
	add_item("Proletariats Patience is rewarding and fun");


	add_item(":) Master Chief");
	add_item("High Charity");
	add_item("Truth, Mercy, and Regret");
	add_item("Halo 3 has the best soundtrack");

	VStack::update(0.0f);

}


void Menu::add_item(const std::string message){
	//insert(-1, new Item(tf, message, 10));
	insert(-1, new HStack(tf, 10, std::vector<Entity*>{new Button(tf), new Text(message)}));
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
	return sf::Vector2f(tf.x, tf.y + entities[index]->tf.h * index + (padding * index));
}


sf::Vector2f Menu::get_position_of_entity(const int index){
	return sf::Vector2f(entities[index]->tf.x, entities[index]->tf.y);
}


void Menu::check_item(const int &index){
	//((Item*)entities[select_index])->check();
	HStack* item = (HStack*)entities[index];
	((Button*)item->entities[0])->press(); // 0, because the button the first element in the item hstack.
	((Text*)item->entities[1])->dimmer(); // 0, because the button the first element in the item hstack.
}

bool Menu::item_is_checked(const int &index){
	HStack* item = (HStack*)entities[index];
	return ((Button*)item->entities[0])->is_activated();
}


int Menu::get_index_to_first_checked_item(){
	for (int i = 0; i < size; ++i){
		HStack* item = (HStack*)entities[i];
		if (((Button*)item->entities[0])->is_activated()){ // 0, because the button the first element in the item hstack.
			return i;
		}
	}
	return size; // no checked item was found; return the last index.
}


int Menu::get_index_to_last_unchecked_item(){
	return get_index_to_first_checked_item()-1;
}



void Menu::process_animations(float dt){
	for (auto &anim : animations){
		anim.step(dt);
	}
}


void Menu::delete_stale_animations(){
	std::unordered_set<Entity*> addresses;

	for (int i = animations.size()-1; i >= 0; --i){
		if (addresses.count(animations[i].entity)){
			animations.erase(animations.begin() + i);
		} else {
			addresses.insert(animations[i].entity);
		}
	}

}



void Menu::update(float dt){
	select_rect.setPosition(get_position_of_entity(select_index));
	select_rect.setSize(sf::Vector2f(entities[select_index]->tf.w, entities[select_index]->tf.h));

	process_animations(dt);

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
			delete_stale_animations();

			if (item_is_checked(select_index)){ // UNCHECKING AN ITEM
				int first_checked_index = get_index_to_first_checked_item();

				if (select_index == first_checked_index){
					check_item(select_index);
					return true;
				}

				for (int i = first_checked_index; i <= select_index; ++i){
					sf::Vector2f A = get_position_of_entity(i);
					sf::Vector2f B = get_position_of_item((i == select_index) ? first_checked_index : i+1);
					PositionAnimation anim = PositionAnimation(entities[i], A, B);
					anim.start();
					animations.push_back(std::move(anim));
				}

				check_item(select_index);

				auto it = entities.begin() + select_index;
				std::rotate(entities.begin() + first_checked_index, it, it+1);


			} else { // CHECKING AN ITEM
				int last_unchecked_index = get_index_to_last_unchecked_item();

				if (select_index == last_unchecked_index){
					check_item(select_index);
					return true;
				}

				for (int i = select_index; i <= last_unchecked_index; ++i){
					sf::Vector2f A = get_position_of_entity(i);
					sf::Vector2f B = get_position_of_item((i == select_index) ? last_unchecked_index : i-1);
					PositionAnimation anim = PositionAnimation(entities[i], A, B);
					anim.start();
					animations.push_back(std::move(anim));
				}

				check_item(select_index);

				auto it = entities.begin() + select_index;
				std::rotate(it, it+1, entities.begin() + last_unchecked_index+1);

			}

			animations[0].lambda_complete = [&](){
				//elements_currently_static = true;  // NOTE: caused bug where elements were snapping to their destination.
			};

			elements_currently_static = false;

			return true;
		}
	}
	return false;
}


