#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 


struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;

    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;


vec3 CalcDirLight(DirLight light,Material material, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(lightDir,normal);
    
    float diff = max(dot(lightDir,normal),0.0);
    float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);
    
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoords).rgb * diff;
    vec3 specular = light.specular * texture(material.specular, TexCoords).r * spec;

    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 viewDir,vec3 FragPos)
{
    vec3 lightDir =  normalize(light.position - FragPos);
    float diff = max(dot(lightDir,normal),0.0);
    
    vec3 reflectDir = reflect(viewDir,normal);
    float spec = pow(max(dot(reflectDir,viewDir),0.0), material.shininess);
    
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + dist*light.linear + dist*dist*light.quadratic);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb * attenuation;
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoords).rgb * diff * attenuation;
    vec3 specular = light.specular * texture(material.specular, TexCoords).r * spec * attenuation;
    
    return (ambient + diffuse + specular);
} 

vec3 CalcSpointLight(SpotLight light, Material material, vec3 normal, vec3 viewDir,vec3 FragPos)
{
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + dist*light.linear + dist*dist*light.quadratic);
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;
  	
    // diffuse 
    
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    
    // specular
    
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  
        
    // cutoff angle
    float cutoff;
    vec3 result;
    float theta = dot(lightDir, normalize(-light.direction));

    if(theta > light.cutoff)
    {
        result = ambient + diffuse + specular;
    }
    else
    {
        float intenstiy = (theta - light.outerCutoff) / (light.cutoff - light.outerCutoff);
        intenstiy = clamp(intenstiy,0.0,1.0);
        result = ambient * intenstiy + diffuse * intenstiy + specular * intenstiy;
    }
    result *= attenuation;
    return result;
}

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    vec3 result = CalcDirLight(dirLight,material,norm,viewDir);

    for(int i=0; i<NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i],material,norm,viewDir,FragPos);
    }
    
    result += CalcSpointLight(spotLight,material,norm,viewDir,FragPos);
    
    FragColor = vec4(result, 1.0);
} 