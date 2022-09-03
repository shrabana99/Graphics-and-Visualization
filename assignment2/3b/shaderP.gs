/*#version 460

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 pos[];
out vec3 gcolor; 

uniform sampler3D volumeTexture;

void main()
{
    for(int i = 0; i < 3; i++){
        gcolor = vec3(texture(volumeTexture, pos[i]).r, 0, 0) ; 
        gl_Position = gl_in[i].gl_Position; 
        EmitVertex();
    }
    EndPrimitive();
}
*/


#version 460
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in vec3 pos[];
out vec3 gcolor; 

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

uniform sampler3D volumeTexture;

float isoVal = 50.0; 

vec3 findPoint(float c1, float c2, vec3 P1, vec3 P2) {
        vec3 P3 = P1 + (P2 - P1) * ((c1 - isoVal) / (c1 - c2) )   ;
        return P3;
}



void main()
{       
        vec3 i0 = vec3(gl_in[0].gl_Position);
        vec3 i1 = vec3(gl_in[1].gl_Position);
        vec3 i2 = vec3(gl_in[2].gl_Position);

        vec3 p1 , p2; 
        int flag = 0;

        float c0 =  texture(volumeTexture, pos[0]).r * 256 ;  // gcolor[0][0] * 256; 
        float c1 =  texture(volumeTexture, pos[1]).r * 256;  // gcolor[1][0] * 256;
        float c2 =  texture(volumeTexture, pos[2]).r * 256;  // gcolor[2][0] * 256;


        if(     (c0 > isoVal && c1 < isoVal && c2 < isoVal ) ||
                (c0 < isoVal && c1 > isoVal && c2 > isoVal )  ){
                p1 = findPoint(c0, c1, i0, i1);
                p2 = findPoint(c0, c2, i0, i2);
                flag = 1;
        }

        else if( (c1 > isoVal && c0 < isoVal && c2 < isoVal ) ||
                 (c1 < isoVal && c0 > isoVal && c2 > isoVal )  ){
                p1 = findPoint(c1, c0, i1, i0);
                p2 = findPoint(c1, c2, i1, i2);
                flag = 1;
        }
        else if( (c2 > isoVal && c1 < isoVal && c0 < isoVal ) ||
                 (c2 < isoVal && c1 > isoVal && c0 > isoVal )  ){
                p1 = findPoint(c2, c1, i2, i1);
                p2 = findPoint(c2, c0, i2, i0);
                flag = 1;
        }

        if(flag == 1) {
                gcolor = vec3(1, 1, 1); 
                gl_Position = projection * view * world * vec4( p1, 1.0);
                EmitVertex();   
                gcolor = vec3(1, 1, 1); 
                gl_Position = projection * view * world * vec4( p2, 1.0);;  
                EmitVertex();   
        }

        for(int i = 0; i < 3; i++){
            gcolor = vec3(texture(volumeTexture, pos[i]).r, 0, 1) ; 
            gl_Position = projection * view * world * gl_in[i].gl_Position; 
            EmitVertex();
        }
        
        EndPrimitive();

}
