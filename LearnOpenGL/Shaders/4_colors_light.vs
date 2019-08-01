#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Local Space --> World Space --> Camera Space --> Clip Space
    // 获得裁剪坐标后，OpenGl会自动进行 透视除法 和 裁剪
    gl_Position = projection * view * model * vec4(aPos ,  1.0);
}