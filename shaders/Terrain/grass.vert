#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aOffset;


out vec3 pos;
out vec3 normal;
//out vec2 texCoord;
smooth sample out float height;
flat out int instance;

uniform int shells = 16;
uniform float maxLength = 0.2;

// 3D transformation uniforms
uniform mat4 proj;
uniform mat4 view;

void main() {
	vec3 displacement = aOffset * maxLength;
	gl_Position = proj * view * (vec4(aPos + displacement, 1.0));
	pos = aPos + displacement;
	normal = aNormal;
	height = aOffset.y;
	instance = gl_InstanceID;
}