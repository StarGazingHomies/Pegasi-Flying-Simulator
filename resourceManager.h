#pragma once

#include<map>
#include"Graphics/Shader.h"
#include"Graphics/Font.h"
#include"Game/Phys.h"
#include"UI/Button.h"
#include"Graphics/FBO.h"

class resourceManager
{
public:
	// Static resource manager for now, maybe things will be dynamically loaded later.
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, FBO> framebuffers;
	static std::map<std::string, Font> fonts;
	static std::map<std::string, Button> buttons;

	static Shader& loadShader(
		std::string name,
		const char* vertexFile,
		const char* fragmentFile,
		const char* geometryFile = nullptr,
		const char* tessControlFile = nullptr,
		const char* tessEvaluationFile = nullptr);

	static Shader& getShader(std::string name);

	static Font& loadFont(
		std::string name,
		const char* file,
		int fontSize,
		int scrWidth,
		int scrHeight);

	static Font& getFont(std::string name);

	static FBO& generateFBO(
		std::string name,
		int width,
		int height);

	static FBO& getFBO(std::string name);

	static Button& generateButton(
		std::string name, 
		int x1, int y1, int x2, int y2, 
		const char* defaultImg = nullptr, 
		const char* hoverImg = nullptr, 
		const char* pressedImg = nullptr
		);

	static Button& getButton(
		std::string name);

	static void updateScreenSize(int newWidth, int newHeight);
};

