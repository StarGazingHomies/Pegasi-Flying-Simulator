// tessellation evaluation shader
#version 460 core

layout (quads, equal_spacing, ccw) in;

uniform mat4 view;            // the view matrix
uniform mat4 projection;      // the projection matrix

// send to Fragment Shader for coloring
out DATA
{
    float height;
    mat4 view;
    mat4 projection;
} data_out;

void main()
{
    // get patch coordinate
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // lookup texel at patch coordinate for height and scale + shift as desired
    data_out.height = 0;
    data_out.projection = projection;
    data_out.view = view;

    // retrieve control point position coordinates
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // bilinearly interpolate position coordinate across patch
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    // output patch point position in clip space
    gl_Position = p;
}