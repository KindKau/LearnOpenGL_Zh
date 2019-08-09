#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

out vec3 Result;


void main()
{
    // Local Space --> World Space --> Camera Space --> Clip Space
    gl_Position = projection * view * model * vec4(aPos ,  1.0);

    // Pos World Space
    vec3 PosVS = vec3(view * model * vec4(aPos ,  1.0));

    // Normal World Space 法线矩阵：模型矩阵左上角的逆矩阵的转置矩阵(不等比缩放时必用)
    // 注意 inverse 逆矩阵计算 消耗很大 避免在Shader中使用
    vec3 NormalVS = normalize(mat3(transpose(inverse(view * model))) * aNormal);
    
    //*** Gouraud Light Model ***
    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = lightColor * ambientStrength;

    // diffuse
    vec3 lightPosVS = mat3(view) * lightPos;
    vec3 lightDir = normalize(lightPosVS - PosVS);
    float diff = max(dot(lightDir,NormalVS),0.0);
    vec3 diffuse = lightColor * diff;

    // specular
    vec3 viewDir = normalize(-PosVS);
    vec3 reflectDir = reflect(-lightDir,NormalVS);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular = lightColor * spec;


    Result = objectColor * (ambient + diffuse + specular);

    
    
}