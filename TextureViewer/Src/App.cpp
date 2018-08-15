/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#include "App.hpp"

#include <thread>
#include <atomic>

App::App()
{
	// TODO: Make window resizable.
	window.create( { 800,600 }, "Texture Viewer", sf::Style::Close );

	auto succes = loadTextureAsync( "data/IMG1.jpg", [&] {
		sf::Event ev;
		while ( window.pollEvent( ev ) ) {
			if ( ev.type == sf::Event::Closed )
				window.close();
		}

		// Render some loading message, for now just change window color.
		window.clear( sf::Color::Red );
		window.display();
	} );

	if ( succes )
		sprite.setTexture( texture );

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
	while ( window.pollEvent( ev ) ) {
		if ( ev.type == sf::Event::Closed )
			window.close();

		// TODO: Space + LMB + Move = moving around
		// Scale view when resize. 
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

bool App::loadTextureAsync( const std::string& path, std::function<void()> l )
{
	// @Cleanup: this could be better, this window.setActive is ugly
	std::thread thread;
	std::atomic_bool endThread = false;

	// Setting window context lock.
	window.setActive( false );

	thread = std::thread( [&] {
		window.setActive( true );

		while ( !endThread )
			l();

		window.setActive( false );
	} );

	auto succes = texture.loadFromFile( path );
	endThread = true;
	thread.join();

	window.setActive( true );

	return succes;
}
