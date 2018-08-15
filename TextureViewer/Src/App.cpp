/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#include "App.hpp"

#include <thread>
#include <fstream>
#include <iterator>
#include <locale>
#include <codecvt>
// Debug
#include <iostream>

App::App()
{
	// TODO: Make window resizable.
	window.create( { 1280,720 }, "Texture Viewer", sf::Style::Close );
	view = window.getDefaultView();

	loadTextureAsync( u8"data/ąćź.jpg" );
	executionLoop();
}

void App::executionLoop()
{
	while ( window.isOpen() ) {
		readInput();
		update();
		render();
	}
}

void App::readInput()
{
	sf::Event ev;
	float accumZoom = 1.f;

	while ( window.pollEvent( ev ) ) {
		switch ( ev.type ) {
		case sf::Event::Closed:
		{
			window.close();
			break;
		}
		case sf::Event::MouseWheelScrolled:
		{
			if ( moving ) break;

			if ( ev.mouseWheelScroll.delta <= -1 )
				currentZoom = std::min( ZOOM_MAX, currentZoom + .1f );
			else if ( ev.mouseWheelScroll.delta >= 1 )
				currentZoom = std::max( ZOOM_MIN, currentZoom - .1f );

			accumZoom *= currentZoom;

			view.setSize( window.getDefaultView().getSize() );
			view.zoom( currentZoom );
			window.setView( view );
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			if ( ev.mouseButton.button == sf::Mouse::Left ) {
				moving = true;
				oldPos = sf::Vector2f( sf::Mouse::getPosition( window ) );
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			if ( ev.mouseButton.button == sf::Mouse::Left )
				moving = false;
			break;
		}
		case sf::Event::MouseMoved:
		{
			if ( !moving ) break;

			const sf::Vector2f newPos( ev.mouseMove.x, ev.mouseMove.y );
			auto deltaPos = oldPos - newPos;

			deltaPos *= accumZoom;

			view.move( deltaPos );
			window.setView( view );

			oldPos = newPos;
			break;
		}
		}
		// Pass ev argument to input box here.
	}
}

void App::update()
{
	// update input box.
}

void App::render()
{
	window.clear();
	window.draw( sprite );
	// Draw input box here
	window.display();
}

void App::loadTextureAsync( const std::string& path )
{
	// Note: capture parameters as copies because they'll be destroyed after leaving this func, since thread will be detached.
	std::thread{ [=] {
		auto converted = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.from_bytes( path );

		std::ifstream file( converted, std::ifstream::binary );
		if ( !file ) {
			std::cout << "Cannot open file \"" << path << "\".\n";
			return;
		}

		using It = std::istreambuf_iterator<char>;
		// Strange way of getting file size.
		file.seekg( 0, std::ios::end );
		size_t fileSize = file.tellg();
		file.seekg( 0, std::ios::beg );


		std::vector<char> content;
		content.reserve( fileSize );
		std::move( It( file ), It(), std::back_inserter( content ) );

		if ( texture.loadFromMemory( content.data(), fileSize ))
			sprite.setTexture( texture );
		else
			std::cout << "Cannot load texture from memory (\"" << path << "\".\n";

		std::cout << "loaded";
	} }.detach();
}