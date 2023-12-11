#include "Shapes.h"

TexturedRectangle::TexturedRectangle(std::string texture, glm::vec2 pos1, glm::vec2 pos2) {
	this->texture = texture;
	this->pos = RectAlignment::fromPositions(pos1, pos2);

	std::vector<float> vertices = {
		pos.getX1(), pos.getY1(), 0.0f, 0.0f,
		pos.getX2(), pos.getY1(), 1.0f, 0.0f,
		pos.getX2(), pos.getY2(), 1.0f, 1.0f,
		pos.getX1(), pos.getY2(), 0.0f, 1.0f
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		2, 3, 0
	};

	this->vao = VAO{};
	this->vao.Bind();

	this->vbo = VBO{ vertices };
	this->vbo.Bind();

	this->ebo = EBO{ indices };
	this->ebo.Bind();

	this->vao.LinkAttrib(this->vbo, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
	this->vao.LinkAttrib(this->vbo, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	this->vao.Unbind();
	this->vbo.Unbind();
	this->ebo.Unbind();
}

void TexturedRectangle::draw() {
	resourceManager::getShader("2DTexture").Activate();

	this->vao.Bind();
	resourceManager::getTexture(this->texture).Bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	resourceManager::getTexture(this->texture).Unbind();
	this->vao.Unbind();
}
