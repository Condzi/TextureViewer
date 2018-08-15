/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#include "App.hpp"

#include <thread>
#include <atomic>

App::App()
{
	loadTextureAsync( "data/IMG1.jpg", [] {
		std::puts( "Load async!" );
	} );
}

bool App::loadTextureAsync( const std::string& path, std::function<void()> l )
{
	std::thread thread;
	std::atomic_bool endThread = false;

	thread = std::thread( [&] {
		while ( !endThread )
			l();
	} );

	auto succes = texture.loadFromFile( path );
	endThread = true;
	thread.join();

	return succes;
}
