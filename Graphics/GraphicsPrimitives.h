#pragma once

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include "../ResourceManager.h"
#include <glm/glm.hpp>

// Primitive drawing class
// Not efficient, but this class should only be used for debug purposes.
class GraphicsPrimitives {
private:
	static GraphicsPrimitives instance;
	static bool initialized;
	static std::unique_ptr<VAO> vao;
	static std::unique_ptr<VBO> vbo;
	static std::unique_ptr<EBO> ebo;
	GraphicsPrimitives();
public:
	static GraphicsPrimitives& get();
	static void initialize();
	void drawLine(glm::vec2 start, glm::vec2 end, glm::vec3 color, glm::mat4 proj);
	void drawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, glm::mat4 proj, glm::mat4 view);
};

