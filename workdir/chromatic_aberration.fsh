#version 460 core

layout (binding = 0) uniform sampler2D uFramebuffer;

in vec2 vTexCoords;

out vec4 FragColor;

void main() {
    const float cStrength = 0.002;

    vec4 rvalue = texture(uFramebuffer, vTexCoords + vec2(cStrength, 0.0));
    vec4 gvalue = texture(uFramebuffer, vTexCoords);
    vec4 bvalue = texture(uFramebuffer, vTexCoords + vec2(-cStrength, 0.0));

    FragColor = vec4(rvalue.r, gvalue.g, bvalue.b, 1.0);
}
