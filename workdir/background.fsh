#version 460 core

uniform vec2 uResolution;
uniform float uTime;

out vec4 FragColor;

//https://iquilezles.org/articles/palettes/
vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 1.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(0.5, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b * sin(6.28318 * (c * t + d));
}

//https://www.shadertoy.com/view/mtyGWy
void main() {
    vec2 uv = (gl_FragCoord.xy * 2.0 - uResolution.xy) / uResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i * 0.4 + uTime * 0.4);

        d = sin(d * 8.0 + uTime) / 8.0;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }
    
    finalColor = pow(finalColor, vec3(0.4545)) - 0.2;

    FragColor = vec4(finalColor, 1.0);
}