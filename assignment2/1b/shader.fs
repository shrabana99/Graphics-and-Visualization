#version 460

in vec3 fNormal;
in vec3 fPosition;

out vec4 fColor;

uniform vec3 viewPos;  
uniform vec3 lightPos;  


void main()
{
    // vec3 lightPos = vec3(-5, 0, 0);

    vec3 lightDir = normalize(lightPos - fPosition);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    // vec3 viewPos = vec3(0.0f, 0.0f, 1.7f);  

    vec3 objectColor = vec3(1.0, 1.0, 1.0);

    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    
    // Diffuse lighting
    float diff = max(dot(fNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fPosition);
    vec3 reflectDir = reflect(-lightDir, fNormal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        

    vec3 result = (ambient + diffuse + specular) * objectColor;
    fColor = vec4(result, 1.0);


}
