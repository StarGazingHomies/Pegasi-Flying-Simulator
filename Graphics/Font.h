#pragma once

#include<ft2build.h>
#include FT_FREETYPE_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<map>
#include<string>
#include<vector>
#include<algorithm>
#include<stb/stb_image.h>

#include"Shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

struct Character {
    float        texLocation;// Top left corner
    glm::fvec2   Size;       // Size (width, height)
    glm::fvec2   Bearing;    // Offset from baseline
    glm::fvec2   Advance;    // Distance between current char and next one
};

struct Font_point {
    // Coordinates
    GLfloat x;
    GLfloat y;
    // Texture Coordinates
    GLfloat s;
    GLfloat t;
    // Colours
    GLfloat r;
    GLfloat g;
    GLfloat b;
};

struct DisplaySize {
    float height, width;
};

enum class Alignment {
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

struct DisplayPos {
    Alignment alignment;
    float x, y;
};

// TODO: Make text accept alpha channel in colours
class Font
{
public:

    // Stores all loaded characters
    std::map<char, Character> charMap;
    std::map<char, std::vector<float>> renderPos;
    std::vector<Font_point> charStash;
    // VAO and VBO for text rendering
    unsigned int VAO, VBO;
    unsigned int texture;

    // Screen width, screen height, and size used when loading the font
    unsigned int scrWidth, scrHeight, size;
    // The width and height of the texture atlas
    unsigned int atlas_width, atlas_height;

    // Max size due to allocated buffer size.
    unsigned long max_size = sizeof(float) * 1024 * 6;

    // Constructor
    Font();
    // Loads the font
    void Load(const char* fontName, unsigned int height, unsigned int scrWidth, unsigned int scrHeight);
    // Update the projection matrix to fit the new screen size
    void updateScreenSize(unsigned int newWidth, unsigned int newHeight);
    // Prepare to render a single line of text
    void renderLine(std::string text, float x, float y, unsigned int fontSize, glm::vec3 color, bool textShadow = true, float shadowOffset = 1.0f);
    // Prepare to render while aligning the text to some corner/center
    void renderLine(std::string text, DisplayPos pos, unsigned int fontSize, glm::vec3 color, bool textShadow = true, float shadowOffset = 1.0f);
    // Prepare to render a block of text, with automatic line splits
    void renderText(std::string text, float x, float y, float maxWidth, unsigned int fontSize, glm::vec3 color, bool centered = false, float spacing = 1.0f, bool textShadow = true, float shadowOffset = 1.0f);
    // Render all previously prepared text
    void renderAll(Shader& textShader);

    // Determine the width and height of a particular line of text
    std::pair<float, float> getTextSize(std::string text);

    // Get the absolute position of aligned text based on window size and the position
    std::pair<float, float> absolutePos(std::string text, int fontSize, DisplayPos position);

    // Delete the object
    void Delete();
};