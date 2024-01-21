#pragma once
#include <iostream>
#include <queue>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
		 
#include "Physics/Primitives.h"
#include "../Graphics/Shader.h"
#include "Physics/Phys.h"
#include "Player.h"
#include "../resourceManager.h"
#include "Terrain.h"
#include "Sky.h"
#include "Cloud.h"
#include "../UI/Button.h"
#include "../UI/Text.h"
#include "../UI/Scene.h"
#include "../UI/Slider.h"
#include "../UI/Shapes.h"
#include "SurfaceNet.h"

enum class GameState {
	START_MENU,
	PAUSE_MENU,
	SETTINGS,
	IN_GAME
};

class Game {
public:
	GLFWwindow* window;

	//std::unique_ptr<Physics> physEngine;
	std::unique_ptr<Player> p;
	std::unique_ptr<HeightmapTerrain> terrain;
	std::unique_ptr<SurfaceNetTerrain> terrain2;
	std::unique_ptr<Sky> tempSky;
	std::unique_ptr<Clouds> tempClouds;
	std::unique_ptr<Scene> startScene;
	std::unique_ptr<SurfaceNet> debugSurfaceNet;

	GameState gameState = GameState::START_MENU;

	static std::queue<KeyEvent> keyEvents;
	static std::queue<unsigned int> textEvents;
	static std::queue<MouseEvent> mouseEvents;
	static double lastXPos, lastYPos;

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

