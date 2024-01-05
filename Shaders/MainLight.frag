#version 330 core

out vec4 FragColor;

uniform vec3 u_lightColor;

void main()
{
    vec3 color = u_lightColor;


    FragColor = vec4(color, 1.0);
}