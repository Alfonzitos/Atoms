#version 430 core
layout (location = 0) in vec3 uPos;
layout (location = 1) in vec4 color;

uniform mat4 uMVP;
uniform mat4 view;
uniform float uPointSize;

out vec4 uCol;
out float vViewz;

void main()
{
    vec4 viewPos = view * vec4(uPos, 1.0);
    gl_Position = uMVP * vec4(uPos, 1.0);
    gl_PointSize = uPointSize;
    uCol = color;
    vViewz = viewPos.z;
}