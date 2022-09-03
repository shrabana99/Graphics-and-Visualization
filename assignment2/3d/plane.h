#ifndef PLANE_H
#define PLANE_H


#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_utils.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;


class Plane {
public:
    // plane
    float zConstant = 0.7; // -0.5 to 0.5
    float z = 89.0; 

    // mesh Data
    vector<Vector3f>       vertices;
    vector<unsigned int> indices;
    int verticeNo, indiceNo, edgeNo, sides; 

    unsigned int VAO, textureID, colTextureID;

    int cubeX = 5, cubeY = 5, cubeZ = 5;
    vector<unsigned char> scalar;

    void readMesh(const char * OffFile)
    {
        // read file
        FILE * input = fopen( OffFile, "r");
        int fscan; 
        // Read the no. of vertices, Given an axis parallel plane, define a slice then compute and display the scalar field restricted
        fscan = fscanf(input, "%d", &cubeX);
        fscan = fscanf(input, "%d", &cubeY);
        fscan = fscanf(input, "%d", &cubeZ);  // cout << x << y << z;

        // Read the vertices' location
        for(int i = 0, a;i < cubeX * cubeY * cubeZ; i++) {
            fscan = fscanf(input, "%d", &a); 
            scalar.push_back(a); 
        } 
        fclose(input); 
        // create  texture
        createTexture(); 

        for(int i = 0; i < cubeX-1; i++){
            for(int j = 0; j < cubeY-1; j++){
                for(int k = 0; k < cubeZ-1; k++){
                    Vector3f ver = Vector3f(i, j, k);
                    vertices.push_back(ver);
                }
            }
        }

        // cout << vertices.size();

        // now that we have aldl the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw()  // Shader &shader
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, textureID); 
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, vertices.size());
        glBindVertexArray(0);
    }

private:
    // render data 
    unsigned int VBO, EBO, VBO1;

    // initializes all the buffer objects/arrays
    void createTexture(){
        // volume texture
        glEnable(GL_TEXTURE_3D);
        glGenTextures(1, &textureID); 
        glBindTexture(GL_TEXTURE_3D, textureID);  
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T,   GL_CLAMP);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R,    GL_CLAMP);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, 				GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER,   GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 4);
        
        glTexImage3D(GL_TEXTURE_3D,0,GL_RED,cubeX, cubeY, cubeZ,0,GL_RED,GL_UNSIGNED_BYTE,&scalar[0]);
        glGenerateMipmap(GL_TEXTURE_3D); 
       
    }

    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3f), &vertices[0], GL_STATIC_DRAW);  

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);	
        // 3 -> 3 dimension 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)0);
        
        glBindVertexArray(0);
    }
};


#endif

