#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float u_time;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    float distance = length(aPos);
    gl_PointSize = 50.0 * (1.0 / distance);
}