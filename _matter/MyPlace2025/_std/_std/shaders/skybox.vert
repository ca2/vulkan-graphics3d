
#version 330 core

layout (location = 0) in vec3 aPos; // Skybox positions

out vec3 TexCoords; // Pass to fragment shader

uniform mat4 projection;
uniform mat4 view;

void main() {
    TexCoords = aPos; // Skybox uses aPos directly as texture coordinates
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // Keep the z/w ratio 1:1 for a skybox
}
