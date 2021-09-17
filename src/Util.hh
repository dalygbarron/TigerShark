#ifndef UTIL_H
#define UTIL_H

#include "IO.hh"
#include <glm/vec2.hpp>

/**
 * Namespace for useful stuff that doesn't strictly fit one domain.
 */
namespace Util {
    /**
     * Represents a rectangle.
     */
    class Rect {
        public:
            glm::ivec2 pos;
            glm::ivec2 size;

            /**
             * default constructor.
             */
            Rect();

            /**
             * Defining all components.
             * @param x is the x value.
             * @param y is the y value.
             * @param w is the w value.
             * @param h is the h value.
             */
            Rect(int x, int y, int w, int h);
    };
};

#endif
