#ifndef MESH_H
#define MESH_H


#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_utils.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;


struct Vertex {
    Vector3f position;
    Vector3f normal;
};

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    int verticeNo, indiceNo, edgeNo, sides; 
    // vector<Texture>      textures;
    unsigned int VAO;

    // max x,y and min x,y
    float maxX, maxY, maxZ, minX, minY, minZ; 

    void readMesh(const char * OffFile)
    {
        FILE * input = fopen( OffFile, "r");
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

            
            Vertex ver; 
            ver.position = Vector3f(x, y, z);  

            ver.normal = Vector3f( 0, 0, 0);
            vertices.push_back(ver); 
        }

        // Read the Polygons 
        for(int i = 0;i < indiceNo;i ++) {
            // No. of sides of the polygon (Eg. 3 => a triangle) 
            fscan = fscanf(input, "%d", &sides);
            // read the vertices that make up the polygon 
            if(sides == 4){
                // rectangle to lines
                int arr[sides]; 
                for(int j = 0; j < sides; j++) {
                    fscan = fscanf(input, "%d", &arr[j]);
                    // indices.push_back(v); 
                }
                for(int j = 0; j < sides; j++) {
                    indices.push_back(arr[j]); 
                    indices.push_back(arr[ (j+1)%sides ] );  
                }
                sides = 2;
            }
            else{ // triangle
                int tris[3]; 
                for(int j = 0, v; j < sides; j++) {
                    fscan = fscanf(input, "%d", &v);
                    indices.push_back(v); 
                    tris[j] = v; 
                }

                int A = tris[0], B = tris[1], C = tris[2];
                // compute the normal of each triangle and add it to each vertex
                // p := cross(B-A, C-A)
                Vector3f n = vertices[B].position - vertices[A].position; 
                n = n.Cross(vertices[C].position - vertices[A].position) ;

                for(int j = 0; j < 3; j++){
                    vertices[tris[j]].normal += n; 
                }

            }
        } // printf("%d ", indices.size() );

        // normalize the normal 
        for(int i = 0; i < verticeNo; i++){
            vertices[i].normal.Normalize();
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
        if(sides == 3 ){
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        }
        else if(sides == 2 ){
            glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        glBindVertexArray(0);
    }

private:
    // render data 
    unsigned int VBO, EBO, VBO1;

    // initializes all the buffer objects/arrays
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);	
        // 3 -> 3 dimension 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
       
        // vertex normal
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        
        glBindVertexArray(0);
    }

};


#endif





/*
            // read the vertices that make up the polygon 
            int tris[3]; 
            for(int j = 0, v; j < sides; j++) {
                fscan = fscanf(input, "%d", &v);
                indices.push_back(v); 
                tris[i] = v;
            }

            int A = tris[0], B = tris[1], C = tris[2];
            // compute the normal of each triangle and add it to each vertex
            // p := cross(B-A, C-A)
            Vector3f n = vertices[B].position - vertices[A].position; 
            n = n.Cross(vertices[C].position - vertices[A].position) ;

            for(int j = 0; j < 3; j++){
                vertices[tris[j]].normal += n; 
            }
        }

       /* // normalize the normal 
        for(int i = 0; i < verticeNo; i++){
            vertices[i].normal.Normalize();
        }*/ 

//        fclose(input);

