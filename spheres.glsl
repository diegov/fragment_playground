#version 330 core

#define MAX_ITERS 50

out vec3 color;
in vec3 pos;

float epsilon = 0.001;

float sceneDistance(vec3 point) {
    float repeatX = 0.5;
    float x = mod(point.x + repeatX / 2.0, repeatX) - repeatX / 2.0;

    float repeatY = 0.5;
    float y = mod(point.y + repeatY / 2.0, repeatY) - repeatY / 2.0;

    float originalZ = point.z;
    float repeatZ = 1.3;
    float z = mod(point.z + repeatZ / 2.0, repeatZ) - repeatZ / 2.0;

    point = vec3(x, y, z);
    
    float l1 = (length(point - vec3(0.0, 0.0, 0.0)) - 0.19);
    //float l2 = (length(point - vec3(-0.15, 0.0, 0.1)) - 0.28);
    point.z = originalZ;
    //return min(max(l1, l2), 2.6 - point.z);
    return min(l1, 2.6 - point.z);
}


vec3 getNormal(vec3 point) {
    return normalize(vec3(sceneDistance(vec3(point.x + epsilon, point.y, point.z)) - 
                          	sceneDistance(vec3(point.x - epsilon, point.y, point.z)),
                          sceneDistance(vec3(point.x, point.y + epsilon, point.z)) -
                          	sceneDistance(vec3(point.x, point.y - epsilon, point.z)),
                          sceneDistance(vec3(point.x, point.y, point.z + epsilon)) - 
                          	sceneDistance(vec3(point.x, point.y, point.z - epsilon))));
}

bool isLit(vec3 point, vec3 direction, vec3 normal, vec3 light) {
    vec3 currPos = point + direction * epsilon * 2.0 + normal * epsilon * 2.0;

    for (int j = 0;  j < MAX_ITERS; ++j) {
        float lightDist = length(light - currPos);
        float dist = sceneDistance(currPos);

        if (lightDist < dist) {
            return true;
        }
        
        if (dist < epsilon) {
            return false;
        }

        currPos = currPos + direction * dist;
    }    
    return false;
}

float lightContribution(vec3 point, vec3 normal, vec3 light) {
    vec3 toLight = light - point;
    vec3 lightDir = normalize(toLight);
    float basicLight = clamp(dot(normal, lightDir), 0.0, 1.0) / (1.0 + sqrt(length(toLight)));
    if (isLit(point, lightDir, normal, light)) {
        return basicLight;
    } else {
        return basicLight * 0.01 + 0.01;
    }
}

void main() {
    int resX = 400;
    int resY = 400;

    float height = float(resY);
    float width = float(resX);

    vec2 centerOffset;
    float divisor;
    if (height > width) {
        centerOffset = vec2(0.5, 0.5 + (((height / width) - 1.0) / 2.0));
        divisor = float(width);
    } else {
        centerOffset = vec2(0.5 + (((width / height) - 1.0) / 2.0), 0.5);
        divisor = float(height);
    }
    
    vec2 uv = vec2(gl_FragCoord.x / divisor, gl_FragCoord.y / divisor) - centerOffset;
    vec3 up = vec3(0, 1, 0.03);
    vec3 right = vec3(1, 0, 0.03);
    vec3 direction = normalize(cross(right, up));

    vec3 light = vec3(0.5, 0.5, -1.0);
    vec3 light2 = vec3(-0.5, 1.0, -0.7);

    direction = normalize(direction + vec3(0.25 * uv.x, 0.25 * uv.y, 0.0));

    vec3 currPos = vec3(uv.x * 3.0, uv.y * 3.0, -1.0);

    float travelled = 0.0;
    for (int j = 0;  j < MAX_ITERS; ++j) {
        float dist = sceneDistance(currPos);
        if (dist < epsilon) {
            vec3 normal = getNormal(currPos);
            float lightIntensity = lightContribution(currPos, normal, light);
            float lightIntensity2 = lightContribution(currPos, normal, light2);
            vec3 base = vec3(0.6, 0.9, 0.6) * clamp(lightIntensity + lightIntensity2, 0.0, 1.0);
            float clarity = 1.0 / (sqrt(1.0 + travelled));
            vec3 fog = vec3(0.3, 0.3, 0.36) * clarity;

            color = (base * clarity) + (fog * (1.0 - clarity));
            return;
        }

        travelled += dist;
        currPos = currPos + direction * dist;
    }
    
    color = vec3(0, 0, 0);
}
