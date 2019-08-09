#version 330 core
out vec4 FragColor;

in vec3 NormalVS;
in vec3 PosVS;
in vec3 LightPosVS;

uniform vec3 objectColor;
uniform vec3 lightColor;



void main()
{
    //****Phong Light Mode****

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = lightColor * ambientStrength;

    // diffuse
    vec3 Normal = normalize(NormalVS);
    vec3 LightVector = normalize(LightPosVS - PosVS);
    float diff = max(dot(Normal,LightVector),0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-LightVector, Normal);
    float spec = pow(max(dot(normalize(-PosVS),reflectDir),0.0),32);
    vec3 specular = specularStrength * spec * lightColor;


    // result
    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result,1.0);
}