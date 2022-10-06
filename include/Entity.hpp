#pragma once
#include <vb.hpp>
#include <cmath>
#include <unordered_set>
#include <Animation.hpp>


struct Program; // FD


struct Entity : public sf::Drawable {
	vb::Transform tf;
	sf::Color color;
	bool engaged;
	PositionAnimation pos_anim;
	ColorAnimation col_anim;


	Entity(){}
	Entity(vb::Transform _tf)
		: tf(_tf), color(COLOR_DEFAULT), engaged(false)
	{ }

	Entity(vb::Transform _tf, bool _engaged)
		: tf(_tf), color(COLOR_DEFAULT), engaged(_engaged)
	{ }

	Entity(const Entity& other) = default;
	virtual ~Entity(){};


	void round_position(){
		tf.x = round(tf.x);
		tf.y = round(tf.y);
	}


	void set_position(sf::Vector2f pos){
		tf.x = pos.x;
		tf.y = pos.y;
	}

	sf::Vector2f get_position(){
		return sf::Vector2f(tf.x, tf.y);
	}


	void set_color(sf::Color col){
		color = col;
	}

	void set_alpha(int alpha){
		color.a = alpha;
	}

	
	void move(float x_offset, float y_offset, float mag, float dt){
		tf.x += (x_offset * mag * dt);
		tf.y += (y_offset * mag * dt);
	};



	// static UI entities do not require defining these
	virtual void engage(bool value)  {}

	virtual void update(float dt){
		pos_anim.step(dt);
		col_anim.step(dt);
	}

	virtual bool handler(sf::Event& event, Program& p) {return false;}


	// every entity will define a draw method
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;


};


