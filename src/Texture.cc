#include "src/Gfx.hh"
#include <spdlog/spdlog.h>
#include <iostream>

Gfx::Texture::~Texture() {
    this->free();
}

void Gfx::Texture::free() {
    if (this->id != 0) glDeleteTextures(1, &(this->id));
    this->id = 0;
    this->size.x = 0;
    this->size.y = 0;
    this->from.x = 1;
    this->from.y = 1;
}

unsigned int Gfx::Texture::getId() const {
    return this->id;
}

glm::uvec2 Gfx::Texture::getSize() const {
    return this->size;
}

glm::vec2 Gfx::Texture::getFrom() const {
    return this->from;
}

void Gfx::Texture::create(unsigned int width, unsigned int height) {
    this->free();
    this->size.x = width;
    this->size.y = height;
    this->from.x = 1.0 / width;
    this->from.y = 1.0 / height;
    gl(GenTextures(1, &(this->id)));
    gl(BindTexture(GL_TEXTURE_2D, this->id));
    gl(TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));       
    gl(TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    gl(TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    gl(TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    gl(BindTexture(GL_TEXTURE_2D, 0));
}

bool Gfx::Texture::loadFromFile(char const *filename) {
    this->free();
    sf::Image image;
    if (!image.loadFromFile(filename)) {
        spdlog::error("couldn't load texture from file {}", filename);
        return false;
    }
    image.flipVertically();
    sf::Vector2u size = image.getSize();
    this->create(size.x, size.y);
    gl(BindTexture(GL_TEXTURE_2D, this->id));
    gl(TexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        size.x,
        size.y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image.getPixelsPtr()
    ));
    gl(BindTexture(GL_TEXTURE_2D, 0));
    return true;
}
