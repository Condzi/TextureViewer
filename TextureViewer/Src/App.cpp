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

	font.loadFromFile( "data/consolas.ttf" );
	inputBox.text.setFont( font );
	inputBox.text.setCharacterSize( 50 );
	inputBox.text.setString( "test" );

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

		inputBox.processEvent( ev );
	}
}

void App::update()
{
	inputBox.update();

	if ( inputBox.confim && !loadingInProgress ) {
		loadTextureAsync( inputBox.text.getString().toWideString() );
		loadingInProgress = true;
	}
}

void App::render()
{
	window.clear();
	window.draw( sprite );
	window.draw( inputBox );
	window.display();
}

void App::loadTextureAsync( const std::wstring& path )
{
	// Note: capture parameters as copies because they'll be destroyed after leaving this func, since thread will be detached.
	std::thread{ [path, this] {
		std::ifstream file( path, std::ifstream::binary );
		if ( !file ) {
			std::wcout << "Cannot open file \"" << path << "\".\n";
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

		if ( texture.loadFromMemory( content.data(), fileSize ) )
			sprite.setTexture( texture );
		else
			std::wcout << "Cannot load texture from memory (\"" << path << "\".\n";

		loadingInProgress = false;
	} }.detach();
}