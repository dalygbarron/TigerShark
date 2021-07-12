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

bool Gfx::Atlas::xmlLoad(pugi::xml_node const &node) {
    pugi::xml_node picNode = node.child("pic");
    if (!picNode) {
        spdlog::error("atlas node has no pic node");
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
            spdlog::warn("sprite with no name in atlas");
            continue;
        }
        this->sprites[name] = rect;
    }
    return true;
}
