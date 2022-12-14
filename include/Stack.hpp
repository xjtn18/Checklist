#pragma once
#include <Entity.hpp>
#include <initializer_list>


struct VStack : public Entity {
	int size = 0;
	int padding;
	bool elements_currently_static = true;
	std::vector<Entity*> entities;


	VStack(){};
	VStack(vb::Transform _tf, int _padding, std::initializer_list<Entity*> l);
	~VStack();

	void clear();
	void insert(int index, Entity *entity);
	void remove(int index);

	// overrides
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
};


struct HStack : public Entity {
	int size = 0;
	int padding;
	std::vector<Entity*> entities;

	HStack(){};
	HStack(vb::Transform _tf, int _padding, std::vector<Entity*> l);
	~HStack();

	void clear();
	void insert(int index, Entity *entity);
	void remove(int index);

	// overrides
	void update(float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
};


