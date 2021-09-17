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
