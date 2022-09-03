#version 460

in vec3 Position;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;  

void main()
{
    gl_Position =  projection * view * world * vec4( Position, 1.0);
}
