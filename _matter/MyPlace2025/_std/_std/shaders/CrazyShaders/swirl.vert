
#version 330 core

layout (location = 0) in vec3 aPos;       // Position
layout (location = 1) in vec3 aColor;     // Color
layout (location = 2) in vec2 aTexCoords; // Texture coordinates
layout (location = 3) in mat4 instanceMatrix;

out vec3 ourColor;         // Pass color to the fragment shader
out vec2 TexCoords;        // Pass texture coordinates to the fragment shader
out float fresnelFactor;   // Fresnel edge factor for "glow" effect
out vec3 fragPosition;     // Fragment position in world space for effects
out vec3 fragNormal;       // Fragment normal in world space for effects

// Uniforms for transformations and effects
uniform mat4 view;
uniform mat4 projection;
uniform float time;        // Time variable for animated effects

// Function to generate pseudo-random noise based on position
float noise(vec3 p) {
    return fract(sin(dot(p, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
}

void main() {
    // Compute world position and add a noise-based displacement
    float noiseStrength = 0.2;    // Strength of the noise displacement
    vec3 noisyPos = aPos + vec3(noise(aPos + time), noise(aPos.yzx + time), noise(aPos.zxy + time)) * noiseStrength;

    // Apply twisting effect around the y-axis over time
    float twistAmount = 1.5;
    float twistAngle = twistAmount * noisyPos.y + time;
    mat4 twistRotation = mat4(
        cos(twistAngle), 0.0, sin(twistAngle), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(twistAngle), 0.0, cos(twistAngle), 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    // Compute final position with instance matrix, twist, and projection
    vec4 worldPosition = instanceMatrix * twistRotation * vec4(noisyPos, 1.0);
    gl_Position = projection * view * worldPosition;

    // Pass attributes to the fragment shader
    ourColor = aColor;
    TexCoords = aTexCoords;
    fragPosition = vec3(instanceMatrix * vec4(aPos, 1.0));
    fragNormal = vec3(instanceMatrix * vec4(0.0, 0.0, 1.0, 0.0)); // Assuming default z-up normals

    // Fresnel edge factor for glow effect at grazing angles
    vec3 viewDir = normalize(-fragPosition); 
    fresnelFactor = pow(1.0 - dot(viewDir, normalize(fragNormal)), 3.0);
}
