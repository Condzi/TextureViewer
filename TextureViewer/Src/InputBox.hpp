/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

#include <SFML/Graphics.hpp>

class InputBox final :
	public sf::Drawable
{
public:
	inline static const sf::Color OUTLINE_SELECTED = sf::Color::Red;

	sf::Text text;
	bool selected = false;
	// Stays just 1 frame
	bool confim = false;

	void processEvent( const sf::Event& ev );
	void update();

	void draw( sf::RenderTarget& target, sf::RenderStates states ) const override;
};