#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourCoords;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragColor = mix( texture(texture0, ourCoords), texture(texture1, ourCoords), 0.5 );
    //FragColor =  texture(texture1, ourCoords);
}