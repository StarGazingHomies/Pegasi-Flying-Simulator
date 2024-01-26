#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec2 texCoord;

uniform float chunkSize = 8.0;

// 3D transformation uniforms
uniform mat4 proj;
uniform mat4 view;

void main() {
	// Export to geometry shader
    normal = aNormal;
    texCoord = fract(aPos.xz / chunkSize);
    gl_Position = proj * view * vec4(aPos, 1.0f);
}