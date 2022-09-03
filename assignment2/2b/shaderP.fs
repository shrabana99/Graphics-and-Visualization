#version 460

in vec3 color;
out vec4 fColor;

uniform sampler1D colTexture;  
uniform sampler3D volumeTexture;
 

void main()
{
    vec4 sampled = texture(volumeTexture, color); 
    
    // fColor = vec4(sampled[0], 0, 0.5 - sampled[0], 0); 

    fColor = texture(colTexture, sampled[0]);

    // fColor =  vec4(color, 0);// 
    // vec4 col = ; 
    // fColor = texture(volumeTexture, color) ; // vec4(col[0], col[1], 1 - col[0], col[3]) ; //  * vec4(color, 1.0);  
    // vec4(150.0/255.0, 176.0/255.0, 157.0/255.0, 0.0);

}
