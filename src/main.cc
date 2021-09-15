#include "Gfx.hh"
#include "Util.hh"
#include "IO.hh"
#include "Config.hh"
#include "gl.hh"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <spdlog/spdlog.h>
#include <iostream>

int main() {
    spdlog::info("Welcome to Tiger Shark Engine: Extreme Sex edition");
    sf::Clock clock;
    {
        // running the stuff.
        std::unique_ptr<sf::Window> window(Config::createWindow("Shoik"));
        glm::vec2 fromScreen(2.0 / Config::WIDTH, 2.0 / Config::HEIGHT);
        // setting up more stuff
        Gfx::Shader shader;
        Gfx::Atlas atlas;
        if (!shader.loadFromFile("frag.frag", "vert.vert")) goto end;
        if (!IO::xmlLoadFromFile(atlas, "sprites.xml")) goto end;
        shader.setVec2("fromScreen", fromScreen);
        shader.setVec2("fromTexture", atlas.getTexture().getFrom());
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
            while (window->pollEvent(event)) {
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
            window->display();
        }
    }
    end:
        spdlog::info("goodbye");
        return 0;
}
