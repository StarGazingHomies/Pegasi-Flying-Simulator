#include "Font.h"

const char* FONT_getErrorString(GLenum type) {
	switch (type) {
	case GL_NO_ERROR:
		return "GL_NO_ERROR";
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
	default:
		return "invalid error number";
	}
}

void FONT_printErrorString(std::string location) {
	std::cout << location << " in Font.cpp: " << FONT_getErrorString(glGetError())
		<< "\n";
}

Font::Font() {}

void Font::load(const char* fontName, unsigned int height,
	unsigned int scrWidth, unsigned int scrHeight) {
	// Store the width and height of the window
	Font::scrWidth = scrWidth;
	Font::scrHeight = scrHeight;
	Font::size = height;

	// Initialize freetype
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "FREETYPE_ERROR: Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontName, 0, &face)) {
		std::cout << "FREETYPE_ERROR: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, height);

	// To save typing
	FT_GlyphSlot g = face->glyph;
	int w = 0;
	int h = 0;

	// Go through each character and find the size
	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		w += g->bitmap.width;
		h = std::max(h, (int)g->bitmap.rows);
	}

	/* you might as well save this value as it is needed later on */
	atlas_width = w;
	atlas_height = h;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Disable byte alignment restriction so we can use a single byte to represent
	// the grayscale color
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Get rid of mipmap levels. Mipmap levels suck when you don't want to use it
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Sub textures in the atlas
	int x = 0;

	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) continue;

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows,
			GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		Character character = { (float)x / w,
							   glm::fvec2(g->bitmap.width, g->bitmap.rows),
							   glm::fvec2(g->bitmap_left, g->bitmap_top),
							   glm::fvec2(g->advance.x >> 6, g->advance.y >> 6) };
		x += g->bitmap.width;

		charMap.insert(std::pair<char, Character>(i, character));
	}

	// Clear FreeType resources
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Set up VAO and VBO for text.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	// No data for now, load data when ready.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, max_size, NULL, GL_DYNAMIC_DRAW);
	// Set up attribute pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
		(void*)0);  // vec2 pos
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
		(void*)(2 * sizeof(float)));  // vec2 texCoords
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
		(void*)(4 * sizeof(float)));  // vec3 color
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	// Stop binding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Font::updateScreenSize(unsigned int newWidth, unsigned int newHeight) {
	scrWidth = newWidth;
	scrHeight = newHeight;
}

void Font::renderLine(std::string text, float x, float y, float fontSize,
	glm::vec3 color, bool textShadow, float shadowOffset) {
	float scale = fontSize / size;
	// Calculate position for each glyph
	for (char c : text) {
		Character p = charMap[c];

		// Extract position from the character
		float xpos = x + p.Bearing.x * scale;
		float ypos = y - (p.Size.y - p.Bearing.y) * scale;

		float w = p.Size.x * scale;
		float h = p.Size.y * scale;

		// Advance the cursor to the start of the next character
		x += p.Advance.x * scale;
		y += p.Advance.y * scale;

		// Skip glyphs that have no pixels
		if (!w || !h) continue;

		// If textshadow is on, put it into the stash first
		if (textShadow) {
			charStash.push_back(FontPoint{ xpos + shadowOffset,
										  ypos + h - shadowOffset, p.texLocation, 0,
										  0.0f, 0.0f, 0.0f });
			charStash.push_back(FontPoint{ xpos + w + shadowOffset,
										  ypos - shadowOffset,
										  p.texLocation + p.Size.x / atlas_width,
										  p.Size.y / atlas_height, 0.0f, 0.0f, 0.0f });
			charStash.push_back(FontPoint{ xpos + shadowOffset, ypos - shadowOffset,
										  p.texLocation, p.Size.y / atlas_height,
										  0.0f, 0.0f, 0.0f });
			charStash.push_back(FontPoint{
				xpos + w + shadowOffset, ypos + h - shadowOffset,
				p.texLocation + p.Size.x / atlas_width, 0, 0.0f, 0.0f, 0.0f });
			charStash.push_back(FontPoint{ xpos + w + shadowOffset,
										  ypos - shadowOffset,
										  p.texLocation + p.Size.x / atlas_width,
										  p.Size.y / atlas_height, 0.0f, 0.0f, 0.0f });
			charStash.push_back(FontPoint{ xpos + shadowOffset,
										  ypos + h - shadowOffset, p.texLocation, 0,
										  0.0f, 0.0f, 0.0f });
		}
		// Put it into the stash, which is rendered by the renderAll() method.
		charStash.push_back(
			FontPoint{ xpos, ypos + h, p.texLocation, 0, color.x, color.y, color.z });
		charStash.push_back(
			FontPoint{ xpos + w, ypos, p.texLocation + p.Size.x / atlas_width,
					  p.Size.y / atlas_height, color.x, color.y, color.z });
		charStash.push_back(FontPoint{ xpos, ypos, p.texLocation,
									  p.Size.y / atlas_height, color.x, color.y,
									  color.z });
		charStash.push_back(FontPoint{ xpos + w, ypos + h,
									  p.texLocation + p.Size.x / atlas_width, 0,
									  color.x, color.y, color.z });
		charStash.push_back(
			FontPoint{ xpos + w, ypos, p.texLocation + p.Size.x / atlas_width,
					  p.Size.y / atlas_height, color.x, color.y, color.z });
		charStash.push_back(
			FontPoint{ xpos, ypos + h, p.texLocation, 0, color.x, color.y, color.z });
	}
}

void Font::renderLine(std::string text, RectAlignment pos, TextAlignment textAlignment, float fontSize, glm::vec3 color, bool textShadow, float shadowOffset) {
	RectAlignment p = Font::absolutePos(text, fontSize, pos, textAlignment);
	glm::vec2 topRightPos = p.getTopRight();
	renderLine(text, topRightPos.x, topRightPos.y, fontSize, color, textShadow, shadowOffset);
}

void Font::renderAll(const Shader& textShader) {
	// Enable blend so alpha is considered when overlaying
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Depth function GL_LEQUAL so that the thing drawn later will be on top
	glDepthFunc(GL_LEQUAL);

	// Bind the texture and shader (set up correct rendering state)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	textShader.Activate();
	glm::mat4 projection = glm::ortho(0.0f, (float)scrWidth, 0.0f, (float)scrHeight);
	glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(glGetUniformLocation(textShader.ID, "textColor"), 0.0f, 1.0f, 0.0f);
	glActiveTexture(GL_TEXTURE0);

	// Bind the VAO and VBO to prepare for drawing
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// If the current buffer is larger, use the current one
	if (charStash.size() * sizeof(FontPoint) < max_size)
		glBufferSubData(GL_ARRAY_BUFFER, 0, charStash.size() * sizeof(FontPoint),
			charStash.data());
	// Otherwise, make a larger buffer so everything can fit
	else {
		glBufferData(GL_ARRAY_BUFFER, charStash.size() * sizeof(FontPoint),
			charStash.data(), GL_DYNAMIC_DRAW);
		max_size = charStash.size() * sizeof(FontPoint);
	}

	// Draw!
	glDrawArrays(GL_TRIANGLES, 0, (int)charStash.size());

	charStash.clear();

	// Unbind VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VBO and texture.
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Reset depth function
	glDepthFunc(GL_LESS);
	// Disable blend again
	glDisable(GL_BLEND);
}

void Font::renderText(std::string text, float x, float y, float maxWidth,
	unsigned int fontSize, glm::vec3 color, bool centered,
	float spacing, bool textShadow, float shadowOffset) {
	float scale = (float)fontSize / size;

	int i = 0, lastTextPos = 0, lastWord = 0, line = 0;
	float curDrawPos = 0.0f, lastWordDrawPos = 0.0f;
	char c;
	std::string drawStr;
	while (i < text.length()) {
		c = text[i];
		// Find the word and split it. We don't want words on different lines, ever.
		if (c == ' ' or c == '\n') {
			lastWord = i;
			lastWordDrawPos = curDrawPos;
		}
		curDrawPos += charMap[c].Advance.x * scale;

		// If the current word exceeds max width, start a new line.
		// If we have a return character, start a new line.
		// Also, if the current word is too long even by itself, then the word takes
		// 1 line.
		if ((curDrawPos > maxWidth or c == '\n') and lastWord > lastTextPos) {
			// Draw the substring
			drawStr = text.substr(lastTextPos, lastWord - lastTextPos);
			if (centered)
				renderLine(drawStr, x - curDrawPos / 2,
					(y - line * scale * spacing * size), fontSize, color,
					textShadow, shadowOffset);
			else
				renderLine(drawStr, x, (y - line * scale * spacing * size), fontSize,
					color, textShadow, shadowOffset);

			// Reset for next line, keeping the last word
			curDrawPos -= lastWordDrawPos;
			lastWordDrawPos = 0;
			lastTextPos = lastWord + 1;
			line++;
		}
		i++;
	}

	drawStr = text.substr(lastTextPos);
	if (centered)
		renderLine(drawStr, x - curDrawPos / 2, (y - line * scale * spacing * size),
			fontSize, color, textShadow, shadowOffset);
	else
		renderLine(drawStr, x, (y - line * scale * spacing * size), fontSize, color,
			textShadow, shadowOffset);
}

RectAlignment Font::getTextBoundingBox(std::string text, float fontSize, TextAlignment textAlignment) {
	float x = 0, minY = 0, maxY = 0;
	for (char c : text) {
		Character p = charMap[c];

		x += p.Advance.x;
		minY = std::min(minY, p.Bearing.y - p.Size.y); // min is the bottom
		maxY = std::max(maxY, p.Bearing.y); // max is the top
	}
	float scale = fontSize / size;

	float finalHeight = (minY + maxY) * scale;
	float finalWidth = x * scale;
	float yOffset = 0;
	switch (textAlignment) {
	case (TextAlignment::LEFT):
		return RectAlignment::fromPositions(glm::vec2(0.0f), glm::vec2(finalWidth, finalHeight), glm::vec2(0, yOffset));
	case (TextAlignment::CENTER):
		return RectAlignment::fromPositions(glm::vec2(0.0f), glm::vec2(finalWidth, finalHeight), glm::vec2(finalWidth/2, yOffset));
	case (TextAlignment::RIGHT):
		return RectAlignment::fromPositions(glm::vec2(0.0f), glm::vec2(finalWidth, finalHeight), glm::vec2(finalWidth, yOffset));
	}
}

RectAlignment Font::absolutePos(std::string text, int fontSize, RectAlignment targetPos, TextAlignment textAlignment) {
	RectAlignment textPos = getTextBoundingBox(text, fontSize, textAlignment);
	targetPos.flipVertical(scrHeight);
	textPos.align(targetPos);
	return textPos;
}

std::vector<CharLinePos> Font::getLinePos(std::string text, RectAlignment pos, float fontSize) {
	RectAlignment p = Font::absolutePos(text, fontSize, pos);
	return getLinePos(text, p.getTopRight().x, p.getTopRight().y, fontSize);
}

std::vector<CharLinePos> Font::getLinePos(std::string text, float x, float y,
	float fontSize) {
	std::vector<CharLinePos> pos;
	float scale = fontSize / size;
	for (char c : text) {
		Character p = charMap[c];

		// Extract position from the character
		// float xpos = x + p.Bearing.x * scale;
		// float ypos = y - (p.Size.y - p.Bearing.y) * scale;

		float w = p.Size.x * scale;
		float h = p.Size.y * scale;

		// For now (simple), just use advance
		pos.push_back(CharLinePos{ x, x + p.Advance.x * scale });

		// Advance the cursor to the start of the next character
		x += p.Advance.x * scale;
		y += p.Advance.y * scale;

		// Skip glyphs that have no pixels
		if (!w || !h) continue;
	}
	return pos;
}

void Font::Delete() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture);
	charMap.clear();
}