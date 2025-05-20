#version 330 core

in vec3 ourColor;           // Color passed from the vertex shader
in vec2 TexCoords;          // Texture coordinates passed from the vertex shader
in float fresnelFactor;     // Fresnel factor for edge "glow"
in vec3 fragPosition;       // World-space position for chromatic aberration
in vec3 fragNormal;         // World-space normal for lighting effects

out vec4 FragColor;         // Output color

uniform sampler2D texture1; // Texture sampler
uniform vec3 gradientColor; // Uniform color for gradient overlay
uniform float time;         // Time variable for animated effects

void main() {
    // Sample the texture color
    vec4 texColor = texture(texture1, TexCoords);

    // Chromatic aberration based on world position and time
    vec2 chromaOffset = vec2(sin(fragPosition.x * 3.0 + time * 5.0), cos(fragPosition.y * 3.0 + time * 5.0)) * 0.005;
    vec4 chromaRed = texture(texture1, TexCoords + chromaOffset * 0.5);
    vec4 chromaGreen = texture(texture1, TexCoords - chromaOffset * 0.5);
    vec4 chromaBlue = texture(texture1, TexCoords + chromaOffset * 0.75);
    vec4 chromaColor = vec4(chromaRed.r, chromaGreen.g, chromaBlue.b, 1.0);

    // Fresnel edge glow effect for outer rim
    vec4 fresnelGlow = vec4(gradientColor * fresnelFactor * 0.8, 1.0);

    // Combine texture color, chromatic aberration, and fresnel glow
    vec4 baseColor = mix(texColor, chromaColor, 0.6) + fresnelGlow;

    // Final color output with a pulsating alpha channel
    float pulsate = 0.5 + 0.5 * sin(time * 5.0 + length(fragPosition) * 2.0);
    FragColor = baseColor * vec4(1.0, 1.0, 1.0, pulsate);
}
