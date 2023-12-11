#pragma once

#include<glm/glm.hpp>
#include <string>

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

	RectAlignment();
	RectAlignment(glm::vec2 center, glm::vec2 halfSize, glm::vec2 alignment);
	RectAlignment(RectAlignment other, glm::vec2 halfSize, glm::vec2 alignment);

	// Convert from pos1/pos2/alignment_p1 to center/halfSize/alignment_center.
	static RectAlignment fromPositions(glm::vec2 pos1, glm::vec2 pos2, glm::vec2 alignment);
	// No alignment means centered
	static RectAlignment fromPositions(glm::vec2 pos1, glm::vec2 pos2);

	static RectAlignment singleton(glm::vec2 pos);

	glm::vec2 getAbsAlignmentPos() const;

	// Get methods
	glm::vec2 getTopRight() const;
	glm::vec2 getTopLeft() const;
	glm::vec2 getBottomLeft() const;
	glm::vec2 getBottomRight() const;
	float getX1() const;
	float getX2() const;
	float getY1() const;
	float getY2() const;

	void align(RectAlignment other) {
		this->center = other.getAbsAlignmentPos() - alignment;
	}

	void flipVertical(int screenHeight);

	std::string toString() const;
};