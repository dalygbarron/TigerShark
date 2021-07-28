#include "Gfx.hh"
#include <spdlog/spdlog.h>

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

std::unordered_map<std::string, Util::Rect>::const_iterator Gfx::Atlas::getSpritesBegin() const {
    return this->sprites.cbegin();
}

std::unordered_map<std::string, Util::Rect>::const_iterator Gfx::Atlas::getSpritesEnd() const {
    return this->sprites.cend();
}

bool Gfx::Atlas::xmlLoad(pugi::xml_node const &node) {
    pugi::xml_node picNode = node.child("pic");
    if (!picNode) {
        IO::logXmlError("atlas node has no pic node", node);
        return false;
    }
    bool result = this->texture.xmlLoad(picNode);
    if (!result) return false;
    for (pugi::xml_node sprite = node.child("sprite");
        sprite;
        sprite = sprite.next_sibling("sprite")
    ) {
        Util::Rect rect;
        rect.xmlLoad(sprite);
        char const *name = sprite.attribute("name").value();
        if (!name || name[0] == 0) {
            IO::logXmlError("sprite with no name in atlas", sprite);
            continue;
        }
        this->sprites[name] = rect;
    }
    return true;
}
