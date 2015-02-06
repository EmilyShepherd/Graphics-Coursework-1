#version 150

precision highp float;

in vec3 in_Normal;
in vec3 in_Position;
in vec2 in_UV;

uniform mat4 mvpmatrix;

out vec4 normal;
out vec2 UV;

void main(void)
{
    gl_Position = mvpmatrix * vec4(in_Position, 1.0);
    normal = mvpmatrix * vec4(in_Normal, 0.0);
    UV = in_UV;
}

