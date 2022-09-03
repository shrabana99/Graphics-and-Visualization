#version 460

in vec3 Position;
in vec3 Normal;

// out vec3 color;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position =  projection * view * world * vec4(  Position, 1.0);
    
    // color = Normal;
}
