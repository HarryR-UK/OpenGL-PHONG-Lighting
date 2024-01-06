#include <GL/glew.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <OpenGL/gl.h>
#include <iostream>

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Shader.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 1800
#define WINDOW_HEIGHT 1080

typedef glm::vec4 vec4;
typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

typedef glm::mat4 mat4;
typedef glm::mat3 mat3;


// initialises the last position to be the center of the screen
float lastXPosition = (float) (WINDOW_WIDTH * 0.5f);
float lastYPosition = (float) (WINDOW_HEIGHT * 0.5f);


bool mouseActive = false;
bool keyPressed = false;

float initialCameraPosY = 0.5f;

Camera camera(vec3(0.0f, initialCameraPosY, -3.0f));

bool isPaused = false;

void processInput(GLFWwindow* window, float& deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        if(!keyPressed)
        {
            keyPressed = true;
            mouseActive ? glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED) : glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseActive = !mouseActive;

        }
    }
    else{
        keyPressed = false;
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.setSprint(true);
    }
    else{
        camera.setSprint(false);
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.processKeyboardInput(FORWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {   
        camera.processKeyboardInput(BACKWARD, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.processKeyboardInput(LEFT, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.processKeyboardInput(RIGHT, deltaTime);
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.processKeyboardInput(UP, deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        camera.processKeyboardInput(DOWN, deltaTime);
    }


    //cameraPosition.y = initialCameraPosY;

}


float calculateDeltaTime(float& lastFrame, float& currentFrame)
{
    float dt = currentFrame - lastFrame;
    lastFrame = currentFrame;
    return dt;
}

void renderTriangle(Shader& shader, unsigned int& VAO, unsigned int& VBO, unsigned int& EBO, unsigned int& texture1, unsigned int& texture2)
{
    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


void mouseCallBack(GLFWwindow* window, double xPosition, double yPosition)
{
    float xOffset = xPosition - lastXPosition;
    float yOffset = lastYPosition - yPosition;

    lastXPosition = xPosition;
    lastYPosition = yPosition;

    if(!mouseActive)
        camera.processMouseMovement(xOffset, yOffset);

    // next we add the offset values to the globally declared pitch and yaw values


}


void renderLight(Shader& lightShader, unsigned int& lightVAO, unsigned int& lightVBO)
{
    lightShader.use();
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

int main(int argc, char* argv[])
{


    if(!glfwInit())
        return -1;
    

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 3);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);


    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // captures and maintains the mouse cursorc:w

    GLFWwindow* window;
    float SCR_WIDTH = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
    float SCR_HEIGHT = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

    SCR_WIDTH = WINDOW_WIDTH;
    SCR_HEIGHT = WINDOW_HEIGHT;

    window = glfwCreateWindow(SCR_WIDTH,
    SCR_HEIGHT, "OpenGL", NULL,
     NULL);




    if(!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        std::cout << "GLEW COULD NOT INITIALISE" << '\n';
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //glfwSwapInterval(1);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // sets the function associated with each callback of the mouse movement
    glfwSetCursorPosCallback(window, mouseCallBack);

    //IMGUI
    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);
    ImGui::StyleColorsDark();

    
    // vertices data opengl buffers

    float vertices[] = {

        // positions            TEX COORDS      NORMALS
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,    0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);



    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    // This sends the data from the CPU to thr GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // laying out the vertex data for the GPU

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Texture coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*) (5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    
    // setting up the element buffer objects
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    

    Shader triangleShader("./Shaders/Triangle_Vert.glsl", "./Shaders/Triangle_Frag.glsl");


    // textures

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load("/Users/testTheOne/OpenGLProjects/test1/Resources/Images/container.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }else{
        std::cout << "ERROR::TEXTURE::CANNOT_LOAD_TEXTURE0" << '\n';
    }
    // free the memory
    stbi_image_free(data);

    */
    // awesome face
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

/*
    unsigned char* data2 = stbi_load("/Users/testTheOne/OpenGLProjects/test1/Resources/Images/amoogus.png", &width, &height, &nrChannels, 0);
    if(data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "ERROR::TEXTURE::CANNOT_LOAD_TEXTURE1" << '\n';
    }

    stbi_image_free(data2);
*/
    stbi_set_flip_vertically_on_load(false);

    triangleShader.use();
    triangleShader.setInt("texture1", 0);
    triangleShader.setInt("texture2", 1);

    // FLOOR

    // reset all bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    Shader::unbind();


    







    // !FLOOR

    // transformations
    //identity matrix


    mat4 triangleModel = mat4(1.0f);
    triangleModel = glm::scale(triangleModel, vec3(5.0f, 0.3f, 5.0f));
    //model = glm::rotate(model, glm::radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
    //model = glm::scale(model, vec3(3.0f, 0.5f, 3.0f));
    //model = glm::rotate(model, glm::radians(20.0f), vec3(0.0f, 1.0f, 0.0f));

    mat4 proj = mat4(1.0f);
    proj = glm::perspective(glm::radians(camera.fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
    //proj = glm::ortho(0.0f, (float) WINDOW_WIDTH, 0.0f, (float) WINDOW_HEIGHT, 0.1f, 100.0f);
    
    // Camera


    // the lookat function requires a position, target and up vector to creates a view matrix 


    
    // pitch = lookuing up and down
    // yaw = magnitude in which were looking to the left or to the right
    // roll = how much we roll





    mat4 view = mat4(1.0f);               
    view = camera.getViewMatrix();



    // !Camera

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    triangleShader.use();
    triangleShader.setMat4("model", triangleModel);
    triangleShader.setMat4("view", view);
    triangleShader.setMat4("proj", proj);





    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    Shader::unbind();

    // LIGHTING


    // actual light object
    float lightVertices[] = {

        // positions            TEX COORDS
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //vec3 result = lightColor * objectColor; // = ( 1.0f, 0.5f, 0.31f)
    // This shows that the object absorbs a large portion of the white light, 
    // but will still refect a large portion of the Red  green and vlue
    // this reflects how lighting works in real life, the object will reflect certain colors of what it is
    // but will absorb a large proportion of other colors which it does not reflect
    Shader lightShader("./Shaders/MainLight.vert", "./Shaders/MainLight.frag");
    struct Light {
        vec3 position;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

    struct Material{
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
    };

    Light light;

    light.ambient = vec3(0.4f, 0.4f, 0.4f);
    light.diffuse = vec3(0.75f, 0.75f, 0.75f);
    light.specular = vec3(1.0f);
    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;


    Material material;
    material.ambient =vec3(0, 0.26f, 0.26f);
    material.diffuse = vec3(0.0f, 1.0f, 1.0f);
    material.specular = vec3(0.5f, 0.5f, 0.5f);
    material.shininess = 32.0f;

    mat4 lightModel = mat4(1.0f);
    lightModel = glm::scale(lightModel, vec3(0.2f,0.2f,0.2f));

    light.position = vec3(2.0f, 2.0f, 0.0f);
    lightModel = glm::translate(lightModel, light.position);



    vec3 objectColor = vec3(0.12, 0.87, 0.97);

    lightShader.use();
    lightShader.setMat4("u_projection", proj);
    lightShader.setMat4("u_view", view);
    lightShader.setMat4("u_model", lightModel);

    triangleShader.use();


    triangleShader.setVec3("viewPos", camera.position);

    triangleShader.setVec3("material.ambient", material.ambient);
    triangleShader.setVec3("material.diffuse", material.diffuse);
    triangleShader.setVec3("material.specular", material.specular);
    triangleShader.setFloat("material.shininess", material.shininess);

    triangleShader.setVec3("light.position", light.position);
    triangleShader.setVec3("light.ambient",  light.ambient);
    triangleShader.setVec3("light.diffuse",  light.diffuse); // darken diffuse light a bit
    triangleShader.setVec3("light.specular", light.specular); 
    triangleShader.setFloat("light.constant", light.constant);
    triangleShader.setFloat("light.linear", light.linear);
    triangleShader.setFloat("light.quadratic", light.quadratic);



    


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    Shader::unbind();


    ImVec4 clearColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);

    while(!glfwWindowShouldClose(window))
    {

            glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            ImGui_ImplGlfwGL3_NewFrame();


            float currentFrame = glfwGetTime();
            deltaTime = calculateDeltaTime(lastFrame, currentFrame);
            processInput(window, deltaTime);

            proj = glm::perspective(glm::radians(camera.fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
            view = camera.getViewMatrix();               

            //model = glm::rotate(model, glm::radians(10.0f) * deltaTime, vec3(0.0f, 1.0f, 0.0f));
            // UPDATE
                //model = glm::mat4(1.0f);
                //model = glm::rotate(model, glm::radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
                //model = glm::rotate(model, glm::radians((float) glfwGetTime()) * deltaTime, vec3(0.0f, 1.0f, 0.0f));
                //model = glm::rotate(model, glm::radians((float) glfwGetTime()) * deltaTime * 0.5f, vec3(1.0f, 0.0f, 0.0f));

                const float radius = 5.f;
                lightModel = mat4(1.0f);
                //lightPosition = vec3(sin(glfwGetTime()) * radius, sin(glfwGetTime()) * 5.0f, cos(glfwGetTime()) * radius);
                lightModel = glm::translate(lightModel, light.position);
                




            // END

            triangleShader.use();
            triangleShader.setMat4("model", triangleModel);
            triangleShader.setMat4("view", view);
            triangleShader.setMat4("proj", proj);    
            triangleShader.setVec3("viewPos", camera.position);

            triangleShader.setVec3("material.ambient", material.ambient);
            triangleShader.setVec3("material.diffuse", material.diffuse);
            triangleShader.setVec3("material.specular", material.specular);
            triangleShader.setFloat("material.shininess", material.shininess);

            triangleShader.setVec3("light.position", light.position);
            triangleShader.setVec3("light.ambient",  light.ambient);
            triangleShader.setVec3("light.diffuse",  light.diffuse); // darken diffuse light a bit
            triangleShader.setVec3("light.specular", light.specular); 
            triangleShader.setFloat("light.constant", light.constant);
            triangleShader.setFloat("light.linear", light.linear);
            triangleShader.setFloat("light.quadratic", light.quadratic);


            lightShader.use();
            lightShader.setMat4("u_projection", proj);
            lightShader.setMat4("u_view", view);
            lightShader.setMat4("u_model", lightModel);
            lightShader.setVec3("u_lightColor", light.diffuse);





            renderTriangle(triangleShader, VAO, VBO, EBO, texture1, texture2);

            renderLight(lightShader, lightVAO, lightVBO);

            // IMGUI RENDERING
            {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                static float f = 0.0f;
                static int counter = 0;
                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
                ImGui::ColorEdit4("Clear Color: ", (float*)&clearColor);



            }

            {
                ImGui::Begin("Light Properties");

                ImGui::Text("Light");
                ImGui::SliderFloat3("Position", (float*)&light.position, -10.0f, 10.f);
                ImGui::ColorEdit3("Ambient", (float*)&light.ambient);
                ImGui::ColorEdit3("Diffuse", (float*)&light.diffuse);
                ImGui::ColorEdit3("Specular", (float*)&light.specular);
                ImGui::SliderFloat("Constant", (float*)&light.constant, 0.f, 10.f);
                ImGui::SliderFloat("Linear", (float*)&light.linear, 0.f, 1.f);
                ImGui::SliderFloat("Quadratic", (float*)&light.quadratic, 0.f, 1.f);

                ImGui::End();
            }

            {
                ImGui::Begin("Material");

                ImGui::Text(
                    "Material"
                );

                ImGui::ColorEdit3(
                    "Ambient", 
                    (float*)&material.ambient
                );

                ImGui::ColorEdit3(
                    "Diffuse", 
                    (float*)&material.diffuse
                );

                ImGui::ColorEdit3(
                    "Specular",
                    (float*)&material.specular
                );

                ImGui::SliderFloat(
                    "Shininess",
                    (float*)&material.shininess,
                    0.f,
                    100.f
                );


                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            
            glfwSwapBuffers(window);

            glfwPollEvents();

    }

    glDeleteBuffers(1,&VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteBuffers(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}

