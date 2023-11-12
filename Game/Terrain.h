#pragma once


#include "../Graphics/VAO.h"
#include "../Graphics/VBO.h"
#include "../Graphics/EBO.h"
#include "../Graphics/Texture.h"
#include "../Graphics/Shader.h"
		 
#include "SurfaceNet.h"
		 
#include <functional>
#include <glm/gtc/type_ptr.hpp>

const double chunkSize = 16.0;
const int chunkPrecision = 16;


class Chunk {
	// Will contain more stuff than just terrain in the future
private:

public:
	SurfaceNet surfaceNet;
	int chunkX, chunkY, chunkZ;
	Chunk(int x, int y, int z, Arr3D<double> data);

	void draw(glm::mat4 projMatrix, glm::mat4 viewMatrix);
};

struct i32vec3hasher {
	std::size_t operator()(const glm::i32vec3& k) const {
		// Generated by github copilot
		return k.x * 73856093 ^ k.y * 19349663 ^ k.z * 83492791;
	}
};

class Terrain2 {
	std::unordered_map<glm::i32vec3, std::shared_ptr<Chunk>, i32vec3hasher> chunks;
public:
	Terrain2() = default;

	std::shared_ptr<Chunk> getChunk(glm::i32vec3 pos);
	std::shared_ptr<Chunk> getChunk(int x, int y, int z);

	void generateChunk(int x, int y, int z);
	void draw(glm::mat4 projMatrix, glm::mat4 viewMatrix);
};

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

