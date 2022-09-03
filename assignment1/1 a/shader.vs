#version 460
in vec4 Position;

uniform mat4 gWorld;

void main()
{
    gl_Position = gWorld * vec4(Position);
}
