#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec4 col;
out vec4 vertexColour;
out vec2 texCoord;

uniform vec2 fromScreen;
uniform vec2 fromTexture;

void main() {
    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
    gl_Position.xy = pos * fromScreen - vec2(1.0);
    vertexColour = col;
    texCoord = tex * fromTexture;
}
