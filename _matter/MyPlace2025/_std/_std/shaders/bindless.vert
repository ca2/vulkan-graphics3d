#version 430 core

layout(location = 0) in vec3 aPos;      // Vertex position
layout(location = 1) in vec2 aTexCoord; // Texture coordinates

layout(location = 2) in mat4 instanceModel;  // Model matrix for each instance

out vec2 TexCoord;
flat out uint instanceID;

uniform mat4 view;       // Camera view matrix
uniform mat4 projection; // Projection matrix

void main()
{
    // Apply the instance's model matrix and then the view and projection transformations
    gl_Position = projection * view * instanceModel * vec4(aPos, 1.0);
    
    // Pass the texture coordinates to the fragment shader
    TexCoord = aTexCoord;
    
    // Pass the instance ID for texture selection
    instanceID = gl_InstanceID;
}


