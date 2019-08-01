#version 330 core
out vec4 FragColor;

in vec3 N;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = lightColor * ambientStrength;

    vec3 Normal = normalize(N);
    vec3 LightVector = normalize(lightPos - fragPos);
    float diff = max(dot(Normal,LightVector),0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;

    FragColor = vec4(result,1.0f);
}