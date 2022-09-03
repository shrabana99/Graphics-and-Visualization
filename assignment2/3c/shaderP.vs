#version 460

in vec3 Position;
in vec3 Color;

out vec3 color;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 pos = vec3( Position[0]/256, Position[1]/256, Position[2]/178); 
    pos = pos - vec3(0.5, 0.5, 0.5); 
    gl_Position =  projection * view * world * vec4( pos,  1.0);
    color =   Color; // Position + vec3(0.5, 0.5, 0.5);
}
