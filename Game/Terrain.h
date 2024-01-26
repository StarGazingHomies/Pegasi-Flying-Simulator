#pragma once


#include "../Graphics/VAO.h"
#include "../Graphics/VBO.h"
#include "../Graphics/EBO.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Shader.h"

#include "../config.h"

#include "SurfaceNet.h"		 
		 
#include <functional>
#include <glm/gtc/type_ptr.hpp>


class Chunk {
	// Will contain more stuff than just terrain in the future
private:
	std::vector<glm::vec3> grassLayerOffsets;
public:
	SurfaceNet surfaceNet;
	Texture grass, grassOffsets;
	VAO grassVAO;
	VBO grassLayerVBO;

	int chunkX, chunkY, chunkZ;
	Chunk(int x, int y, int z, Arr3D<double> data);

	void draw(glm::mat4 projMatrix, glm::mat4 viewMatrix);
	void tick(double deltaTime);
	// Note that this is in chunk coordinates
	double getValue(int x, int y, int z);
};

struct i32vec3hash {
	std::size_t operator()(const glm::i32vec3& k) const {
		// Generated by github copilot
		return k.x * 73856093 ^ k.y * 19349663 ^ k.z * 83492791;
	}
};

class SurfaceNetTerrain {
	std::unordered_map<glm::i32vec3, std::shared_ptr<Chunk>, i32vec3hash> chunks;
public:
	PerlinNoiseGenerator perlin;
	SurfaceNetTerrain(int seed);

	std::shared_ptr<Chunk> getChunk(glm::i32vec3 pos);
	std::shared_ptr<Chunk> getChunk(int x, int y, int z);

	void generateChunk(int x, int y, int z);
	void draw(glm::mat4 projMatrix, glm::mat4 viewMatrix);
	void tick(double deltaTime);
};

class HeightmapTerrain {
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
	HeightmapTerrain();

	// Two ways to generate
	void Load(const char* file);
	void Generate(int x, int y, int w, int h, int rx, int ry,
		std::function<float(float, float)> heightFunc = [](float a, float b) {return 0; });
	
	int getIndex(int x, int y);

	std::vector<float> getCoord(int x, int y);
	void setCoord(int x, int y, std::vector<float> vals);

	void Draw(const Shader& terrainShader, glm::mat4 proj, glm::mat4 view, glm::vec3 camPos);
};

