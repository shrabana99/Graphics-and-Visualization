#ifndef PLANE_H
#define PLANE_H


#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_utils.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;


struct Vertex {
    Vector3f Position;
    Vector3f Scalar;
};


class Plane {
public:
    // plane
    float z = 89; 

    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    int verticeNo, indiceNo, edgeNo, sides; 

    unsigned int VAO;

    int cubeX = 5, cubeY = 5, cubeZ = 5;
    vector<unsigned int> scalar;

    void readMesh(const char * OffFile)
    {
        
        // cout << OffFile;
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
        
        
        for(int y = -cubeY; y < cubeY ; y++){
            for(int x = -cubeX; x < cubeX ; x++){
                Vertex ver; 
                ver.Position = Vector3f( (float) x/ (cubeX * 2), (float) y/ (cubeY * 2), (z*2 - cubeZ) / (cubeZ * 2) );  
                
                float scalarVal = computeScalar(ver.Position) / 256 ; 

                // ver.Position.Print(); cout << endl; 

                ver.Scalar = Vector3f( 1 - scalarVal , 0 , scalarVal );
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

        // cout << scalar.size(); 

        // now that we have aldl the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw()  // Shader &shader
    {

        // draw mesh
        glBindVertexArray(VAO);
        
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(0);
    }

private:
    // render data 
    unsigned int VBO, EBO, VBO1;

    float findScalar(int x, int y, int z){
        return scalar[z * cubeY * cubeX +  y * cubeX + x ]; 
    }

    float computeScalar(Vector3f pos){

        float intPol = 0.0;

        // remap point coordinates to grid coordinates
        float x = pos.x * cubeX + cubeX/2; 
        float y = pos.y * cubeY + cubeY/2; 
        float z = pos.z * cubeZ + cubeZ/2; 

        // the cube's coordinates
        int x_c = ceil(x), x_f = floor(x);
        int y_c = ceil(y), y_f = floor(y);
        int z_c = ceil(z), z_f = floor(z);

        float xd = x - x_f, yd = y - y_f, zd = z - z_f;


        float c[8]; int l = 0;
        for(int i = 0; i <= 1; i++ ){
            for(int j = 0; j <= 1; j++){
                for(int k = 0; k <= 1; k++){
                    c[l++] = findScalar( (i == 0 ? x_f : x_c), (j == 0 ? y_f : y_c), ( k== 0 ? z_f : z_c) ); 
                }
            }
        }

        float c1[4]; l = 0; 
        for(int i = 0; i < 4; i++){
            c1[l++] = c[i] * (1 - xd) + c[i+4] * xd; 
        }

        float c2[2]; l = 0; 
        for(int i = 0; i < 2; i++){
            c2[l++] = c1[i] * (1 - yd) + c1[i+1] * yd; 
        }

        intPol = c2[0] * (1 - zd) + c2[1] * zd;

        return intPol;
    }

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
       
        // vertex scalar
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Scalar));
        
        glBindVertexArray(0);
    }
};


#endif

