// tessellation control shader
#version 460 core

// num of control points per patch
layout (vertices=4) out;

// input from tesselation control shader
in vec2 TexCoord[];
// output texture coordinates to tes eval shader
out vec2 TextureCoord[];

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);

void main()
{
    // attr passthrough
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    TextureCoord[gl_InvocationID] = TexCoord[gl_InvocationID];

    const int MIN_TESS_LEVEL = 4;
    const int MAX_TESS_LEVEL = 16;
    const float MIN_DISTANCE = 25;
    const float MAX_DISTANCE = 200;

    // transform vertex with view&model matrix (no projection yet)
    vec4 eyeSpacePos00 = view * model * gl_in[0].gl_Position;
    vec4 eyeSpacePos01 = view * model * gl_in[1].gl_Position;
    vec4 eyeSpacePos10 = view * model * gl_in[2].gl_Position;
    vec4 eyeSpacePos11 = view * model * gl_in[3].gl_Position;

    // Scale distance between 0 and 1 wrt min/max dist
    // Linear
    //float distance00 = clamp((abs(eyeSpacePos00.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);
    //float distance01 = clamp((abs(eyeSpacePos01.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);
    //float distance10 = clamp((abs(eyeSpacePos10.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);
    //float distance11 = clamp((abs(eyeSpacePos11.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0);

    // Raise the result to a power such that the cutoffs are a bit further away
    // Power of 2 makes them to sudden, but linear is a bit to close
    float distpow = 1.8;
    float distance00 = pow(clamp((abs(eyeSpacePos00.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0), distpow);
    float distance01 = pow(clamp((abs(eyeSpacePos01.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0), distpow);
    float distance10 = pow(clamp((abs(eyeSpacePos10.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0), distpow);
    float distance11 = pow(clamp((abs(eyeSpacePos11.z)-MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0), distpow);
    
    // Calculate tesselation levels of each side based on vertex 
    float tessLevel0 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00) );
    float tessLevel1 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01) );
    float tessLevel2 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11) );
    float tessLevel3 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10) );

    // Output outer tesselation levels
    gl_TessLevelOuter[0] = tessLevel0;
    gl_TessLevelOuter[1] = tessLevel1;
    gl_TessLevelOuter[2] = tessLevel2;
    gl_TessLevelOuter[3] = tessLevel3;

    // Inner tesselation levels are max of outer
    gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
    gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
}