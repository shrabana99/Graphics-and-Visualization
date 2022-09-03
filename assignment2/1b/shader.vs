#version 460

in vec3 Position;
in vec3 Normal;
// in vec3 Color;

out vec3 fPosition; 
out vec3 fNormal;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // we read the multiplication from right to left
    gl_Position =  projection * view * world * vec4( Position, 1.0);
    fNormal = Normal;
    fPosition = vec3( world * vec4( Position, 1.0) );
}
