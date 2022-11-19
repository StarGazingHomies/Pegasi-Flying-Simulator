#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<vector>

class EBO
{
public:
	GLuint ID;

	// Constructors
	EBO();
	EBO(std::vector<GLuint>& indices);

	// Bind the EBO to tell OpenGL to use it
	void Bind();
	// Unbind the EBO to avoid modifying it
	void Unbind();
	// Delete the EBO to free up VRAM
	void Delete();
};

#endif