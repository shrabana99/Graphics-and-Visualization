#version 460
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main()
{
    vec4 translation = vec4(0, -0.1, 0, 0); 
    float yaxis = 0.1; // plane parallel to xz plane

    float vy0 = gl_in[0].gl_Position[1]; 
    float vy1 = gl_in[1].gl_Position[1]; 
    float vy2 = gl_in[2].gl_Position[1]; 

    if(vy0 > yaxis && vy1 > yaxis && vy2 > yaxis){
        gl_Position = gl_in[0].gl_Position;
        EmitVertex();

        gl_Position = gl_in[1].gl_Position;
        EmitVertex();

        gl_Position = gl_in[2].gl_Position;
        
        EmitVertex();

        EndPrimitive();
    }

    else if(vy0 < yaxis && vy1 < yaxis && vy2 < yaxis){
        gl_Position = gl_in[0].gl_Position;
        gl_Position = gl_Position + translation; 
        EmitVertex();

        gl_Position = gl_in[1].gl_Position;
        gl_Position = gl_Position + translation; 

        EmitVertex();

        gl_Position = gl_in[2].gl_Position;
        gl_Position = gl_Position + translation; 
        
        EmitVertex();

        EndPrimitive();
    }   
}
