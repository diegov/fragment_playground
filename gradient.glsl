#version 330 core

out vec3 color;

uniform vec2 resolution;

void main() {
    float x = gl_FragCoord.x / resolution.x;
    float y = gl_FragCoord.y / resolution.y;
    
    float r = clamp(x * y, 0.0, 1.0);
    float g = clamp(x, 0.0, 1.0);
    float b = clamp(y, 0.0, 1.0);
    color = vec3(r, g, b);
}
