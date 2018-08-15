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
	// --- camera movement
	static constexpr float ZOOM_MAX = 5.f;
	static constexpr float ZOOM_MIN = 0.1f;
	float currentZoom = 1.f;
	bool moving = false;
	sf::Vector2f oldPos;
	sf::View view;
	// ---
	sf::Texture texture;
	sf::Sprite sprite;

	void executionLoop();
	void readInput();
	void update();
	void render();

	// Loads texture from path, while executing lambda in a loop. `l` is called until texture is loaded.
	void loadTextureAsync( const std::string& path );
};