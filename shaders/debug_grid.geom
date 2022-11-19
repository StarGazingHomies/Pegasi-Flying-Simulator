#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 4) out;
//layout(triangle_strip, max_vertices = 3) out;

in DATA
{
    float height;
    mat4 view;
    mat4 projection;
} data_in[];

void main()
{
    gl_Position = data_in[0].projection * data_in[0].view * gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = data_in[1].projection * data_in[1].view * gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = data_in[2].projection * data_in[2].view * gl_in[2].gl_Position;
    EmitVertex();
    
    gl_Position = data_in[0].projection * data_in[0].view * gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}