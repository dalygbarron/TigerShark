#include "Util.hh"

Util::Rect::Rect() {
    this->pos.x = 0;
    this->pos.y = 0;
    this->size.x = 0;
    this->size.y = 0;
}

Util::Rect::Rect(int x, int y, int w, int h) {
    this->pos.x = x;
    this->pos.y = y;
    this->size.x = w;
    this->size.y = h;
}

bool Util::Rect::xmlLoad(pugi::xml_node const &node) {
    pugi::xml_node x = node.child("x");
    pugi::xml_node y = node.child("y");
    pugi::xml_node w = node.child("w");
    pugi::xml_node h = node.child("h");
    if (x) {
        pugi::xml_text text = x.text();
        this->pos.x = text.as_int(this->pos.x);
    }
    if (y) {
        pugi::xml_text text = y.text();
        this->pos.y = text.as_int(this->pos.y);
    }
    if (w) {
        pugi::xml_text text = w.text();
        this->size.x = text.as_int(this->size.x);
    }
    if (h) {
        pugi::xml_text text = h.text();
        this->size.y = text.as_int(this->size.y);
    }
    return true;
}
