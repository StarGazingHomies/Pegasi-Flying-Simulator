#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"Shader.h"
#include<vector>

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	// Default constructor for objects init with texture
	Texture();
	// Constructor with image, type of texture (diffuse, specular, height, etc), and texture slot
	Texture(const char* image, const char* texType, GLuint slot);
	// Constructor with bytes
	Texture(unsigned char* bytes, int width, int height, int numColCh, const char* texType, GLuint slot);
	Texture(std::vector<float> data, int width, int height, int numColCh, const char* texType, GLuint slot, bool genMipmaps = false);

	// Some other constructors for specific purposes
	static Texture hmapTexture(std::vector<float> data, int w, int h);

	// Assigns a texture unit to a texture
	void texUnit(const Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
#endif