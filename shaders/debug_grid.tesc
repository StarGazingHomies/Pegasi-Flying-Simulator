#version 460 core

// num of control points per patch
layout (vertices=4) out;

void main()
{
    // Pass through gl_Position
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    // Outer tesselation levels
    gl_TessLevelOuter[0] = 64;
    gl_TessLevelOuter[1] = 64;
    gl_TessLevelOuter[2] = 64;
    gl_TessLevelOuter[3] = 64;

    // Inner tesselation levels
    gl_TessLevelInner[0] = 64;
    gl_TessLevelInner[1] = 64;
}