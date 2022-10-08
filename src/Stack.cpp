#include <Stack.hpp>
#include <Shapes.hpp>
#include <algorithm>


VStack::VStack(vb::Transform _tf, int _padding, std::initializer_list<Entity*> l)
	: Entity(_tf),
	  padding(_padding),
	  entities(l)
{
}

VStack::~VStack(){}



void VStack::clear(){
	for (int i = 0; i < entities.size(); ++i){
		delete entities[i];
	}
	entities.clear();
	tf.h = 0;
}


void VStack::insert(int index, Entity *entity){
	if (index == -1){
		entities.push_back(entity);
	} else {
		entities.insert(entities.begin() + index, entity);
	}
	++size;
}

void VStack::remove(int index){
	delete entities[index];
	entities.erase(entities.begin() + index);
	--size;
}


void VStack::update(float dt) {
	tf.h = 0;
	float _y = tf.y;
	for (auto& e : entities){
		if (elements_currently_static){
			e->tf.y = _y;
		}
		_y += e->tf.h + padding;
		e->update(dt);

		tf.h += e->tf.h + padding;
		tf.w = std::max(e->tf.w, tf.w);
	}
	tf.h -= padding;

	Entity::update(dt); // step through animations
}


void VStack::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto& e : entities){
		target.draw(*e);

		// draw bounding rectangles
		continue;
		auto rect = BorderedRect(vb::Transform{0, 0, e->tf.w, e->tf.h}, 1);
		rect.rect.setPosition(0, e->tf.y);
		target.draw(rect);
	}
}






HStack::HStack(vb::Transform _tf, int _padding, std::vector<Entity*> l)
	: Entity(_tf),
	  padding(_padding),
	  entities(l)
{
}

HStack::~HStack(){}



void HStack::clear(){
	for (int i = 0; i < entities.size(); ++i){
		delete entities[i];
	}
	entities.clear();
	tf.w = 0;
}


void HStack::insert(int index, Entity *entity){
	if (index == -1){
		entities.push_back(entity);
	} else {
		entities.insert(entities.begin() + index, entity);
	}
	++size;
}

void HStack::remove(int index){
	delete entities[index];
	entities.erase(entities.begin() + index);
	--size;
}



void HStack::update(float dt) {
	tf.w = 0;
	float _x = tf.x;
	for (auto& e : entities){
		e->tf.y = tf.y;
		e->tf.x = _x;
		_x += e->tf.w + padding;
		e->update(dt);

		tf.w += e->tf.w + padding;
		tf.h = std::max(e->tf.h, tf.h);
	}
	tf.w -= padding;

	Entity::update(dt); // step through animations
}


void HStack::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for (auto& e : entities){
		target.draw(*e);
	}
}


