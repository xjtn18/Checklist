#pragma once
#include <Debug.hpp>
#include <vb.hpp>
#include <functional>
#include <Entity.hpp>

using namespace vb;


struct Animation {
	float x, y; 			// marks current time in the animation :/: marks the stage of the animation.
	float target;		// determines what the last stage will be.
	float rate;			// determines how quickly the animation will run.
	std::function<float(float)> f;			// function that maps x to the position in the animation.
	std::function<float(float)> inversef;	// function that maps y to a time in the animation.
	std::function<void(float)> stepper;
	std::function<void(void)> lambda_complete;
	bool reached_destination;
	bool dead;			// the active state of the animation.
	int delay_target;

	Animation()
		: x(0),
		  y(0),
		  target(1),
		  rate(1.0),
		  f([](float x) -> float {return 1.0f;}),
		  inversef([](float x) -> float {return 0.0f;}),
		  stepper([](float x) -> void {}),
		  lambda_complete([]() -> void {}),
		  reached_destination(false),
		  dead(true),
		  delay_target(0)
	{ }

	virtual ~Animation()
	{}

	void start(){
		dead = false;
		reached_destination = false;
		x = 0;
		//rate = (rate < 0) ? -rate : rate; // set positive rate
	}

	void interrupt(){
		x = inversef(1 - f(x));
	}

	void tick(float dt){
		if (delay_target > 0){
			--delay_target;
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


	virtual void step(float dt){
		if (is_alive()){
			tick(dt);
			stepper(y);
		}
	}

};



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




struct OpacityAnimation : public Animation {
};




struct ScaleAnimation : public Animation {
	///// Members /////
	sf::Transformable *polygon;
	float scale_A, scale_B;
	float home, target;
	bool forw = false; // animation is scaling from A -> B
	///////////////////

	ScaleAnimation(sf::Transformable *poly, float a, float b)
		: Animation(),
		polygon(poly),
		scale_A(a), scale_B(b),
		home(scale_A), target(scale_B)
	{
	}


	void forward(){
		home = scale_A;
		target = scale_B;
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
		home = scale_B;
		target = scale_A;
		if (is_alive()){
			if (forw){
				interrupt();
			}
		} else if (forw) {
			start();
		}
		forw = false;
	}

	void boomerang(){
		if (forw){
			back();
		} else {
			forward();
		}
	}


	virtual void step(float dt) override {
		if (is_alive()){
			Animation::step(dt); // calculate the new y
			float scale = home + (target - home) * y;
			polygon->setScale(scale, scale);
		}
	}

};










struct PositionAnimation : public Animation {
	///// Members /////
	Entity *entity;
	sf::Vector2f pos_A, pos_B; // two positions that the animation transitions between
	sf::Vector2f home, target; // specifies which of the 2 locations is home and which is target.
	bool forw = true; // animation is travelling A -> B
	///////////////////

	PositionAnimation() : entity(nullptr) { }

	PositionAnimation(Entity *ent, sf::Vector2f a, sf::Vector2f b)
		: Animation(),
		entity(ent),
		pos_A(a), pos_B(b),
		home(a), target(b)
	{
		rate = 2.0;
		f 			= [](float x) { return pow(-pow(1/(x/2 + 0.5) - 1, 3) + 1, 3); }; // fluid transition
		inversef = [](float x) { return 2/((cbrt(cbrt(-x) + 1)) + 1) - 1; }; // its inverse
	}


	void forward(){
		home = pos_A;
		target = pos_B;
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
		home = pos_B;
		target = pos_A;
		if (is_alive()){
			if (forw){
				interrupt();
			}
		} else if (forw) {
			start();
		}
		forw = false;
	}

	void boomerang(){
		(forw) ? back() : forward();
	}

	virtual void step(float dt) override {
		if (is_alive()){
			Animation::step(dt); // calculate the new y
			entity->set_position(get_intermediate_position(home, target, y));
		}
	}

};



