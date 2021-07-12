#ifndef TEST_H
#define TEST_H

#include <catch2/catch.hpp>

#define TMP(...) tempFile(#__VA_ARGS__)
#define IN_WINDOW(content) {sf::ContextSettings settings;\
        settings.depthBits = 24;\
        settings.stencilBits = 8;\
        settings.antialiasingLevel = 0;\
        settings.majorVersion = 3;\
        settings.minorVersion = 3;\
        settings.attributeFlags = sf::ContextSettings::Attribute::Core;\
        sf::Window window(\
            sf::VideoMode(800, 600),\
            "unit test",\
            sf::Style::Default,\
            settings\
        );\
        window.setVerticalSyncEnabled(true);\
        window.setActive(true);\
        {content}}

/**
 * Creates a file in a temp file directory, writes a string to it, closes it,
 * and then returns the path to it.
 * @param content is the content to write into the file.
 * @return the filename of the new file. I guess if something fails then it
 *         will return null but it really shouldn't. It could though so just
 *         assert not null first to avoid seg faults.
 */
char const *tempFile(char const *content);

#endif
