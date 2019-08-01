#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 N;
out vec3 fragPos;

void main()
{
    // Local Space --> World Space --> Camera Space --> Clip Space
    gl_Position = projection * view * model * vec4(aPos ,  1.0);
    
    fragPos = vec3(model * vec4(aPos ,  1.0));
    N = aNormal;
}