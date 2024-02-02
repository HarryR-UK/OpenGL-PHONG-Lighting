#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform vec3 lightDir;

vec3 calcDirectionalLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-lightDir);

    vec3 ambient = vec3(0.5);
    // nect we calculate the diffuse impact of the light
    // on the current fragment by taking the dot product
    // between the norm and lightDir vectors
    // the result is then multilied by the lights colors
    // which gets the diffuse component, resulting in a darker diffuse component

    // max prevents a negative number
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(1.0) * diff;

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = (vec3(1.0) * spec) * 0.8;  


    // point light specific

    return (ambient + diffuse + specular);
}
void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = calcDirectionalLight(norm, viewDir);


    FragColor = vec4(result, 1.0);
    
}
