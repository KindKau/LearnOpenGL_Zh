#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourCoords;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    vec2 filpCoords = vec2(0.5 + ourCoords.x /10.0f, 0.5f + ourCoords.y/10.0f);
    //FragColor = mix( texture(texture1, ourCoords), texture(texture0, filpCoords) , 0.5 );
    FragColor =  texture(texture1, filpCoords);
}