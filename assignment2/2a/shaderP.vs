#version 460

in vec3 Position;
in vec3 Color;

out vec3 color;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position =  projection * view * world * vec4( Position, 1.0);
    color =   Color; // Position + vec3(0.5, 0.5, 0.5);
}
