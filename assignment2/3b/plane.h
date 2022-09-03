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

        /*

        Vector3f ver = Vector3f(-0.5, -0.5, z);   
        vertices.push_back(ver); 
        ver = Vector3f(0.5, -0.5, z);   
        vertices.push_back(ver); 
        ver = Vector3f(-0.5, 0.5, z);   
        vertices.push_back(ver); 
        ver = Vector3f(0.5, 0.5, z);   
        vertices.push_back(ver); 

        indices = { 0, 1, 2, 2, 1, 3 }; 

        */


        for(int y = -cubeY; y < cubeY ; y++){
            for(int x = -cubeX; x < cubeX ; x++){
                Vector3f ver; 
                ver = Vector3f( (float) x/ (cubeX * 2), (float) y/ (cubeY * 2), (z*2 - cubeZ) / (cubeZ * 2) );  
                vertices.push_back(ver); 
            }
        }

        // cout << computeScalar(Vector3f (0, 0, 0 ) ); 

        for(int y = 0; y <  2 * cubeY-1; y++){
            for(int x = 0; x <  2 * cubeX-1; x++){
                
                int i0 = (y) * (2 * cubeX ) + x; 
                int i1 = i0 + 1; 
                int i2 = i0 + (2 * cubeX ); 
                int i3 = i2 + 1; //  cout << i0 << " " << i2 << " " << i1 << endl; 

                indices.push_back(i0); 
                indices.push_back(i2); 
                indices.push_back(i1); 

                indices.push_back(i1); 
                indices.push_back(i2); 
                indices.push_back(i3); 
            } 
        }





        // now that we have aldl the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw()  // Shader &shader
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_3D, textureID); 

        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3f), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);	
        // 3 -> 3 dimension 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)0);
        
        glBindVertexArray(0);
    }
};


#endif

