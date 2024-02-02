#version 330 core

layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormals;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;

    // this just does some stuff to allow the model matrix to not
    // distort the normals
    Normal = mat3(transpose(inverse(model))) * aNormals;

// Then the last thing we need is the actual fragment's position. We're going to do all the lighting calculations in world space so we want a vertex position that is in world space first. We can accomplish this by multiplying the vertex position attribute with the model matrix only (not the view and projection matrix) to transform it to world space coordinates. This can easily be accomplished in the vertex shader so let's declare an output variable and calculate its world space coordinates: 
    FragPos = vec3(model * vec4(aPos, 1.0));
}
