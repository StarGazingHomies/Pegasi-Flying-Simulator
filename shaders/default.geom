#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;

out vec3 crntPos;
out vec3 normal;
out vec3 color;
out vec2 texCoord;

in DATA
{
    vec3 normal;
	vec3 color;
	vec2 texCoord;
    mat4 projection;
} data_in[];


// Default main function
void main()
{
    gl_Position = data_in[0].projection * gl_in[0].gl_Position;
    normal = data_in[0].normal;
    color = data_in[0].color;
    texCoord = data_in[0].texCoord;
    EmitVertex();

    gl_Position = data_in[1].projection * gl_in[1].gl_Position;
    normal = data_in[1].normal;
    color = data_in[1].color;
    texCoord = data_in[1].texCoord;
    EmitVertex();

    gl_Position = data_in[2].projection * gl_in[2].gl_Position;
    normal = data_in[2].normal;
    color = data_in[2].color;
    texCoord = data_in[2].texCoord;
    EmitVertex();
    
    gl_Position = data_in[0].projection * gl_in[0].gl_Position;
    normal = data_in[0].normal;
    color = data_in[0].color;
    texCoord = data_in[0].texCoord;
    EmitVertex();
    EndPrimitive();
}