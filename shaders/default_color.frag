#version 460 core
out vec4 fragColor;

in vec3 normal;
in vec3 color;

void main() {
	fragColor = vec4((normalize(normal)+vec3(1.0f)/2), 1.0f);

	//fragColor = vec4(color, 1.0f);
}