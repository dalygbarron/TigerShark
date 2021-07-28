#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Window.hpp>

/**
 * Where configuration options and shit go. At some point this file is likely
 * to become auto generated but it will require messing around with the meson
 * files which I do not want to do right now.
 */
namespace Config {
    int const WIDTH = 1024;
    int const HEIGHT = 600;

    /**
     * Creates a window with the settings that we need.
     * @param title is the title to give to the window.
     * @return the created window object which you must note is a pointer to
     *         something allocated on the heap.
     */
    sf::Window *createWindow(char const *title);
};

#endif
