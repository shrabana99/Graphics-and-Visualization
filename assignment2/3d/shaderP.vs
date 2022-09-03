#version 460

in vec3 Position;
out vec3 pos;

/*
uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;
*/

void main()
{
    gl_Position =   vec4( Position, 1.0);
    pos =   Position ; // + vec3(0.5, 0.5, 0.5);
}
