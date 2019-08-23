#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
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
uniform Light light;

void main()
{
    // attenuation
    float dist = length(light.position - FragPos);
    float attenuation = 1.0/(light.constant + light.linear * dist + light.quadratic * dist * dist);

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse,TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  
        
    // cutoff angle
    float cutoff;
    vec3 result;
    float theta = dot(lightDir, normalize(-light.direction));
    if(theta > light.cutoff)
    {
        result = ambient * attenuation + diffuse * attenuation + specular * attenuation;
    }
    else
    {
        float intenstiy = (theta - light.outerCutoff) / (light.cutoff - light.outerCutoff);
        intenstiy = clamp(intenstiy,0.0,1.0);
        result = ambient * attenuation + diffuse * attenuation + specular * attenuation;
        result *= intenstiy;
        //result = vec3(.01,0.1,0.1);
    }
    
    
    
    FragColor = vec4(result, 1.0);
} 