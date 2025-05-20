
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform float time;  // Dynamic time for animation

// Function to create a 3D fractal noise pattern
float fractalNoise(vec3 pos, float frequency, float timeOffset) {
    float noise = 0.0;
    for (int i = 1; i < 5; ++i) {
        float f = float(i) * frequency;
        noise += sin(pos.x * f + time * timeOffset) * 0.5 / float(i);
        noise += sin(pos.y * f * 1.2 + time * timeOffset * 1.1) * 0.25 / float(i);
        noise += sin(pos.z * f * 1.4 + time * timeOffset * 1.2) * 0.125 / float(i);
    }
    return noise;
}

// Function to create a radial kaleidoscope effect
vec2 kaleidoscope(vec2 uv, float angleSlices) {
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);

    // Create mirrored sectors based on angleSlices
    float mirroredAngle = mod(angle, 2.0 * 3.14159 / angleSlices);
    mirroredAngle = abs(mirroredAngle - 3.14159 / angleSlices);

    return vec2(cos(mirroredAngle) * radius, sin(mirroredAngle) * radius);
}

// Generate some oscillating rainbow colors
vec3 rainbow(float x) {
    return vec3(
        0.5 + 0.5 * sin(x + 0.0),
        0.5 + 0.5 * sin(x + 2.0),
        0.5 + 0.5 * sin(x + 4.0)
    );
}

void main() {    
    // Step 1: Fetch the original skybox texture for a base
    vec4 texColor = texture(skybox, TexCoords);

    // Step 2: Normalize the texture coordinates for some extra math fun
    vec2 uv = TexCoords.xy / length(TexCoords); // Convert to 2D

    // Step 3: Apply kaleidoscope effect to warp the skybox with angular symmetry
    float numSlices = 10.0; // Increase this for more fractal-like effects
    uv = kaleidoscope(uv, numSlices);

    // Step 4: Introduce fractal noise that layers multiple noise patterns at varying frequencies
    float baseNoise = fractalNoise(vec3(uv, TexCoords.z), 5.0, 2.5);

    // Step 5: Create a recursive effect with multiple fractal layers, each one distorting space more
    vec3 distortedPos = TexCoords + vec3(sin(baseNoise), cos(baseNoise), tan(baseNoise)) * 0.2;

    // Step 6: Recursive self-reference for added brain-breaking complexity (feedback loop)
    vec4 recursiveTexColor = texture(skybox, distortedPos * fractalNoise(TexCoords, 10.0, 0.5));
    
    // Step 7: Time-distortion and circular warp: bend time itself and distort geometry
    float timeWarp = fractalNoise(TexCoords * 0.5, 8.0, 0.5) * time * 0.1;
    uv = uv * mat2(cos(timeWarp), -sin(timeWarp), sin(timeWarp), cos(timeWarp));
    
    // Step 8: Create a recursive time-driven vortex effect with additional time-based distortion
    float vortexRadius = length(uv) * 10.0;
    float vortexAngle = atan(uv.y, uv.x) + time * 5.0;
    uv = vec2(cos(vortexAngle), sin(vortexAngle)) * vortexRadius;

    // Step 9: Apply rainbow-like color shifting based on recursive distortions and fractal noise
    vec3 finalColor = rainbow(vortexRadius + time * 2.0) * recursiveTexColor.rgb;

    // Step 10: Mix the original texture with the recursively modified and time-warped colors
    FragColor = mix(texColor, vec4(finalColor, 1.0), 0.9);
    
    // Step 11: Overlay multiple levels of depth distortion using fractal iterations
    for (int i = 0; i < 5; ++i) {
        float layerDepth = float(i) * 0.2 + baseNoise * 0.05;
        vec3 depthPos = TexCoords + vec3(sin(layerDepth), cos(layerDepth), tan(layerDepth)) * 0.1;
        FragColor.rgb += texture(skybox, depthPos).rgb * 0.2 / float(i + 1);
    }
}
