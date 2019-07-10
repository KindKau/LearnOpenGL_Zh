#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourCoords;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    vec2 filpCoords = vec2(ourCoords.x /2.0f, ourCoords.y/2.0f);
    FragColor = mix( texture(texture1, ourCoords), texture(texture0, filpCoords) , 0.5 );
    //FragColor =  texture(texture1, ourCoords);
}