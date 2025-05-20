#version 330 core

layout(location = 0) in vec3 aPos;

// Uniforms for view and projection matrices
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Apply the view and projection transformations
    gl_Position = projection * view * vec4(aPos, 1.0);
    gl_PointSize = 5.0; // Size of the point to make it visible
}
