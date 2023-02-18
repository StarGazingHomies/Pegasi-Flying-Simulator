#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;
//layout(triangle_strip, max_vertices = 3) out;

in DATA
{
    float height;
    vec2 texCoord;
    mat4 model;
    mat4 view;
    mat4 projection;
} data_in[];

out vec3 crntPos;
out vec2 texCoord;

void main()
{
    
    crntPos = vec3(gl_in[0].gl_Position);
    texCoord = data_in[0].texCoord;
    gl_Position = data_in[0].projection * data_in[0].view * data_in[0].model * gl_in[0].gl_Position;
    EmitVertex();
    
    crntPos = vec3(gl_in[1].gl_Position);
    texCoord = data_in[1].texCoord;
    gl_Position = data_in[1].projection * data_in[1].view * data_in[1].model * gl_in[1].gl_Position;
    EmitVertex();

    crntPos = vec3(gl_in[2].gl_Position);
    texCoord = data_in[2].texCoord;
    gl_Position = data_in[2].projection * data_in[2].view * data_in[2].model * gl_in[2].gl_Position;
    EmitVertex();
    
    crntPos = vec3(gl_in[0].gl_Position);
    texCoord = data_in[0].texCoord;
    gl_Position = data_in[0].projection * data_in[0].view * data_in[0].model * gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}