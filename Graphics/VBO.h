#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

class VBO {
public:
	GLuint ID = 0;
	unsigned int size = 0;
	bool initialized = false;

	// Default constructor
	VBO();
	// Constructor with vertices list
	VBO(std::vector<Vertex>& vertices);
	// Constructor with floats list
	VBO(float vertices[]);
	// Constructor with floats vector
	VBO(std::vector<float> vertices, GLenum usage = GL_STATIC_DRAW);

	// Initialize a VBO
	void Initialize();
	// Bind the VBO to tell OpenGL to use it
	void Bind();
	// Unbind the VBO to avoid accidentally modifying it
	void Unbind();
	// Change the VBO's data. Suggested to only use with GL_DYNAMIC_... VBO's
	void Data(std::vector<float> vertices);
	// Delete the VBO and free up VRAM.
	void Delete();
};

#endif