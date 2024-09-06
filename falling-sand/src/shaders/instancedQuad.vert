#version 460

layout (location = 0) in vec2 a_DefaultQuadPos;
layout (location = 1) in vec2 a_instancedQuadPos;
layout (location = 2) in vec3 a_instancedQuadCol;

uniform mat4 u_projMat;
uniform int u_QuadSize;

out vec3 color;

void main()
{
    gl_Position = u_projMat * vec4(a_instancedQuadPos + (u_QuadSize * a_DefaultQuadPos), 0.0, 1.0);
    color = a_instancedQuadCol;
}
