#include "Alignment.h"

RectAlignment::RectAlignment() :
	center(0.0f),
	halfSize(0.0f),
	alignment(0.0f) {
}

RectAlignment::RectAlignment(glm::vec2 center, glm::vec2 halfSize, glm::vec2 alignment) :
	center(center), 
	halfSize(halfSize), 
	alignment(alignment) {
}

RectAlignment::RectAlignment(RectAlignment other, glm::vec2 halfSize, glm::vec2 alignment) {
	this->center = other.getAbsAlignmentPos() - alignment;
	this->halfSize = halfSize;
	this->alignment = alignment;
}

RectAlignment RectAlignment::fromPositions(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 alignment) {
	glm::vec2 center = (pos1 + pos2) / 2.0f;
	glm::vec2 halfSize = glm::abs(pos1 - pos2) / 2.0f;
	glm::vec2 newAlignment = alignment - halfSize;
	RectAlignment result{ center, halfSize, newAlignment };
	return result;
}

RectAlignment RectAlignment::fromPositions(glm::vec2 pos1, glm::vec2 pos2) {
	return fromPositions(pos1, pos2, glm::abs(pos1 - pos2) / 2.0f);
}

RectAlignment RectAlignment::singleton(glm::vec2 pos) {
	return RectAlignment{ pos, glm::vec2(0.0f), glm::vec2(0.0f) };
}

glm::vec2 RectAlignment::getAbsAlignmentPos() const {
	return center + alignment;
}

glm::vec2 RectAlignment::getTopRight() const {
	return center - halfSize;
}

glm::vec2 RectAlignment::getTopLeft() const {
	return center + glm::vec2(-halfSize.x, halfSize.y);
}

glm::vec2 RectAlignment::getBottomRight() const {
	return center + glm::vec2(halfSize.x, -halfSize.y);
}

glm::vec2 RectAlignment::getBottomLeft() const {
	return center + halfSize;
}

float RectAlignment::getX1() const {
	return center.x - halfSize.x;
}

float RectAlignment::getX2() const {
	return center.x + halfSize.x;
}

float RectAlignment::getY1() const {
	return center.y - halfSize.y;
}

float RectAlignment::getY2() const {
	return center.y + halfSize.y;
}

void RectAlignment::flipVertical(int screenHeight) {
	this->center.y = screenHeight - this->center.y;
}

std::string RectAlignment::toString() const {
	return "RectAlignment{center=" + std::to_string(center.x) + "," + std::to_string(center.y) + ",halfSize=" + std::to_string(halfSize.x) + "," + std::to_string(halfSize.y) + ",alignment=" + std::to_string(alignment.x) + "," + std::to_string(alignment.y) + "}";
}
