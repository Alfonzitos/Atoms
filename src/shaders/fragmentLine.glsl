#version 430 core

in vec4 uCol;
in float vViewz;

out vec4 fragColor;

void main()
{
    fragColor = uCol;
}