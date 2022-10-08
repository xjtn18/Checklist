#pragma once
#include <Debug.hpp>
#include <vb.hpp>
#include <functional>
#include <cmath>

using namespace vb;


struct Entity; // FD


template <typename T>
struct Animation {
	float x, y; 			// marks current time in the animation :/: marks the stage of the animation.
	float target;		// determines what the last stage will be.
	float rate;			// determines how quickly the animation will run.
	bool reached_destination;
	bool dead;			// the active state of the animation.
	bool forw = true; // animation is travelling A -> B
	int delay;
	T home, end;
	std::function<float(float)> f;			// function that maps x to the position in the animation.
	std::function<float(float)> inversef;	// function that maps y to a time in the animation.
	std::function<void(float)> stepper;
	std::function<void(void)> lambda_complete;

	Animation()
		: dead(true)
	{ }


	Animation(T _home, T _end)
		: x(0),
		  y(0),
		  target(1),
		  rate(1.0),
		  home(_home),
		  end(_end),
		  f([](float x) -> float {return 1.0f;}),
		  inversef([](float x) -> float {return 0.0f;}),
		  stepper([](float x) -> void {}),
		  lambda_complete([]() -> void {}),
		  reached_destination(false),
		  dead(true),
		  delay(0)
	{
		start();
	}

	virtual ~Animation()
	{}

	void start(){
		dead = false;
		reached_destination = false;
		x = 0;
	}

	void interrupt(){
		x = inversef(1 - f(x));
	}


	void boomerang(){
		(forw) ? back() : forward();
	}


	void tick(float dt){
		if (delay > 0){
			--delay;
			return;
		}
		x += (rate * dt);
		bool clamped = anim_clamp(x, 0, target + 0.001);
		if (clamped){
			reached_destination = (x == target);
			dead = true;
			lambda_complete();
		}
		y = f(x);
	}

	bool is_alive(){
		return !dead;
	}


	void step(float dt){
		if (is_alive()){
			tick(dt);
			stepper(y);
			update(y);
		}
	}

	virtual void update(float y) = 0;


	void forward(){
		std::swap(home, end);
		if (is_alive()){
			if (!forw){
				interrupt();
			}
		} else if (!forw) {
			start();
		}
		forw = true;
	}


	void back(){
		std::swap(home, end);
		if (is_alive()){
			if (forw){
				interrupt();
			}
		} else if (forw) {
			start();
		}
		forw = false;
	}

};





/*
struct AnimationChain : public Animation {
	std::vector<Animation> animations;
	int current;

	AnimationChain(std::vector<Animation>& alist)
		: animations(alist),
			current(-1)
	{}


	void start(){
		Animation::start();
		current = -1;
	}


	virtual void step(float dt) override {
		if (is_alive()){
			if (current == -1 || !animations[current].is_alive()){
				current += 1;
				if (current == animations.size()){ // all animations completed
					dead = true;
					return;
				}
				animations[current].start();
			}
			animations[current].tick(dt);
			animations[current].stepper(y);
		}
	}

};
*/






/*
struct ScaleAnimation : public Animation<float> {
	///// Members /////
	sf::Transformable *polygon;
	///////////////////

	ScaleAnimation(sf::Transformable *poly, float a, float b)
		: Animation<float>(a, b),
		polygon(poly)
	{
	}



	virtual void step(float dt) override {
		if (is_alive()){
			Animation::step(dt); // calculate the new y
			float scale = home + (target - home) * y;
			polygon->setScale(scale, scale);
		}
	}

};
*/







// TODO (jacob): Create a simple struct with a lerp function that uses the default x=y lerp function.
//     We'll use this to animate the opacity of newly checked and unchecked items.


struct ColorAnimation : public Animation<sf::Color> {
	///// Members /////
	Entity *entity;
	bool alpha_only;
	///////////////////

	ColorAnimation()
		: entity(nullptr)
	{ }

	ColorAnimation(Entity *ent, sf::Color b, bool ao = false);


	ColorAnimation(Entity *ent, sf::Color a, sf::Color b, bool ao = false)
		: Animation<sf::Color>(a, b),
		  entity(ent),
		  alpha_only(ao)
	{
		setup();
	}
	// @NOTE: I wonder if I can just call the other constructor from within this constructor
	//             so we dont need a "setup" function.

	void setup(){
		rate = 5.0;
		f 			= [](float x) { return x; }; // linear 
		inversef = [](float x) { return -x; }; // its inverse
	}

	virtual void update(float);
};







struct PositionAnimation : public Animation<sf::Vector2f> {
	///// Members /////
	Entity *entity;
	///////////////////

	PositionAnimation() : entity(nullptr) { }


	PositionAnimation(Entity *ent, sf::Vector2f b);


	PositionAnimation(Entity *ent, sf::Vector2f a, sf::Vector2f b)
		: Animation<sf::Vector2f>(a, b),
		  entity(ent)
	{
		setup();
	}


	void setup(){
		rate = 2.0;
		f 			= [](float x) { return pow(-pow(1/(x/2 + 0.5) - 1, 3) + 1, 3); }; // fluid transition
		inversef = [](float x) { return 2/((cbrt(cbrt(-x) + 1)) + 1) - 1; }; // its inverse
	}


	virtual void update(float);
};



