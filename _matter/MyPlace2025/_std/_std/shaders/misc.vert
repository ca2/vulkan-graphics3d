#version 330 core

// This shader is designed to have a bunch of instanced quads face the cameras position in world space. this is just an implementaion of the billboarding affect in doomlikes


// Vertex attributes
layout (location = 0) in vec3 aPos;       // Vertex position
layout (location = 1) in vec3 aColor;     // Color
layout (location = 2) in vec2 aTexCoords; // Texture coordinates
layout (location = 3) in mat4 instanceMatrix; // Instanced transformation matrix

out vec3 ourColor;     // Pass color to the fragment shader
out vec2 TexCoords;    // Pass texture coordinates to the fragment shader

// Uniforms
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos; // Declare cameraPos as a uniform

void main() {
    // Calculate the position of the instance in world space (translation only)
    vec3 instancePos = instanceMatrix[3].xyz;

    // Calculate the direction from the instance to the camera
    vec3 toCamera = normalize(cameraPos - instancePos);

    // Create the right vector by taking the cross product of the camera direction and the up vector
    vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), toCamera)); // Assuming Y is up
    vec3 up = normalize(cross(toCamera, right)); // Re-calculate the up vector

    // Construct the final model matrix
    mat4 billboardMatrix = mat4(1.0);
    billboardMatrix[0].xyz = right; // Set right vector to first column
    billboardMatrix[1].xyz = up;    // Set up vector to second column
    billboardMatrix[2].xyz = toCamera; // Set forward vector to third column

    // Set the last column to maintain the original instance position
    billboardMatrix[3] = instanceMatrix[3]; // Keep the position from the instanceMatrix

    // Calculate the final position using the billboarded model matrix
    gl_Position = projection * view * billboardMatrix * vec4(aPos, 1.0);

    // Pass the color and texture coordinates to the fragment shader
    ourColor = aColor;
    TexCoords = aTexCoords;
}
