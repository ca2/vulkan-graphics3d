
#version 330 core

in vec3 ourColor;       // Color passed from the vertex shader
in vec2 TexCoords;      // Texture coordinates passed from the vertex shader

out vec4 FragColor;     // Output color

uniform sampler2D texture1;  // The texture sampler

void main() {
    // Sample the texture color and multiply it by the input color
    FragColor = texture(texture1, TexCoords);
}