#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;
out vec2 TexCoords;

// Should be ortho projection from pixels to canvas space
// Right words?
uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aPos.x, aPos.y, 0.0f, 1.0);
    TexCoords = aTex;
}