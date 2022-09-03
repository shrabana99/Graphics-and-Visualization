#version 460

// in vec3 color;
out vec4 fColor;

// uniform sampler3D volume;

void main()
{
    // fColor =  vec4(color, 0);// 
    // vec4 col = texture(volume, color); 
    // fColor =  vec4(col[0], col[1], 1 - col[0], col[3]) ; //  * vec4(color, 1.0);  
    fColor = vec4(150.0/255.0, 176.0/255.0, 157.0/255.0, 0.0);

}
