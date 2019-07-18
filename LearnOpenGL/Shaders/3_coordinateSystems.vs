#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aCoords;

out vec3 ourColor;
out vec2 ourCoords;
//uniform float offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Local Space --> World Space --> Camera Space --> Clip Space
    // 获得裁剪坐标后，OpenGl会自动进行 透视除法 和 裁剪
    gl_Position = projection * view * model * vec4(aPos ,  1.0);
    ourColor = aColor;
    ourCoords =vec2(aCoords.x,aCoords.y);
}