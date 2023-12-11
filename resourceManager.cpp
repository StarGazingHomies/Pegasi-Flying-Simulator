#include "resourceManager.h"

std::map<std::string, Shader> resourceManager::shaders;
std::map<std::string, Font> resourceManager::fonts;
std::map<std::string, FBO> resourceManager::framebuffers;
std::map<std::string, Texture> resourceManager::textures;
std::string resourceManager::primaryFontName = "celestiaRedux";

Shader& resourceManager::loadShader(std::string name, const char* vertexFile, const char* fragmentFile, const char* geometryFile, const char* tessControlFile, const char* tessEvaluationFile) {
	// Since std::map automatically creates a shader object when using [] if one doesn't exist
	// we can do this and guarantee that we have a shader obj to compile.
	// It's not a good feature, and it's probably not good practice either, but I'm just having fun with C++ quirks
	shaders[name].Compile(vertexFile, fragmentFile, geometryFile, tessControlFile, tessEvaluationFile);
	return shaders[name];
}

Shader& resourceManager::getShader(std::string name) {
	if (shaders.find(name) == shaders.end()) {
		std::cout << "Shader " << name << " does not exist!" << std::endl;
		throw std::exception();
	}
	return shaders[name];
}

Font& resourceManager::loadFont(
	std::string name,
	const char* file,
	int fontSize,
	int scrWidth,
	int scrHeight) {
	fonts[name].load(file, fontSize, scrWidth, scrHeight);
	return fonts[name];
}

Font& resourceManager::getFont(std::string name) {
	if (fonts.find(name) == fonts.end()) {
		std::cout << "Font " << name << " does not exist!" << std::endl;
		return getPrimaryFont();
	}
	return fonts[name];
}

Font& resourceManager::getPrimaryFont() {
	return getFont(primaryFontName);
}

FBO& resourceManager::generateFBO(std::string name, int width, int height) {
	framebuffers[name].Generate(width, height);
	return framebuffers[name];
}

FBO& resourceManager::getFBO(std::string name) {
	if (framebuffers.find(name) == framebuffers.end()) {
		std::cout << "Framebuffer " << name << " does not exist!" << std::endl;
		throw std::exception();
	}
	return framebuffers[name];
}

Texture& resourceManager::loadTexture(std::string name, const char* file, const char* texType, GLuint slot) {
	textures[name] = Texture{ file, texType, slot };
	return textures[name];
}

Texture& resourceManager::getTexture(std::string name) {
	if (textures.find(name) == textures.end()) {
		std::cout << "Texture " << name << " does not exist!" << std::endl;
		return textures["null"];
	}
	return textures[name];
}

void resourceManager::updateScreenSize(int newWidth, int newHeight) {
	for (std::pair<std::string, Font> pair : fonts) fonts[pair.first].updateScreenSize(newWidth, newHeight);
}