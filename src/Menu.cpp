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
	add_item("animate concurrent checks");
	add_item("integrate text fields");
	add_item("change color scheme");
	add_item("lerp the text opacity");

	add_item("Evil mode");
	add_item("Sawayama Solitaire");
	add_item("Klondike");
	add_item("Proletariats Patience");


	add_item("Master Chief");
	add_item("High Charity");
	add_item("Truth, Mercy, and Regret");
	add_item("Halo array");

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


void Menu::toggle_item(const int &index){
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


void view_animations(std::vector<PositionAnimation> &animations){
	for (auto &anim : animations){
	   dlog(anim.entity);
	}
}


void Menu::delete_stale_animations(){
	std::unordered_set<Entity*> addresses;

	for (int i = animations.size()-1; i >= 0; --i){
		if (!animations[i].is_alive() || addresses.count(animations[i].entity)){
			animations.erase(animations.begin() + i);
		}
		addresses.insert(animations[i].entity);
	}

	//view_animations(animations);
}



bool Menu::is_item_currently_animating(const int index){
	if (index < 0 || index >= size) return false;

	for (auto anim : animations){
		if (anim.entity == entities[index]){
			return true;
		}
	}
	return false;
}



void Menu::check_item(){
	int last_unchecked_index = get_index_to_last_unchecked_item();

	if (select_index == last_unchecked_index){
		toggle_item(select_index);
		return;
	}

	int delay = 0;
	int delay_seperation = 2;
	bool last_item_was_static = false;

	for (int i = select_index; i <= last_unchecked_index; ++i){
		sf::Vector2f A = get_position_of_entity(i);
		sf::Vector2f B = get_position_of_item((i == select_index) ? last_unchecked_index : i-1);
		PositionAnimation anim = PositionAnimation(entities[i], A, B);

		last_item_was_static = !is_item_currently_animating(i) || last_item_was_static;
		delay += delay_seperation;
		if (last_item_was_static){
			anim.delay_target = delay;
		}

		anim.start();
		animations.push_back(std::move(anim));
	}

	toggle_item(select_index);

	auto it = entities.begin() + select_index;
	std::rotate(it, it+1, entities.begin() + last_unchecked_index + 1);
}



void Menu::uncheck_item(){
	int first_checked_index = get_index_to_first_checked_item();

	if (select_index == first_checked_index){
		toggle_item(select_index);
		return;
	}

	int delay = 0;
	int delay_seperation = 2;
	bool last_item_was_static = false;

	for (int i = select_index; i >= first_checked_index; --i){
		sf::Vector2f A = get_position_of_entity(i);
		sf::Vector2f B = get_position_of_item((i == select_index) ? first_checked_index : i+1);
		PositionAnimation anim = PositionAnimation(entities[i], A, B);


		last_item_was_static = !is_item_currently_animating(i) || last_item_was_static;
		delay += delay_seperation;
		if (last_item_was_static){
			anim.delay_target = delay;
		}

		anim.start();
		animations.push_back(std::move(anim));
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
				uncheck_item();

			} else { // CHECKING AN ITEM
				check_item();
			}

			elements_currently_static = false;

			delete_stale_animations();

			// @NOTE(jacob) Deleting stale animations must happen before and after we check/uncheck
			// the item for desired behavior. Only doing it before causes old animations to trigger
			// overlapping items in the list. Only doing it after breaks delayed item shifting.
			
			//dlog(animations.size());


			return true;


		case sf::Keyboard::F:
			uncheck_all();
			return true;

		}

	}

	return false;
}

