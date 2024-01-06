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
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};


uniform Material material;
uniform Light light;


void main()
{
    //FragColor = mix((texture(texture1, TexCoord)* vec4(MainColor, 1.0)), texture(texture2, TexCoord), 0.7) ;
    //FragColor = texture(texture2, TexCoord);// * vec4(ourColor, 1.0f);


    vec3 ambient = light.ambient * material.ambient;


    // calculates the direction vector between the light source
    // and the fragment position
    //
    // with lighting, we do not normally care about
    // the magnitide, and only the angle. This means
    // we try to keep all vectors as unit vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    
    // nect we calculate the diffuse impact of the light
    // on the current fragment by taking the dot product
    // between the norm and lightDir vectors
    // the result is then multilied by the lights colors
    // which gets the diffuse component, resulting in a darker diffuse component

    // max prevents a negative number
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;


    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;  


    // point light specific
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);


}
