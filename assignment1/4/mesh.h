#ifndef MESH_H
#define MESH_H


#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_utils.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;


class Mesh {
public:
    // mesh Data
    vector<Vector3f>       vertices;
    vector<unsigned int> indices;
    int verticeNo, indiceNo, edgeNo; 
    // vector<Texture>      textures;
    unsigned int VAO;

    // max x,y and min x,y
    float maxX, maxY, maxZ, minX, minY, minZ; 
    float avgX, avgY, avgZ; 


    void readMesh(const char * OffFile)
    {
        FILE * input = fopen(OffFile, "r");
        int fscan; 

        // First line should be OFF 
        char type[3]; 
        fscan = fscanf(input, "%s", type);
        if(strcmp(type,"OFF")) {
            printf("Not a OFF file");
            exit(1);
        }

        // Read the no. of vertices, faces and edges 
        fscan = fscanf(input, "%d", &verticeNo);
        fscan = fscanf(input, "%d", &indiceNo);
        fscan = fscanf(input, "%d", &edgeNo);

        // Read the vertices' location
        for(int i = 0;i < verticeNo; i++) {
            float x, y, z;
            fscan = fscanf(input, "%f %f %f", &x,&y,&z);

            maxX = max(maxX, x);
            maxY = max(maxY, y);
            maxZ = max(maxZ, z);
            minX = min(minX, x);
            minY = min(minY, y);
            minZ = min(minZ, z);


            Vector3f ver = Vector3f(x, y, z); 
            vertices.push_back(ver); 
        }
        avgX = (maxX - minX)/2; 
        avgY = (maxY - minY)/2;
        avgZ = (maxZ - minZ)/2; 


        // Read the Polygons 
        for(int i = 0;i < indiceNo;i ++) {
            // No. of sides of the polygon (Eg. 3 => a triangle) 
            int sides; 
            fscan = fscanf(input, "%d", &sides);
        
            // read the vertices that make up the polygon 
            for(int j = 0, v; j < sides; j++) {
                fscan = fscanf(input, "%d", &v);
                indices.push_back(v); 
            }
        }

        fclose(input);

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw()  // Shader &shader
    {

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(0);
    }

private:
    // render data 
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f), (void*)0);
       
        glBindVertexArray(0);
    }
};


#endif

