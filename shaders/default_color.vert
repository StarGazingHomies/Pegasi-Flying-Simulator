#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;


out vec3 normal;
out vec3 color;

// 3D transformation uniforms
uniform mat4 proj;
uniform mat4 view;

void main() {
	// Export to fragment shader
    normal = aNormal;
    color = aColor;

    gl_Position = proj * view * vec4(aPos, 1.0f);
    //gl_Position = vec4(aPos, 1.0f) * translation * rotation * scale * model;
}