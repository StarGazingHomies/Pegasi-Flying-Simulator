#include "resourceManager.h"

std::map<std::string, Shader> resourceManager::shaders;
std::map <std::string, Font> resourceManager::fonts;
std::map <std::string, FBO> resourceManager::framebuffers;


Shader& resourceManager::loadShader(std::string name, const char* vertexFile, const char* fragmentFile, const char* geometryFile, const char* tessControlFile, const char* tessEvaluationFile)
{
	// Since std::map automatically creates a shader object when using [] if one doesn't exist
	// we can do this and guarantee that we have a shader obj to compile.
	// It's not a good feature, and it's probably not good practice either, but I'm just having fun with C++ quirks
	shaders[name].Compile(vertexFile, fragmentFile, geometryFile, tessControlFile, tessEvaluationFile);
	return shaders[name];
}

Shader& resourceManager::getShader(std::string name)
{
	return shaders[name];
}

Font& resourceManager::loadFont(
	std::string name,
	const char* file,
	int fontSize,
	int scrWidth,
	int scrHeight) {
	fonts[name].Load(file, fontSize, scrWidth, scrHeight);
	return fonts[name];
}

Font& resourceManager::getFont(std::string name) {
	return fonts[name];
}

FBO& resourceManager::generateFBO(std::string name, int width, int height) {
	framebuffers[name].Generate(width, height);
	return framebuffers[name];
}

FBO& resourceManager::getFBO(std::string name) {
	return framebuffers[name];
}