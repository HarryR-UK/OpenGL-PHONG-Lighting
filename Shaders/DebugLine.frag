#version 330 core


out vec4 FragColor;
uniform vec3 u_color;

void main()
{
    vec3 color = u_color;
    FragColor = vec4(color, 1.0);

}