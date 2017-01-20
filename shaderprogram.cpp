#include <fstream>
#include <iostream>
#include <shaderprogram.hh>
#include <sstream>
#include <vector>

GLuint ShaderProgram::boundID = 0;

void ShaderProgram::bind() const
{
    if(*ID != boundID)
    {
        glUseProgram(*ID);
        boundID = *ID;
    }
}

GLint ShaderProgram::getUniLoc(const std::string& uniformName) const
{
    auto loc = uniformLocations.at(uniformName);
    return loc;
}

ShaderProgram::ShaderProgram(const std::string& name)
{
    auto vertexID = compile(loadSource(name + ".vs"), GL_VERTEX_SHADER);
    auto fragmentID = compile(loadSource(name + ".fs"), GL_FRAGMENT_SHADER);

    bool vertexErr = getError(false, *vertexID, GL_COMPILE_STATUS);
    bool fragmentErr = getError(false, *fragmentID, GL_COMPILE_STATUS);

    if(vertexErr || fragmentErr)
        throw std::runtime_error("compilation error: " + name);


    auto deleter = [](GLuint* ID)
    {
        glDeleteProgram(*ID);
        delete ID;
    };
    ID = std::unique_ptr<GLuint, decltype(deleter)>(new GLuint, deleter);
    *ID = glCreateProgram();

    glAttachShader(*ID, *vertexID);
    glAttachShader(*ID, *fragmentID);
    glLinkProgram(*ID);

    if(getError(true, *ID, GL_LINK_STATUS))
        throw std::runtime_error("linking error: " + name);

    glDetachShader(*ID, *vertexID);
    glDetachShader(*ID, *fragmentID);
    loadUniformLocations();
}

std::unique_ptr<GLuint, std::function<void(GLuint*)>> ShaderProgram::compile(const std::string& source,
                                                                             GLenum shaderType) const
{
                                                      auto deleter = [](GLuint* ID) {
    glDeleteShader(*ID);
    delete ID;
};

std::unique_ptr<GLuint, decltype(deleter)> shaderID(new GLuint, deleter);
*shaderID = glCreateShader(shaderType);

const GLchar* sourceGL = source.c_str();
glShaderSource(*shaderID, 1, &sourceGL, nullptr);
glCompileShader(*shaderID);
return std::move(shaderID);
}

std::string ShaderProgram::loadSource(const std::string& filename) const
{
    std::ifstream ifs(filename);
    if(!ifs)
        throw std::runtime_error("no file: " + filename);
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

bool ShaderProgram::getError(bool isProgram, GLuint ID, GLenum flag) const
{
    GLint success;
    if(isProgram)
        glGetProgramiv(ID, flag, &success);
    else
        glGetShaderiv(ID, flag, &success);

    if(success == GL_FALSE)
    {
        GLint length;
        if(isProgram)
            glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &length);
        else
            glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);

        std::vector<GLchar> infoLog(static_cast<std::size_t>(length));
        if(isProgram)
            glGetProgramInfoLog(ID, length, nullptr, infoLog.data());
        else
            glGetShaderInfoLog(ID, length, nullptr, infoLog.data());

        std::cout.write(infoLog.data(), static_cast<long>(infoLog.size()))
                << std::endl;
        return true;
    }
    return false;
}

void ShaderProgram::loadUniformLocations()
{
    GLint numUniforms;
    glGetProgramiv(*ID, GL_ACTIVE_UNIFORMS, &numUniforms);
    std::vector<char> uniformName(256);

    for(GLuint i = 0; i < static_cast<GLuint>(numUniforms); ++i)
    {
        GLint dum1;
        GLenum dum2;
        glGetActiveUniform(*ID, i, static_cast<GLsizei>(uniformName.size()),
                           nullptr, &dum1, &dum2, uniformName.data());
        GLint location = glGetUniformLocation(*ID, uniformName.data());

        uniformLocations[uniformName.data()] = location;
    }
}

ShaderMan::ShaderMan(const std::string& path):
    path(path)
{}

ShaderProgram& ShaderMan::get(const std::string& name)
{
    auto prog = shaderPrograms.find(name);
    if(prog == shaderPrograms.end())
    {
        if(!path.empty())
            return shaderPrograms.emplace(name, path + '/' + name).first->second;
        else
            return shaderPrograms.emplace(name, name).first->second;
    }
    return prog->second;
}
