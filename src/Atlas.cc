#include "Gfx.hh"
#include <spdlog/spdlog.h>

char const *Gfx::Atlas::QUERY = "SELECT * FROM sprite WHERE image = ?";

Gfx::Atlas::~Atlas() {
    this->free();
}

void Gfx::Atlas::free() {
    this->texture.free();
    this->sprites.clear();
}

Gfx::Texture const &Gfx::Atlas::getTexture() const {
    return this->texture;
}

Util::Rect Gfx::Atlas::getSprite(char const *name) const {
    if (this->sprites.contains(name)) {
        return this->sprites.at(name);
    } else {
        Util::Rect rect;
        return rect;
    }
}

std::unordered_map<std::string, Util::Rect>::const_iterator
    Gfx::Atlas::getSpritesBegin() const
{
    return this->sprites.cbegin();
}

std::unordered_map<std::string, Util::Rect>::const_iterator
    Gfx::Atlas::getSpritesEnd() const
{
    return this->sprites.cend();
}

bool Gfx::Atlas::load(IO::DB &db, char const *file) {
    this->free();

}
