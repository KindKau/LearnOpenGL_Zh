#version 330 core
out vec4 FragColor;

in vec2 ourCoords;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float offset;

void main()
{
    vec2 filpCoords = vec2(ourCoords.x /2.0f, ourCoords.y/2.0f);
    float offset_clp = clamp(offset, 0.0f, 1.0f);
    FragColor = mix( texture(texture1, ourCoords), texture(texture0, filpCoords) , offset_clp);
    //FragColor =  texture(texture1, ourCoords);
}