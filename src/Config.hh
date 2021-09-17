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
    int const ATLAS_WIDTH = 2048;
    int const ATLAS_HEIGHT = 1024;
    extern char const *VERSION;

    /**
     * Creates a window with the settings that we need.
     * @param title is the title to give to the window.
     * @return pointer to the created window.
     */
    sf::Window *createWindow(char const *title);
};

#endif
