#include"Texture.h"

std::string Texture::nullTexFile{ "resources/null.png" };

// Initialize to null texture, but don't duplicate the null texture.
Texture::Texture() {
	loadFromFile(nullTexFile.c_str(), "diffuse", 0);
}

Texture::Texture(const char* imageFile, const char* texType, GLuint slot) {
	loadFromFile(imageFile, texType, slot);
}

Texture::Texture(unsigned char* bytes, int width, int height, int numColCh, const char* texType, GLuint slot) {
	loadFromBytes(bytes, width, height, numColCh, texType, slot);
}

Texture::Texture(std::vector<float> data, int width, int height, int numColCh, const char* texType, GLuint slot, bool genMipmaps) {
	// Checks if the data has the right size
	if (data.size() != width * height * numColCh)
		throw std::invalid_argument("Data size does not match width, height, and number of color channels");

	// Assigns the type of the texture ot the texture object
	type = texType;

	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	if (genMipmaps) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (numColCh == 4) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_FLOAT,
			data.data());
	}
	else if (numColCh == 3) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGB,
			GL_FLOAT,
			data.data());
	}
	else if (numColCh == 1) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RED,
			GL_FLOAT,
			data.data());
	}
	else {
		throw std::invalid_argument("Automatic Texutre type recognition failed");
	}

	// Generates MipMaps
	if (genMipmaps) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadFromFile(const char* imageFile, const char* texType, GLuint slot) {
	printf("Loading texture %s\n", imageFile);
	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(imageFile, &widthImg, &heightImg, &numColCh, 0);

	loadFromBytes(bytes, widthImg, heightImg, numColCh, texType, slot);
	
	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(bytes);
}

void Texture::loadFromBytes(unsigned char* bytes, int width, int height, int numColCh, const char* texType, GLuint slot) {
	// Assigns the type of the texture ot the texture object
	type = texType;

	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	// Assigns the texture to a Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the OpenGL Texture object
	if (numColCh == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			width,
			height,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else
		throw std::invalid_argument("Automatic Texutre type recognition failed");

	// Generates MipMaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture Texture::hmapTexture(std::vector<float> data, int w, int h) {
	Texture t = Texture();
	t.type = "height";

	glGenTextures(1, &t.ID);
	glActiveTexture(GL_TEXTURE0);
	t.unit = 0;
	glBindTexture(GL_TEXTURE_2D, t.ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_R32F,
		w,
		h,
		0,
		GL_RED,
		GL_FLOAT,
		data.data());

	// Generates MipMaps
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	return t;
}

void Texture::texUnit(const Shader& shader, const char* uniform, GLuint unit) {
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}