#include "Gfx.hh"
#include "Util.hh"
#include "IO.hh"
#include "gl.hh"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

/**
 * Entrance to the program.
 * @return status code of trhe program.
 */
int main() {
    spdlog::info("Welcome to Tiger Shark Engine: Extreme Sex edition");
    sf::Clock clock;
    {
        // running the stuff.
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.antialiasingLevel = 0;
        settings.majorVersion = 3;
        settings.minorVersion = 3;
        settings.attributeFlags = sf::ContextSettings::Attribute::Core;
        sf::Window window(
            sf::VideoMode(800, 600),
            "Shoiirk",
            sf::Style::Default,
            settings
        );
        window.setVerticalSyncEnabled(true);
        window.setActive(true);
        glm::vec2 fromScreen(2.0 / 800, 2.0 / 600);
        // setting up more stuff
        Gfx::Shader shader;
        if (!shader.loadFromFile("frag.frag", "vert.vert")) goto end;
        Gfx::Atlas atlas;
        if (!IO::xmlLoadFromFile(atlas, "atlas.xml")) goto end;
        shader.setVec2("fromScreen", fromScreen);
        shader.setVec2("fromTexture", atlas.getTexture().getFrom());
        window.setActive(true);
        Gfx::Batch2D batch(atlas.getTexture(), shader, 5000);
        // Play some nice music
        sf::Music music;
        if (music.openFromFile("ging.ogg")) {
            music.play();
        }
        // main loop
        while (true) {
            // Handle events.
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    goto end;
                } else if (event.type == sf::Event::Resized) {
                    gl(Viewport(0, 0, event.size.width, event.size.height));
                }
            }
            // Check for opengl errors.
            gl(ClearColor(0.2f, 0.3f, 0.8f, 1.0f));
            gl(Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            batch.clear();
            Util::Rect src;
            Util::Rect dst;
            sf::Color colour;
            for (int i = 0; i < 1000; i++) {
                for (auto it = atlas.getSpritesBegin();
                    it != atlas.getSpritesEnd();
                    ++it
                ) {
                    src = it->second;
                    dst.pos.x = rand() % 775;
                    dst.pos.y = rand() % 575;
                    dst.size.x = rand() % 50;
                    dst.size.y = rand() % 50;
                    colour.r = rand() % 255;
                    colour.g = rand() % 255;
                    colour.b = rand() % 255;
                    colour.a = 255;
                    batch.add(src, dst, colour);
                }
            }
            batch.render();
            window.display();
        }
    }
    end:
        spdlog::info("goodbye");
        return 0;
}
