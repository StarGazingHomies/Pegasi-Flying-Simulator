#pragma once

#include<glm/glm.hpp>

enum class AlignmentDirection {
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	CENTER_TOP,
	CENTER_LEFT,
	CENTER_RIGHT,
	CENTER_BOTTOM,
	CENTER
};

struct RectAlignment {
	glm::vec2 center;    // Center of the rectangle.
	glm::vec2 halfSize;  // Half of the size of the rectangle.
	glm::vec2 alignment; // Offset from center. <0.0, 0.0> means center-aligned. Measured in pixels.
	// center+alignment matched with another center+alignment

	RectAlignment(glm::vec2 center, glm::vec2 halfSize, glm::vec2 alignment) : center(center), halfSize(halfSize), alignment(alignment) {}

	RectAlignment(RectAlignment other, glm::vec2 halfSize, glm::vec2 alignment) {
		this->center = other.getAbsAlignmentPos() - alignment;
		this->halfSize = halfSize;
		this->alignment = alignment;
	}

	// Convert from pos1/pos2/alignment to center/halfSize/alignment.
	static RectAlignment fromPositions(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 alignment) {
		glm::vec2 center = (pos1 + pos2) / 2.0f;
		glm::vec2 halfSize = glm::abs(pos1 - pos2) / 2.0f;
		glm::vec2 newAlignment = alignment - halfSize;
		RectAlignment result{ center, halfSize, newAlignment };
		return result;
	}

	static RectAlignment singleton(glm::vec2 pos) {
		return RectAlignment{ pos, glm::vec2(0.0f), glm::vec2(0.0f) };
	}

	glm::vec2 getAbsAlignmentPos() const {
		return center + alignment;
	}

	glm::vec2 getTopRight() const {
		return center - halfSize;
	}

	void align(RectAlignment other) {
		this->center = other.getAbsAlignmentPos() - alignment;
	}

	void flipVertical(int screenHeight) {
		this->center.y = screenHeight - this->center.y;
	}

	std::string toString() const {
		return "RectAlignment{center=" + std::to_string(center.x) + "," + std::to_string(center.y) + ",halfSize=" + std::to_string(halfSize.x) + "," + std::to_string(halfSize.y) + ",alignment=" + std::to_string(alignment.x) + "," + std::to_string(alignment.y) + "}";
	}
};