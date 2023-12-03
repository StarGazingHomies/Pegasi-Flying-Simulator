#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
//layout (location = 3) in vec2 aTexCoord;


out DATA {
	vec3 normal;
	vec3 color;
//	vec2 texCoord;
} outData;

void main() {
	// Export to geometry shader
    outData.normal = aNormal;
	outData.color = aColor;
	//outData.texCoord = aTexCoord;

    gl_Position = vec4(aPos, 1.0f);
}