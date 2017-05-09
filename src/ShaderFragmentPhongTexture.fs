#version 330 core

#define NUM_MAX_PLIGHTS 2
#define NUM_MAX_SLIGHTS 1

struct Material {
	sampler2D diffuse;
    sampler2D specular;
    float shininess;
};  

struct PLight {
	vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DLight {
	vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SLight {
	vec3 position;
    vec3 direction;
    float aperturaMin;
    float aperturaMax;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform DLight dlight;
uniform PLight plight[NUM_MAX_PLIGHTS];
uniform SLight slight[NUM_MAX_SLIGHTS];

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection);
vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection);
vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection);

void main(){

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
	
	//Directional
    vec3 result = DirectionalLight(dlight, norm, viewDir);
	
	//Puntual
    for(int i = 0; i < NUM_MAX_PLIGHTS; i++){
		result += PointLight(plight[i], norm, viewDir);
	}
    
	//Focal
    for(int i = 0; i < NUM_MAX_SLIGHTS; i++){
        result += SpotLight(slight[i], norm, viewDir);
	}
    
    color = vec4(result, 1.0);
} 

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection){
	vec3 lightDirection = normalize(-light.direction);
	
	//Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
    //Diffuse
    float diff = max(dot(Normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	
    //Specular
    vec3 reflectDir = reflect(-lightDirection, Normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
    return (ambient + diffuse + specular);
}

vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection){
	vec3 lightDirection = normalize(light.position - FragPos);
	
	//Ambient
    vec3 ambient  = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
    //Diffuse
    float diff = max(dot(Normal, lightDirection), 0.0);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
	
    //Specular
    vec3 reflectDir = reflect(-lightDirection, Normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	
    //Attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
	
    return (ambient + diffuse + specular);
}

vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection){
	vec3 lightDirection = normalize(light.position - FragPos);
	
	//Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	
    //Diffuse shading
    float diff = max(dot(Normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
	//Specular shading
    vec3 reflectDir = reflect(-lightDirection, Normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
	//Attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	//Spotlight intensity
    float theta = dot(lightDirection, normalize(-light.direction)); 
    float epsilon = light.aperturaMin - light.aperturaMax;
    float intensity = clamp((theta - light.aperturaMax) / epsilon, 0.0, 1.0);
    
	ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
	
    return (ambient + diffuse + specular);
}