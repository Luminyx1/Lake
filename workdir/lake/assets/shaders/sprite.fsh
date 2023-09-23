#version 460 core

layout (binding = 0) uniform sampler2D uTexture;

in vec2 vTexCoords;

out vec4 FragColor;

void main() {
    FragColor = texture(uTexture, vTexCoords);
}
