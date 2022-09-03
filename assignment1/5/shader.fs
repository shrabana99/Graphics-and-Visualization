#version 460

in vec4 color;
out vec4 fColor;


void main()
{
    // gl_FragColor = vec4(150.0/255.0, 176.0/255.0, 157.0/255.0, 0.0);
    fColor = color; 
}
