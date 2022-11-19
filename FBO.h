#ifndef FRAMEBUFFER_CLASS_H
#define FRAMEBUFFER_CLASS_H

#include<glad/glad.h>
#include"VAO.h"
#include"shaderClass.h"

class FBO
{
public:
	GLuint ID;						// Reference ID for OpenGL
	GLuint framebufferTexture;		// Texture object to store the frame
	GLuint framebufferDepthStencil;	// Texture that stores the depth and stencil buffer

	GLuint rectVAO, rectVBO;		// VAO and VBO for screen rectangle

	FBO();
	
	void Generate(int width, int height);

	void Bind();					// Bind the VAO such that frame data gets saved onto it
	void Draw(Shader& shader);					// Draws the processed images

	void Resize(int scrWidth, int scrHeight);   // Resizes the framebuffer when window is resized

	void Delete();					// Delete the Framebuffer obj
};

#endif