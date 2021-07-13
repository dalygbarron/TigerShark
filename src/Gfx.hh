#ifndef GFX_H
#define GFX_H

#include "Util.hh"
#include "gl.hh"
#include <SFML/Graphics.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <unordered_map>

/**
 * Checks for errors in opengl and logs them.
 * @param file is the file we are currently in.
 * @param line is the line of the file we are currently in.
 */
GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__) 
// TODO: only do this on debug builds maybe
#define gl(call) gl##call; glCheckError_(__FILE__, __LINE__)

/**
 * Where graphics related shit is stored.
 */
namespace Gfx {
    /**
     * A texture living on the graphics card.
     */
    class Texture: public IO::XmlLoad {
        public:
            /**
             * Destroys the texture.
             */
            ~Texture();

            /**
             * Destroys the texture and sets the id back to 0.
             */
            void free();

            /**
             * Gives you the current id of the gpu texture.
             * @return the texture id. If it's 0 there is no texture.
             */
            unsigned int getId() const;

            /**
             * Gives you the current size of the texture.
             * @return the width and height.
             */
            glm::uvec2 getSize() const;

            /**
             * Gives you the inverse of the dimensions of the texture as
             * a vector.
             * @return the inverse size. (1 / width, 1 / height).
             */
            glm::vec2 getFrom() const;

            /**
             * Creates the texture on the gpu.
             */
            bool loadFromFile(char const *filename);

            bool xmlLoad(pugi::xml_node const &node) override;

        private:
            unsigned int id = 0;
            glm::uvec2 size;
            glm::vec2 from;
    };

    /**
     * Texture atlas that stores sprites by name.
     */
    class Atlas: public IO::XmlLoad {
        public:
            /**
             * Destroys the atlas's texture which it created itself.
             */
            ~Atlas();

            /**
             * Destroys everything in the atlas without destroying it.
             */
            void free();

            /**
             * Gives you access to the atlas's texture so you can do stuff with
             * it.
             * @return a const reference to the texture. This reference doesn't
             *         change after you reload the atlas, but the stuff inside
             *         the texture will obviously.
             */
            Texture const &getTexture() const;

            /**
             * Gives you a sprite from the atlas.
             * @param name is the name of the sprite to find.
             * @return the found sprite's rectangle inside the sprite, or
             *         a rectangle of (0, 0, 0, 0) if you give an unknown
             *         sprite name.
             */
            Util::Rect getSprite(char const *name) const;

            /**
             * Gives you an iterator to the start of the sprites.
             * @return the iterator.
             */
            std::unordered_map<std::string, Util::Rect>::const_iterator getSpritesBegin() const;

            /**
             * Gives you the iterator thing to the end of the sprites.
             * @return the end iterator bit.
             */
            std::unordered_map<std::string, Util::Rect>::const_iterator getSpritesEnd() const;

            bool xmlLoad(pugi::xml_node const &node) override;

        private:
            Texture texture;
            std::unordered_map<std::string, Util::Rect> sprites;
    };

    /**
     * Loads shaders and stores them.
     */
    class Shader {
        public:
            /**
             * Destroys whatever shader bits are currently present here.
             */
            ~Shader();

            /**
             * Returns the current shader program id that you use in gl stuff.
             * @return the id.
             */
            unsigned int getId() const;

            /**
             * Deletes the shader program if there is one and sets it to 0.
             */
            void free();

            /**
             * Loads a shader from a file. If this object already has one then
             * that one is deleted first.
             * @param frag is the fragment shader filename.
             * @param vert is the vertex shader filename.
             * @return true iff all went well.
             */
            bool loadFromFile(char const *frag, char const *vert);

            /**
             * Sets a float uniform.
             * @param name is the name of the uniform.
             * @param value is the value to set.
             */
            void setFloat(char const *name, float value);

            /**
             * Sets a vec2 uniform.
             * @param name is the name of the uniform.
             * @param value is the value to set.
             */
            void setVec2(char const *name, glm::vec2 value);

            /**
             * Sets a vec3 uniform.
             * @param name is the name of the uniform.
             * @param value is the value to set.
             */
            void setVec3(char const *name, glm::vec3 value);

            /**
             * Sets a vec4 uniform.
             * @param name is the name of the uniform.
             * @param value is the value to set.
             */
            void setVec4(char const *name, glm::vec4 value);

        private:
            unsigned int id = 0;
            std::unordered_map<std::string, unsigned int> uniforms;

            /**
             * Finds a uniform either in the list or in the shader.
             * @param name is the name of the uniform.
             * @return the id of the uniform or -1 if the shader isn't
             *         initialised.
             */
            int findUniform(char const *name);
    };

    /**
     * 2D Batched sprite renderer
     */
    class Batch2D {
        public:
            /**
             * @param texture is the texture the batch will use for rendering.
             * @param shader is the shader that the batch uses to draw.
             * @param max is the maximum number of sprites that the batch can
             *        render.
             */
            Batch2D(
                Gfx::Texture const &texture,
                Gfx::Shader &shader,
                unsigned int max
            );

            /**
             * Destroys the buffers in memory and the opengl things that were
             * created specifically for this batch. Does not destroy the
             * texture or shaders used tho.
             */
            ~Batch2D();

            /**
             * Clears the list of sprites to draw so we may begin again.
             */
            void clear();

            /**
             * Takes a rectangle out of the batch's source texture and draws it
             * to the given rectangle on the screen.
             * @param src is where to sample texture from.
             * @param dst is where to put texture.
             * @param colour is the colour to colour it with.
             */
            void add(Util::Rect src, Util::Rect dst, sf::Color colour);

            /**
             * Pushes everything to the graphics card and renders the lot to
             * the screen. If you have not cleared since last render it will be
             * mad.
             */
            void render() const;

        private:
            Gfx::Texture const &texture;
            Gfx::Shader &shader;
            mutable bool rendered;
            unsigned int n;
            unsigned int max;
            float *vertices;
            unsigned int vbo;
            unsigned int ebo;
            unsigned int vao;
    };

    /**
     * 3D Batched sprite renderer.
     */
    class Batch3D {

    };

    /**
     * Loads a shader from a string. (Not a shader program like the Shader
     * class represents, this is an actual shader like a fragment shader or
     * whatever).
     * @param data is the data string.
     * @param type is the type of shader to load.
     * @return the id of the shader. If it is 0 then something went wrong.
     */
    unsigned int shaderFromString(char const *data, GLenum type);
};

#endif
