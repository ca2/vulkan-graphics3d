#version 430 core

layout (local_size_x = 16, local_size_y = 16) in;
 
void main()
{
    // Perform your compute tasks here
    uint x = gl_GlobalInvocationID.x;
    uint y = gl_GlobalInvocationID.y;

    // Example: Perform some computation on each work item
    // (e.g., set a texture or buffer data based on x and y)
}
