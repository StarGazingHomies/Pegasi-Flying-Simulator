#pragma once

#include"Primitives.h"

class OBB;

class Physics {
public:
	std::vector<std::shared_ptr<OBB>> objects;

	Physics();
	void Draw(Shader& shader, glm::mat4 projMatrix, glm::mat4 viewMatrix);
	void Tick(double time);
};