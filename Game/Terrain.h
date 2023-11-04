#pragma once

#include"../Graphics/VAO.h"
#include"../Graphics/VBO.h"
#include"../Graphics/EBO.h"
#include"../Graphics/Texture.h"
#include"../Graphics/Shader.h"
#include<functional>
#include<glm/gtc/type_ptr.hpp>

class Terrain {
public:
	VAO terrainVAO;
	VBO terrainVBO;
	EBO terrainEBO;
	Texture heightMap;

	int posX, posY, width, height, resX, resY;
	float scale, shift;
	// For contiguous storage, indices are calculated by 5 * ( (resY+1) * x + y )
	// Each coord is obviously 3 floats + 2 tex coord floats

	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<float> heights;
	Terrain();

	// Two ways to generate
	void Load(const char* file);
	void Generate(int x, int y, int w, int h, int rx, int ry,
		std::function<float(float, float)> heightFunc = [](float a, float b) {return 0; });
	
	int getIndex(int x, int y);

	std::vector<float> getCoord(int x, int y);
	void setCoord(int x, int y, std::vector<float> vals);

	void Draw(const Shader& terrainShader, glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);
};

