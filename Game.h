#pragma once
#include <iostream>
#include <queue>
#include <random>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Phys_Object.h"
#include"Shader.h"
#include"Phys.h"
#include"Player.h"
#include"resourceManager.h"
#include"Terrain.h"
#include"Sky.h"
#include"Cloud.h"

enum class GameState {
	START_MENU,
	PAUSE_MENU,
	SETTINGS,
	IN_GAME
};

class Game
{
public:
	GLFWwindow* window;

	std::unique_ptr<Physics> physEngine;
	std::unique_ptr<Player> p;
	std::unique_ptr<Terrain> terrain;
	std::unique_ptr<Sky> tempSky;
	std::unique_ptr<Clouds> tempClouds;

	GameState gameState = GameState::START_MENU;

	static std::queue<int> keyEvents;

	static bool updateScreenSize;
	static int scrWidth, scrHeight;

	Game();

	int init();
	int run();

	void startMenu_draw();
	void startMenu_tick(double frameTime);

	void inGame_draw();
	void inGame_tick(double frameTime);

	int cleanup();
};

