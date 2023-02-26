#version 410 core

in vec3 dir;

out vec4 FragColor;

uniform vec3 baseColor = vec3(0.1f, 0.0f, 0.1f);


// Sun & Moon position, radius, and color
uniform vec3 sunPos = vec3(0.707f, 0.707f, 0.0f);
uniform float sunRad = 0.97f;
uniform vec3 sunColor = vec3(0.92f, 0.87f, 0.7f);

uniform vec3 moonPos = vec3(-0.707f, -0.707f, 0.0f);
uniform float moonRad = 0.985f;
uniform vec3 moonColor = vec3(0.92f, 0.87f, 0.98f);

// No more than 100 stars?
const int maxStars = 100;
uniform vec3 stars[maxStars];
uniform float starRad = 0.999f;
uniform vec3 starColor = vec3(1.0f);

uniform sampler2D sunTex;
uniform sampler2D moonTex;

vec2 calcTexCoord(vec3 pos, vec3 dir, float rad) {
    // Radius [0..1]
    float r = sqrt((1 - dot(dir, pos)) / (1 - rad));

    vec3 right = vec3(-pos.z, 0, pos.x);
    // Get angle between dir and right
    float angle = atan(dot(cross(dir, right), pos), dot(dir, right));
    // Get cartesian coords and map to [0..1]
    return vec2(r*cos(angle), r*sin(angle))/2+0.5; 
}

void main()
{
    
    FragColor = vec4(0.0f);

    if (dot(dir, sunPos) > sunRad) {
        FragColor = texture(sunTex, calcTexCoord(sunPos, dir, sunRad));
    }

    if (dot(dir, moonPos) > moonRad) {
        FragColor = texture(moonTex, calcTexCoord(moonPos, dir, moonRad));
    }
    
    if (FragColor.w < 1.0f) {
	    FragColor = vec4(FragColor.xyz * FragColor.w + baseColor * (1 - FragColor.w), 1.0f);
        //FragColor = vec4(dir/2+0.5, 1.0f);

    }
//    for (int i = 0; i < maxStars; i++) {
//        if (dot(dir, stars[i]) > starRad) {
//            FragColor = vec4(starColor, 1.0f);
//        }
//    }
}
