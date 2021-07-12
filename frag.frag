#version 330 core

in vec4 vertexColour;
in vec2 texCoord;
out vec4 FragColor;

uniform vec2 fromScreen;
uniform sampler2D tex;

void main() {
    FragColor = vertexColour * texture(tex, texCoord);
} 
