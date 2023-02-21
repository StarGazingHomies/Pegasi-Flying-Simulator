#include "resourceManager.h"

std::map<std::string, Shader> resourceManager::shaders;
std::map<std::string, Font> resourceManager::fonts;
std::map<std::string, FBO> resourceManager::framebuffers;
std::map<std::string, Button> resourceManager::buttons;


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

Button& resourceManager::generateButton(std::string name, int x1, int y1, int x2, int y2,
	const char* defaultImg, const char* hoverImg, const char* pressedImg) {
	buttons[name].set(x1, y1, x2, y2, defaultImg, hoverImg, pressedImg);
	return buttons[name];
}

Button& resourceManager::getButton(std::string name) {
	return buttons[name];
}

void resourceManager::updateScreenSize(int newWidth, int newHeight) {
	for (std::pair<std::string, Font> pair : fonts) fonts[pair.first].updateScreenSize(newWidth, newHeight);
}