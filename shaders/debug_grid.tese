// tessellation evaluation shader
#version 460 core

layout (quads, equal_spacing, ccw) in;

uniform sampler2D heightMap;  // the texture corresponding to our height map
uniform mat4 model;           // the model matrix
uniform mat4 view;            // the view matrix
uniform mat4 projection;      // the projection matrix

// received from Tessellation Control Shader - all texture coordinates for the patch vertices
in vec2 TextureCoord[];

// send to Fragment Shader for coloring
out DATA
{
    float height;
    mat4 model;
    mat4 view;
    mat4 projection;
    vec3 debug;
} data_out;

void main()
{
    // get patch coordinate
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // Bilinear interpolation for height
    float h00 = texture(heightMap, TextureCoord[0]).r;
    float h01 = texture(heightMap, TextureCoord[1]).r;
    float h10 = texture(heightMap, TextureCoord[2]).r;
    float h11 = texture(heightMap, TextureCoord[3]).r;

    float h0 = (h01 - h00) * u + h00;
    float h1 = (h11 - h10) * u + h10;
    data_out.height = (h1 - h0) * v + h0;

    data_out.projection = projection;
    data_out.view = view;
    data_out.model = model;
    data_out.debug = vec3(data_out.height / 128, 1.0f, 1.0f);

    // ----------------------------------------------------------------------
    // retrieve control point position coordinates
    vec4 p00 = gl_in[0].gl_Position;
    vec4 p01 = gl_in[1].gl_Position;
    vec4 p10 = gl_in[2].gl_Position;
    vec4 p11 = gl_in[3].gl_Position;

    // compute patch surface normal
    vec4 uVec = p01 - p00;
    vec4 vVec = p10 - p00;
    vec4 normal = normalize( vec4(cross(vVec.xyz, uVec.xyz), 0) );

    // bilinearly interpolate position coordinate across patch
    vec4 p0 = (p01 - p00) * u + p00;
    vec4 p1 = (p11 - p10) * u + p10;
    vec4 p = (p1 - p0) * v + p0;

    // displace point along normal
    p += normal * data_out.height;

    // ----------------------------------------------------------------------
    // output patch point position in clip space
    gl_Position = p;
}