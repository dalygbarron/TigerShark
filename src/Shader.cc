#include "Gfx.hh"
#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>
#include <sstream>

Gfx::Shader::~Shader() {
    this->free();
}

unsigned int Gfx::Shader::getId() const {
    return this->id;
}

void Gfx::Shader::free() {
    if (this->id != 0) gl(DeleteProgram(this->id));
    this->id = 0;
}

bool Gfx::Shader::loadFromFile(char const *frag, char const *vert) {
    // TODO: delete existing stuff if there is any.
    std::stringstream fragBuffer;
    std::stringstream vertBuffer;
    std::ifstream fileStream;
    fileStream.open(frag);
    if (fileStream.fail()) {
        spdlog::error("Could not open frag shader '{}'", frag);
        return false;
    }
    fragBuffer << fileStream.rdbuf();
    fileStream.close();
    unsigned int fragShader = Gfx::shaderFromString(
        fragBuffer.str().c_str(),
        GL_FRAGMENT_SHADER
    );
    if (fragShader == 0) {
        spdlog::error("error above for frag shader '{}'", frag);
        return false;
    }
    fileStream.open(vert);
    if (fileStream.fail()) {
        spdlog::error("Could not open vert shader '{}'", vert);
        gl(DeleteShader(fragShader));
        return false;
    }
    vertBuffer << fileStream.rdbuf();
    fileStream.close();
    unsigned int vertShader = Gfx::shaderFromString(
        vertBuffer.str().c_str(),
        GL_VERTEX_SHADER
    );
    if (vertShader == 0) {
        spdlog::error("error above for vert shader '{}'", frag);
        gl(DeleteShader(fragShader));
        return false;
    }
    this->id = gl(CreateProgram());
    gl(AttachShader(this->id, fragShader));
    gl(CheckError());
    gl(AttachShader(this->id, vertShader));
    gl(LinkProgram(this->id));
    gl(DeleteShader(fragShader));
    gl(DeleteShader(vertShader));
    int success;
    gl(GetProgramiv(this->id, GL_LINK_STATUS, &success));
    if (!success) {
        char infoLog[512];
        gl(GetProgramInfoLog(this->id, 512, NULL, infoLog));
        spdlog::error(
            "opengl error linking frag '{}' and vert '{}': {}",
            frag,
            vert,
            infoLog
        );
        this->id = 0;
        return false;
    }
    return true;
}

void Gfx::Shader::setFloat(char const *name, float value) {
    unsigned int location = this->findUniform(name);
    if (location == -1) return;
    gl(Uniform1f(location, value));
}

void Gfx::Shader::setVec2(char const *name, glm::vec2 value) {
    unsigned int location = this->findUniform(name);
    if (location == -1) return;
    gl(UseProgram(this->id));
    gl(Uniform2f(location, value.x, value.y));
    gl(UseProgram(0));
}

void Gfx::Shader::setVec3(char const *name, glm::vec3 value) {
    unsigned int location = this->findUniform(name);
    if (location == -1) return;
    gl(UseProgram(this->id));
    gl(Uniform3f(location, value.x, value.y, value.z));
    gl(UseProgram(0));
}

void Gfx::Shader::setVec4(char const *name, glm::vec4 value) {
    unsigned int location = this->findUniform(name);
    if (location == -1) return;
    gl(UseProgram(this->id));
    gl(Uniform4f(location, value.x, value.y, value.z, value.w));
    gl(UseProgram(0));
}

int Gfx::Shader::findUniform(char const *name) {
    if (this->id == 0) {
        spdlog::error("cannot set uniform '{}' on uninitialised shader", name);
        return -1;
    }
    if (!this->uniforms.contains(name)) {
        this->uniforms[name] = gl(GetUniformLocation(this->id, name));
    }
    return this->uniforms[name];
}
