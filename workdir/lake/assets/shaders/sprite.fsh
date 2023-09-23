#version 460 core

layout (binding = 0) uniform sampler2D uTexture;

in vec2 vTexCoords;

out vec4 FragColor;

void main() {
    if (texture(uTexture, vTexCoords).a < 0.1) {
        discard;
    }

    FragColor = texture(uTexture, vTexCoords);
}
