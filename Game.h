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

class Game
{
public:
	GLFWwindow* window;

	std::unique_ptr<Physics> physEngine;
	std::unique_ptr<Player> p;

	static std::queue<int> keyEvents;

	static int newWidth, newHeight;

	Game();

	int init();
	int run();
	int cleanup();
};

