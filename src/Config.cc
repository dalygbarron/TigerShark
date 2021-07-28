#include "Config.hh"

sf::Window *Config::createWindow(char const *title) {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Attribute::Core;
    sf::Window *window = new sf::Window(
        sf::VideoMode(Config::WIDTH, Config::HEIGHT),
        title,
        sf::Style::Default,
        settings
    );
    window->setVerticalSyncEnabled(true);
    window->setActive(true);
    return window;
}
