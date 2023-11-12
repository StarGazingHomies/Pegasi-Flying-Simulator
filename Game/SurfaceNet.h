#pragma once

#include <vector>
#include <random>

#include "../Graphics/Mesh.h"
#include "../resourceManager.h"
#include "Noise.h"

template <typename T>
class Arr3D {
private:
	std::vector<T> data;
	// Consider using std::map if data is sparse instead?
public:
	int width, height, depth;

	Arr3D() = default;
	Arr3D(int width, int height, int depth) {
		this->width = width;
		this->height = height;
		this->depth = depth;
		data.resize(width * height * depth);
	}
	T get(int x, int y, int z) {
		return data[x + y * width + z * width * height];
	}
	void set(int x, int y, int z, T val) {
		data[x + y * width + z * width * height] = val;
	}
	// TODO: Make proxy objects so that [] can be overloaded
};

struct SurfaceNetVertex {
	glm::vec3 pos, normal, color;
	int index;
	SurfaceNetVertex() {
		this->pos = this->normal = this->color = glm::vec3(0);
		this->index = -1;
	}
	SurfaceNetVertex(glm::vec3 pos, glm::vec3 normal, glm::vec3 color, int index) {
		this->pos = pos;
		this->normal = normal;
		this->color = color;
		this->index = index;
	};
};

class SurfaceNet {
public:
	int width, height, depth;
	// For scaling the vertices
	glm::vec3 pos1, pos2;
	Arr3D<double> arr; // Should be of size (width+1) * (height+1) * (depth+1)
	Arr3D<SurfaceNetVertex> vertices;
	std::vector<float> vertexData;
	std::vector<unsigned int> indices;
	int vertexCount, quadCount;

	VAO vao;
	VBO vbo;
	EBO ebo;
	
	SurfaceNet(glm::vec3, glm::vec3, Arr3D<double>);
	SurfaceNet(glm::vec3 pos1, glm::vec3 pos2, int width, int height, int depth);
	void generate();
	void draw(glm::mat4 projMatrix, glm::mat4 viewMatrix);
};

