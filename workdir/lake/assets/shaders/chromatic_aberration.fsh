#version 460 core

layout (binding = 0) uniform sampler2D uFramebuffer;

in vec2 vTexCoords;

out vec4 FragColor;

void main() {
    const float cStrength = 0.002;

    if (vTexCoords.x < cStrength || vTexCoords.x > 1.0 - cStrength ||
        vTexCoords.y < cStrength || vTexCoords.y > 1.0 - cStrength) {
        FragColor = texture(uFramebuffer, vTexCoords);
        return;
    }

    vec4 rvalue = texture(uFramebuffer, vTexCoords + vec2(cStrength, 0.0));
    vec4 gvalue = texture(uFramebuffer, vTexCoords);
    vec4 bvalue = texture(uFramebuffer, vTexCoords + vec2(-cStrength, 0.0));

    FragColor = vec4(rvalue.r, gvalue.g, bvalue.b, 1.0);
}
