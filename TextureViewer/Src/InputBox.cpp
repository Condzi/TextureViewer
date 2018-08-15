/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#include "InputBox.hpp"

// Debug
#include <iostream>

void InputBox::processEvent( const sf::Event& ev )
{
	if ( confim ) confim = false;

	if ( ev.type == sf::Event::TextEntered && selected ) {
		auto str = text.getString().toWideString();
		if ( ev.text.unicode == '\b' ) {
			if ( !str.empty() )
				str.pop_back();
		} else if ( ev.text.unicode == 13 ) // enter?
			confim = true;
		else
			str += ev.text.unicode;

		text.setString( str );
	}

	if ( ev.type == sf::Event::MouseButtonReleased )
		if ( ev.mouseButton.button == sf::Mouse::Left ) {
			// upper space on screen
			if ( sf::FloatRect{ 0,0,1280, 100 }.contains( ev.mouseButton.x, ev.mouseButton.y ) )
				selected = true;
			else
				selected = false;
		}
}

void InputBox::update()
{
	if ( selected ) {
		text.setOutlineColor( OUTLINE_SELECTED );
		text.setOutlineThickness( 1.f );
	} else
		text.setOutlineColor( sf::Color::Transparent );
}

void InputBox::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
	auto orgView = target.getView();
	target.setView( target.getDefaultView() );

	target.draw( text, states );

	target.setView( orgView );
}
