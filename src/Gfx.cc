#include "Gfx.hh"
#include <spdlog/spdlog.h>

GLenum glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        std::cout << error << " | " << file << " (" << line << ")" <<
            std::endl;
    }
    return errorCode;
}

unsigned int Gfx::shaderFromString(char const *data, GLenum type) {
    unsigned int shader = gl(CreateShader(type));
    gl(ShaderSource(shader, 1, &data, NULL));
    gl(CompileShader(shader));
    int success;
    char infoLog[512];
    gl(GetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success) {
        gl(GetShaderInfoLog(shader, 512, NULL, infoLog));
        spdlog::error("error compiling shader: {}", infoLog);
        return 0;
    }
    return shader;
}
