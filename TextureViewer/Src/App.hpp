/*
	Conrad 'Condzi' Kubacki 2018
	https://github.com/condzi
*/

#pragma once

#include <SFML/Graphics.hpp>

#include <functional>

class App final
{
public:
	App();

private:
	sf::RenderWindow window;
	sf::View view;
	sf::Texture texture;
	sf::Sprite sprite;

	// Loads texture from path, while executing lambda in a loop. `l` is called until texture is loaded.
	bool loadTextureAsync( const std::string& path, std::function<void()> l );
};