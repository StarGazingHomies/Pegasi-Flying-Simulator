#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 acolor;
layout (location = 3) in vec2 aTex;

out DATA
{
    vec3 normal;
	vec3 color;
	vec2 texCoord;
    mat4 projection;
} data_out;

//out vec3 normal;
//out vec3 color;
//out vec2 texCoord;
//out mat4 projection;

//out vec3 crntPos;

// 3D transformation uniforms
uniform mat4 camMatrix;

uniform mat4 model;
uniform mat4 translation;
uniform mat4 scale;
uniform mat4 rotation;

void main() {
    // Negative rotation since GLTF and GLM uses different rotation standards (clockwise vs counterclockwise)
    //data_out.crntPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));

    // Export to geometry shader
    data_out.normal = aNormal;
    data_out.color = acolor;
    data_out.texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
    data_out.projection = camMatrix;

    // Export to fragment shader
//    normal = aNormal;
//    color = acolor;
//    texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
//    projection = camMatrix;

    gl_Position = translation * rotation * scale * model * vec4(aPos, 1.0f);
    //gl_Position = vec4(aPos, 1.0f) * translation * rotation * scale * model;
}