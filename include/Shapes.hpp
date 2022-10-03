#pragma once
#include <vb.hpp>
#include <Entity.hpp>


struct BorderedRect : public Entity {

	sf::RectangleShape rect;

	BorderedRect(vb::Transform _tf, int bw)
		: Entity(_tf)
	{
		rect.setSize({(float)tf.w - bw * 2, (float)tf.h - bw * 2});
		rect.setOrigin(-bw, -bw);
		rect.setFillColor(sf::Color::Transparent);

		rect.setOutlineThickness(bw);
		rect.setOutlineColor(COLOR_GREEN);
	}


	void update(float dt){
		rect.setPosition(tf.x, tf.y);
		rect.setFillColor(color);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(rect);
	}

};


struct BorderedCircle : public Entity {

	sf::CircleShape circle;
	int rad = 0;

	BorderedCircle(vb::Transform _tf, int _rad, int bw)
		: Entity(_tf), rad(_rad)
	{
		int actual_rad = rad - bw*2;
		tf.w = rad*2-bw*2;
		tf.h = rad*2-bw*2;
		circle.setRadius(actual_rad);
		circle.setPointCount(30);
		circle.setOrigin(-bw, -bw);
		//circle.setOrigin(rad, rad);
		circle.setFillColor(sf::Color::Transparent);

		circle.setOutlineThickness(bw);
		circle.setOutlineColor(COLOR_GREEN);
	}


	void update(float dt){
		circle.setPosition(tf.x, tf.y);
		//circle.setFillColor(color);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(circle);
	}

};




struct LineShape : public Entity {

	sf::RectangleShape line;

	LineShape(vb::Transform _tf)
		: Entity(_tf)
	{
		line.setOrigin(_tf.w/2, _tf.h/2);
		line.setFillColor(COLOR_GREEN);
		line.setSize({(float)tf.w, (float)tf.h});
	}

	void update(float dt){
		line.setPosition(tf.x, tf.y);
		line.setFillColor(color);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(line);
	}

};


struct Circle : public Entity {

	sf::CircleShape circle;
	int rad = 0;

	Circle(vb::Transform _tf, int _rad, int point_count)
		: Entity(_tf), rad(_rad)
	{
		circle.setRadius(_rad);
		circle.setPointCount(point_count);
		//circle.setOrigin(_rad, _rad);
		circle.setOutlineColor(COLOR_GREEN);
		circle.setFillColor(sf::Color(0,0,0,0));
		circle.setOutlineThickness(3);
	}

	void update(float dt){
		circle.setPosition(tf.x, tf.y);
		circle.setFillColor(color);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(circle);
	}

};

