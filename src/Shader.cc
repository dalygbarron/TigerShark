#include "Gfx.hh"
#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>
#include <sstream>

char const *Gfx::Shader::QUERY = 
    "SELECT frag.label, frag.data, vert.label, vert.data "
    "FROM shader "
    "LEFT JOIN file frag "
    "ON frag.id = shader.frag "
    "LEFT JOIN file vert "
    "ON vert.id = shader.vert "
    "WHERE shader.label = ?1;";

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

bool Gfx::Shader::load(IO::DB &db, char const *name) {
    this->free();
    unsigned int fragShader = 0;
    unsigned int vertShader = 0;
    sqlite3_stmt *query = db.prepare(Gfx::Shader::QUERY);
    sqlite3_bind_text(query, 1, name, -1, SQLITE_STATIC);
    bool found = false;
    db.exec(query, [query, &fragShader, &vertShader, &found]() mutable {
        found = true;
        char const *fragName = (char const *)sqlite3_column_text(query, 0);
        char const *fragText = (char const *)sqlite3_column_text(query, 1);
        char const *vertName = (char const *)sqlite3_column_text(query, 2);
        char const *vertText = (char const *)sqlite3_column_text(query, 3);
        fragShader = Gfx::shaderFromString(fragText, GL_FRAGMENT_SHADER);
        if (fragShader == 0) {
            spdlog::error("error in fragmentShader {}", fragName);
            return;
        }
        vertShader = Gfx::shaderFromString(vertText, GL_VERTEX_SHADER);
        if (vertShader == 0) {
            spdlog::error("error in vertex shader {}", vertName);
            gl(DeleteShader(fragShader));
            return;
        }
    });
    if (!found) {
        spdlog::error("could not find shader program {}", name);
        return false;
    }
    if (fragShader == 0 || vertShader == 0) return false;
    this->id = gl(CreateProgram());
    gl(AttachShader(this->id, fragShader));
    gl(AttachShader(this->id, vertShader));
    gl(LinkProgram(this->id));
    gl(DeleteShader(fragShader));
    gl(DeleteShader(vertShader));
    int success;
    gl(GetProgramiv(this->id, GL_LINK_STATUS, &success));
    if (!success) {
        char infoLog[512];
        gl(GetProgramInfoLog(this->id, 512, NULL, infoLog));
        spdlog::error("opengl error linking {}: {}", name, infoLog);
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
