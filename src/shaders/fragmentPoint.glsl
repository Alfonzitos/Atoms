#version 430 core

in vec4 uCol;
in float vViewz;

out vec4 fragColor;

void main()
{

    vec2 point = gl_PointCoord * 2.0 - 1.0; // multiply pos * 2 then sub 1
    float r2 = dot(point, point);

    if (r2 > 1.0) discard;

    
    float gaussian = exp(-4.0 * r2);


    float alpha = 1.0 - r2;
    fragColor = vec4(uCol.rgb, uCol.a * gaussian);
}