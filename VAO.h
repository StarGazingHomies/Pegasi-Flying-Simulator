#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// ID reference for the VAO
	GLuint ID;
	// Constructor
	VAO();
		
	// Links a vertex attribute using a certain layout
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	// Instanced attribute linking
	void LinkAttribInstanced(VBO& VBO, GLuint divisor, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	// Bind the VAO
	void Bind();
	// Unbind the VAO
	void Unbind();
	// Delete the VAO
	void Delete();
};

#endif