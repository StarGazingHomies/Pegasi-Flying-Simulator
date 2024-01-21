//#include "GraphicsPrimitives.h"
//
//bool GraphicsPrimitives::initialized = false;
//GraphicsPrimitives GraphicsPrimitives::instance{};
//
//void GraphicsPrimitives::initialize() {
//	VAO vao;
//	VBO vbo;
//	EBO ebo;
//}
//
//GraphicsPrimitives GraphicsPrimitives::get() {
//	if (!initialized) {
//		initialize();
//		initialized = true;
//	}
//	return instance;
//}
//
//void GraphicsPrimitives::drawLine(glm::vec2 start, glm::vec2 end, glm::vec3 color, glm::mat4 proj) {
//	// Use the correct shader
//	Shader shader = resourceManager::getShader("2DColor");
//	shader.Activate();
//	glUniform4fv(glGetUniformLocation(shader.ID, "projection"), 1, glm::value_ptr(proj));
//
//	// Set up the VAO, VBO, and EBO
//	vao.Bind();
//	vbo.Bind();
//
//	// Set up the vertices
//	vao.LinkAttrib(vbo, 0, 2, GL_FLOAT, 2 * sizeof(float), (void*)0);
//	std::vector<float> vertices = {
//		start.x, start.y, color.r, color.g, color.b,
//		end.x, end.y, color.r, color.g, color.b
//	};
//	vbo.Data(vertices);
//
//	// Draw the line
//	glDrawArrays(GL_LINES, 0, 2);
//}
//
//void GraphicsPrimitives::drawLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, glm::mat4 proj, glm::mat4 view) {
//	// Shader
//	Shader shader = resourceManager::getShader("defaultColor");
//	shader.Activate();
//	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
//	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
//
//	// Set up the VAO and VBO
//	vao.Bind();
//	vbo.Bind();
//
//	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
//	// No need for normal when drawing a line
//	vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//
//	// Set up the vertices
//	std::vector<float> vertices = {
//		start.x, start.y, start.z, color.r, color.g, color.b,
//		end.x, end.y, end.z, color.r, color.g, color.b
//	};
//	vbo.Data(vertices);
//
//	// Draw the line
//	glDrawArrays(GL_LINES, 0, 2);
//}
