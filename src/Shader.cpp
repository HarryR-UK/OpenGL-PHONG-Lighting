#include "Shader.h"
Shader::Shader(std::string vertSrcPath, std::string fragSrcPath)
{
    if(Shader::fileExists(vertSrcPath))
    {
        // shader source compile
        std::stringstream vertSrcStream = this->getSource(vertSrcPath);
        compileShader(vertSrcStream.str().c_str(), GL_VERTEX_SHADER);
    }
    else{
        // DEBUG LOG
        std::cout << "VERTEX SHADER DOES NOT EXIST IN THIS PATH!" << '\n';
    }

    if(Shader::fileExists(fragSrcPath))
    {
        std::stringstream fragSrcStream = this->getSource(fragSrcPath);
        compileShader(fragSrcStream.str().c_str(), GL_FRAGMENT_SHADER);

    }
    else{
        std::cout << "FRAGEMENT SHADER DOES NOT EXIST IN THIS PATH!" << '\n';
    }

    this->printDebugShader(this->vertID, GL_VERTEX_SHADER);
    this->printDebugShader(this->fragID, GL_FRAGMENT_SHADER);
    
    this->linkProgram();


}

void Shader::linkProgram()
{
    ID = glCreateProgram();
    glAttachShader(ID, vertID);
    glAttachShader(ID, fragID);
    glLinkProgram(ID);

    glDeleteShader(vertID);
    glDeleteShader(fragID);

    this->printDebugProgram();
}

std::stringstream Shader::getSource(std::string& path)
{
    std::stringstream src;
    std::string line;
    std::ifstream file(path);
    if(file.is_open())
    {
        while(std::getline(file, line))
        {
            src << line << '\n';
        }
        file.close();

    }

    return src;
}
void Shader::compileShader(const char* src, GLenum shaderType)
{
 

    switch(shaderType)
    {
        case GL_VERTEX_SHADER:
            this->vertID = glCreateShader(shaderType);
            glShaderSource(vertID, 1, &src , NULL);
            glCompileShader(vertID);
            break;
        case GL_FRAGMENT_SHADER:
            this->fragID = glCreateShader(shaderType);
            glShaderSource(fragID, 1, &src, NULL);
            glCompileShader(fragID);
            break;
        default:
            break;
    }




    

}

bool Shader::fileExists(std::string& path)
{
    struct stat buffer;
    return(stat (path.c_str(), &buffer) == 0);
}



void Shader::printDebugShader(unsigned int& shaderID, GLenum shaderType)
{
    int success;
    char log[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, log);
        switch(shaderType)
        {
            case GL_VERTEX_SHADER:
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_SHADER\n\n";
                break;
            case GL_FRAGMENT_SHADER:
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_SHADER\n\n";
                break;
            case GL_GEOMETRY_SHADER:
                std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_SHADER\n\n";
                break;
            default:
                break;
        }

        std::cout << log << '\n';
    }


}

void Shader::printDebugProgram()
{
    int success;
    char log[512];
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(this->ID, 512, NULL, log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log << '\n';

    }
}

 void Shader::use()
 {
    glUseProgram(ID);
 }


 // Uniform
 void Shader::setFloat(const std::string& name, float value) const
 {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
 }

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec2(const std::string& name, glm::vec2 value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::unbind()
{
    glUseProgram(0);
}
