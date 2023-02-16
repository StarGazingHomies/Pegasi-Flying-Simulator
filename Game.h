#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Phys_Object.h"
#include"Shader.h"
#include"Phys.h"
#include"Player.h"

class Game
{
	Physics physEngine;

	Game();

	void init();
	void run();
};

