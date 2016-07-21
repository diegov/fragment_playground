#version 330 core

#define M_PI 3.14159265358

out vec3 color;

uniform vec2 resolution;
uniform float time;

float trig_to_unit(float value) {
    return (value + 1.0) / 2.0;
}

vec3 sample(vec2 point) {
    if (point.x > 0 && point.x < 0.8 &&
        point.y > 0.2 && point.y < 0.8) {
        return vec3(1.0, 0.5, trig_to_unit(cos(point.y * M_PI * 10)));
    } else {
        return vec3(0.05, 0.05, 0.05);
    }
}

float get_amplitude(float x, float stretched_time) {
    return 0.007 + 0.018 * (cos((-x * 2.0 - stretched_time) * M_PI / 13.99) + cos((x + cos(-time * M_PI / 2.0)) * M_PI / 2.33));
}

void main() {
    float x = gl_FragCoord.x / resolution.x;
    float y = gl_FragCoord.y / resolution.y;

    float stretched_time = (time + sin(time * M_PI / 10.1) + 0.01 * cos(y * M_PI * 3.99)) * 2.0;
    float stretched_time2 = time + sin(time * M_PI / 7.1);

    float amplitude = get_amplitude(x, stretched_time);

    float x_compression = amplitude;
    int max_steps = 10;
    for (int j = 0; j < max_steps; ++j) {
        float prev_point = x * (1.0 / max_steps) * j;
        x_compression += get_amplitude(prev_point, stretched_time);
    }

    x_compression = x_compression / 3 * sqrt(clamp(x, 0.0, 1.0));
    
    float oscillation = amplitude * sin((x - stretched_time2 / 4.33)  * M_PI * 4.5) * sqrt(clamp(x, 0.0, 1.0));

    color = sample(vec2(x + 0.35 * x_compression, y + oscillation));
}
