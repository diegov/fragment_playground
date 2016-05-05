#version 330 core

#define M_PI 3.14159265358

out vec3 color;

uniform vec2 resolution;
uniform float time;

void main() {
    float x = gl_FragCoord.x / resolution.x - 0.5;
    float y = gl_FragCoord.y / resolution.y - 0.5;

    float light = clamp(length(vec2(x, y)), 0.0, 1.0);
    
    float r = clamp(x * y, 0.0, 1.0);
    float g = clamp(x, 0.0, 1.0);
    float b = clamp(y, 0.0, 1.0);
    vec3 color1 = vec3(r, g, b) * light;

    float rate = x * M_PI * 20.0;

    x = pow(x, length(vec2(x, y)) + 0.5);
    y = pow(y, length(vec2(x, y)) + 0.5);

    float min_repeat = 0.01;

    float repeat1 = sin(time * M_PI / (5.0 + length(vec2(x, y)) / 3.0)) / (1.0 + min_repeat) + min_repeat;
    float repeat2 = sin(time * M_PI / (16.0 + length(vec2(x, y)))) / (1.0 + min_repeat) + min_repeat;
    x = mod(x + repeat1 / 2.0, repeat1) - repeat1 / 2.0;
    y = mod(x + y + repeat2 / 2.0, repeat2) - repeat2 / 2.0;

    vec3 other_color = vec3(sin(rate) / 2.0 + 0.5, 0.0, cos(rate) / 2.0 + 0.5);

    color = mix(color1, other_color, pow(cos(y * M_PI * 10.0) / 2.0 + 0.5, 1.5));
}
