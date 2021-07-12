#include "Gfx.hh"
#include "Util.hh"
#include "gl.hh"
#include <spdlog/spdlog.h>
#include <iostream>

Gfx::Batch2D::Batch2D(
    Gfx::Texture &texture,
    Gfx::Shader &shader,
    unsigned int max
):
    texture(texture),
    shader(shader)
{
    int indices[6 * max];
    for (int i = 0; i < max; i++) {
        indices[i * 6] = i * 4;
        indices[i * 6 + 1] = i * 4 + 1;
        indices[i * 6 + 2] = i * 4 + 3;
        indices[i * 6 + 3] = i * 4 + 1;
        indices[i * 6 + 4] = i * 4 + 2;
        indices[i * 6 + 5] = i * 4 + 3;
    }
    this->max = max;
    this->n = 0;
    this->vertices = new float[24 * max];
    gl(GenVertexArrays(1, &(this->vao)));
    gl(BindVertexArray(this->vao));
    gl(GenBuffers(1, &(this->vbo)));
    gl(BindBuffer(GL_ARRAY_BUFFER, this->vbo));
    gl(BufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * 24 * max,
        this->vertices,
        GL_DYNAMIC_DRAW
    ));
    gl(VertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void *)0
    ));
    gl(EnableVertexAttribArray(0));
    gl(VertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void *)(2 * sizeof(float))
    ));
    gl(EnableVertexAttribArray(1));
    gl(VertexAttribPointer(
        2,
        4,
        GL_UNSIGNED_BYTE,
        GL_TRUE,
        6 * sizeof(float),
        (void *)(4 * sizeof(float))
    ));
    gl(EnableVertexAttribArray(2));
    gl(GenBuffers(1, &(this->ebo)));
    gl(BindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo));
    gl(BufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    ));
    gl(BindVertexArray(0));
    gl(BindBuffer(GL_ARRAY_BUFFER, 0));
    gl(BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

Gfx::Batch2D::~Batch2D() {
    gl(DeleteBuffers(1, &(this->vbo)));
    gl(DeleteBuffers(1, &(this->ebo)));
    gl(DeleteVertexArrays(1, &(this->vao)));
    delete[] this->vertices;
}

void Gfx::Batch2D::clear() {
    this->n = 0;
    this->rendered = false;
}

void Gfx::Batch2D::add(Util::Rect src, Util::Rect dst, sf::Color colour) {
    if (this->n == this->max) {
        spdlog::error("batch2d overflowing");
        return;
    }
    unsigned int offset = this->n * 24;
    unsigned char *bytes = (unsigned char *)this->vertices;
    this->vertices[offset] = dst.pos.x;
    this->vertices[offset + 1] = dst.pos.y;
    this->vertices[offset + 2] = src.pos.x;
    this->vertices[offset + 3] = src.pos.y;
    bytes[(offset + 4) * sizeof(float)] = colour.r;
    bytes[(offset + 4) * sizeof(float) + 1] = colour.g;
    bytes[(offset + 4) * sizeof(float) + 2] = colour.b;
    bytes[(offset + 4) * sizeof(float) + 3] = colour.a;
    this->vertices[offset + 6] = dst.pos.x + dst.size.x;
    this->vertices[offset + 7] = dst.pos.y;
    this->vertices[offset + 8] = src.pos.x + src.size.x;
    this->vertices[offset + 9] = src.pos.y;
    bytes[(offset + 10) * sizeof(float)] = colour.r;
    bytes[(offset + 10) * sizeof(float) + 1] = colour.g;
    bytes[(offset + 10) * sizeof(float) + 2] = colour.b;
    bytes[(offset + 10) * sizeof(float) + 3] = colour.a;
    this->vertices[offset + 12] = dst.pos.x + dst.size.x;
    this->vertices[offset + 13] = dst.pos.y + dst.size.y;
    this->vertices[offset + 14] = src.pos.x + src.size.x;
    this->vertices[offset + 15] = src.pos.y + src.size.y;
    bytes[(offset + 16) * sizeof(float)] = colour.r;
    bytes[(offset + 16) * sizeof(float) + 1] = colour.g;
    bytes[(offset + 16) * sizeof(float) + 2] = colour.b;
    bytes[(offset + 16) * sizeof(float) + 3] = colour.a;
    this->vertices[offset + 18] = dst.pos.x;
    this->vertices[offset + 19] = dst.pos.y + dst.size.y;
    this->vertices[offset + 20] = src.pos.x;
    this->vertices[offset + 21] = src.pos.y + src.size.y;
    bytes[(offset + 22) * sizeof(float)] = colour.r;
    bytes[(offset + 22) * sizeof(float) + 1] = colour.g;
    bytes[(offset + 22) * sizeof(float) + 2] = colour.b;
    bytes[(offset + 22) * sizeof(float) + 3] = colour.a;
    this->n++;
}

void Gfx::Batch2D::render() const {
    if (this->rendered) {
        spdlog::error("Rendering with batch2d without clear");
        return;
    }
    this->rendered = true;
    gl(BindBuffer(GL_ARRAY_BUFFER, this->vbo));
    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        sizeof(float) * this->n * 24,
        this->vertices
    );
    gl(BindBuffer(GL_ARRAY_BUFFER, 0));
    gl(BindVertexArray(this->vao));
    gl(BindTexture(GL_TEXTURE_2D, this->texture.getId()));
    gl(UseProgram(shader.getId()));
    gl(DrawElements(GL_TRIANGLES, this->n * 6, GL_UNSIGNED_INT, 0));
    gl(BindVertexArray(0));
    gl(BindTexture(GL_TEXTURE_2D, 0));
    gl(UseProgram(0));
}
