#version 460

in vec3 Position;
in vec3 Color;

out vec4 color;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position =  projection * view * world * vec4( Position, 1.0);

    float x = gl_Position[2], colB = 0.8, colR = 0.2; 


    if( x <= 0.6 ){   // .2, .8
        colB -= 0.2; 
        colR += 0.2; 
    }
    if( x <= 0.2 ){ // .4, .6
        colB -= 0.2; 
        colR += 0.2; 
    }
    if( x <= -0.2 ){ // .6, .4
        colB -= 0.2; 
        colR += 0.2; 
    }
    if( x <= -0.6 ){  //  .8, .2
        colB -= 0.2;
        colR += 0.2;  
    }

    color = vec4( colR, 0, colB, 1);
}
