#version 460
in vec4 Position;

uniform mat4 gWorld[2];

void main()
{
    gl_Position = gWorld[gl_InstanceID % 2] * vec4( Position ); 
}
