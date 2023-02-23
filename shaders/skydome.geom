#version 410 core

layout (triangles) in;
//layout (line_strip, max_vertices = 4) out;
layout(triangle_strip, max_vertices = 3) out;

out vec3 dir;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    // Applies projection and view matricies. 
    // Also converts to wireframe for debug
    // Sets depth to 1.0f to make this the background
    gl_Position = (projection * view * gl_in[0].gl_Position).xyww;
    dir = vec3(gl_in[0].gl_Position);
    EmitVertex();
    
    gl_Position = (projection * view * gl_in[1].gl_Position).xyww;;
    dir = vec3(gl_in[1].gl_Position);
    EmitVertex();
    
    gl_Position = (projection * view * gl_in[2].gl_Position).xyww;
    dir = vec3(gl_in[2].gl_Position);
    EmitVertex();
    
//    gl_Position = (projection * view * gl_in[0].gl_Position).xyww;
//    dir = vec3(gl_in[0].gl_Position);
//    EmitVertex();
    EndPrimitive();
}
