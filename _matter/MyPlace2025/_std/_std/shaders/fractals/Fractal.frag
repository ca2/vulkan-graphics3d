#version 330 core

in vec3 ourColor;       // Color passed from the vertex shader
in vec2 TexCoords;      // Texture coordinates passed from the vertex shader

out vec4 FragColor;     // Output color

uniform sampler2D texture1;  // The texture sampler
uniform float iTime;         // Time uniform for animations
uniform vec2 iResolution;    // Resolution uniform

// Color palette function (https://iquilezles.org/articles/palettes/)
vec3 palette(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    // Generate UV coordinates based on the texture coordinates and resolution
    vec2 fragCoord = TexCoords * iResolution;
    vec2 uv = (fragCoord * 2.0 - iResolution) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    // Loop for the effect (https://www.shadertoy.com/view/mtyGWy)
    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i * 0.4 + iTime * 0.4);

        d = sin(d * 8.0 + iTime) / 8.0;
        d = abs(d);
        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }
        
    FragColor = vec4(finalColor, 1.0);
}