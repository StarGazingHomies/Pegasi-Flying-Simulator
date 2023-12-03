#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 192) out;

in DATA {
	vec3 normal;
	vec3 color;
	//vec2 texCoord;
} inData[];

out vec3 pos;
out vec3 normal;
out vec3 color;
//out vec2 texCoord;
out float height;

uniform int shells = 64;
uniform float maxLength = 0.3;

// 3D transformation uniforms
uniform mat4 proj;
uniform mat4 view;

void main() {
	// Render each layer of the (possibly) grass
	for (int i=0; i<=shells; i++) {
		vec3 displacement = (float(i) / float(shells)) * inData[0].normal * maxLength;
		gl_Position = proj * view * (gl_in[0].gl_Position + vec4(displacement, 0.0));
		pos = gl_in[0].gl_Position.xyz;
		normal = inData[0].normal;
		color = inData[0].color;
		//texCoord = inData[0].texCoord;
		height = float(i) / float(shells);
		EmitVertex();
		
		displacement = (float(i) / float(shells)) * inData[1].normal * maxLength;
		gl_Position = proj * view * (gl_in[1].gl_Position + vec4(displacement, 0.0));
		pos = gl_in[1].gl_Position.xyz;
		normal = inData[1].normal;
		color = inData[1].color;
		//texCoord = inData[1].texCoord;
		height = float(i) / float(shells);
		EmitVertex();
		
		displacement = (float(i) / float(shells)) * inData[2].normal * maxLength;
		gl_Position = proj * view * (gl_in[2].gl_Position + vec4(displacement, 0.0));
		pos = gl_in[2].gl_Position.xyz;
		normal = inData[2].normal;
		color = inData[2].color;
//		texCoord = inData[2].texCoord;
		height = float(i) / float(shells);
		EmitVertex();

		EndPrimitive();
	}
}