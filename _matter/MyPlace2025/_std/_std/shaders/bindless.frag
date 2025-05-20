#version 430 core

in vec2 TexCoord;
flat in uint instanceID;

out vec4 FragColor;

layout(binding = 0) buffer TextureHandles {
    uint64_t textureHandles[];   // Bindless texture handles
};

// Function to create a sampler2D from the bindless handle
sampler2D getTextureFromHandle(uint64_t handle) {
    return sampler2D(handle);
}

void main()
{
    // Retrieve the bindless texture handle for this instance
    uint64_t bindlessHandle = textureHandles[instanceID];
    
    // Create a sampler2D from the bindless handle and sample the texture
    sampler2D texture = getTextureFromHandle(bindlessHandle);
    FragColor = texture(texture, TexCoord);
}
