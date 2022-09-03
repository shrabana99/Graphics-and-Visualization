#version 460

in vec3 gcolor;
out vec4 fColor;

// uniform sampler3D volumeTexture; 

void main()
{
    // fColor = vec4 ( texture(volumeTexture, gcolor).rrr, 0) ; 
    
    fColor =  vec4(gcolor, 1.0); 

    // fColor =  vec4(color, 0);// 
    // vec4 col = ; 
    // fColor = texture(volumeTexture, color) ; // vec4(col[0], col[1], 1 - col[0], col[3]) ; //  * vec4(color, 1.0);  
    // vec4(150.0/255.0, 176.0/255.0, 157.0/255.0, 0.0);

}
