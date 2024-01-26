#version 460 core
out vec4 fragColor;

in vec3 normal;
in vec2 texCoord;

uniform sampler2D grass;

void main() {
    fragColor = vec4(0.4, 0.2, 0.1, 1.0);
}