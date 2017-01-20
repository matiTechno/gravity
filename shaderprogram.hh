#ifndef SHADERPROGRAM_HH
#define SHADERPROGRAM_HH

#include <string>
#define GLEW_NO_GLU
#include <GL/glew.h>
#include <unordered_map>
#include <memory>

// supported filenames: *.vs, *.fs
class ShaderProgram
{
public:
    // filename without extension
    ShaderProgram(const std::string& name);

    void bind() const;
    GLint getUniLoc(const std::string& uniformName) const;

private:
    static GLuint boundID;
    std::unique_ptr<GLuint, std::function<void(GLuint*)>> ID;
    std::unordered_map<std::string, GLint> uniformLocations;

    std::unique_ptr<GLuint, std::function<void(GLuint*)>> compile(const std::string& source,
                                                                  GLenum shaderType) const;
    std::string loadSource(const std::string& filename) const;
    bool getError(bool isProgram, GLuint ID, GLenum flag) const;
    void loadUniformLocations();
};

class ShaderMan
{
public:
    ShaderMan(const std::string& path);
    ShaderMan() = default;
    ShaderMan(const ShaderMan&) = delete;
    ShaderMan& operator=(const ShaderMan&) = delete;

    ShaderProgram& get(const std::string& name);

private:
    std::unordered_map<std::string, ShaderProgram> shaderPrograms;
    std::string path;
};

#endif // SHADERPROGRAM_HH
