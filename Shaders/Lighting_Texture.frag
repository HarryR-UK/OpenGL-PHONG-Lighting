#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

//uniform vec3 lightPosition;

uniform vec3 viewPos;

struct Material
{
    vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct PointLight
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direciton;
    float cutOff;
};

#define NR_POINT_LIGHTS 1
#define NR_SPOT_LIGHTS 1

uniform Material material;
uniform PointLight pointLight[NR_POINT_LIGHTS];
uniform DirectionalLight directionalLight;

vec3 calcPointLights(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoord).rgb;
    // nect we calculate the diffuse impact of the light
    // on the current fragment by taking the dot product
    // between the norm and lightDir vectors
    // the result is then multilied by the lights colors
    // which gets the diffuse component, resulting in a darker diffuse component

    // max prevents a negative number
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoord).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoord).rgb; 


    // point light specific
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambient *  texture(material.texture_diffuse1, TexCoord).rgb;
    // nect we calculate the diffuse impact of the light
    // on the current fragment by taking the dot product
    // between the norm and lightDir vectors
    // the result is then multilied by the lights colors
    // which gets the diffuse component, resulting in a darker diffuse component

    // max prevents a negative number
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse1, TexCoord).rgb;

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoord).rgb; 


    // point light specific

    return (ambient + diffuse + specular);
}


void main()
{
    //FragColor = mix((texture(texture1, TexCoord)* vec4(MainColor, 1.0)), texture(texture2, TexCoord), 0.7) ;
    //FragColor = texture(texture2, TexCoord);// * vec4(ourColor, 1.0f);




    // calculates the direction vector between the light source
    // and the fragment position
    //
    // with lighting, we do not normally care about
    // the magnitide, and only the angle. This means
    // we try to keep all vectors as unit vectors
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = calcDirectionalLight(directionalLight, norm, viewDir);



    for(int i = 0; i < NR_POINT_LIGHTS; ++i)
    {
        result += calcPointLights(pointLight[i], norm, FragPos, viewDir);
    }


    FragColor = vec4(result, 1.0);


}
