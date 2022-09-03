#ifndef ISOSURFACE_H
#define ISOSURFACE_H

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

class Surface {
public:
    float isoval = 30; 

    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    int verticeNo, indiceNo, edgeNo, sides; 

    unsigned int VAO, VBO;
    int cubeX = 5, cubeY = 5, cubeZ = 5;
    vector<unsigned int> scalar;

    void readMesh(const char * OffFile)
    {
        // cout << OffFile;
        FILE * input = fopen( OffFile, "r");
        int fscan; 

        fscan = fscanf(input, "%d", &cubeX);
        fscan = fscanf(input, "%d", &cubeY);
        fscan = fscanf(input, "%d", &cubeZ);  // cout << x << y << z;

        // Read the vertices' location
        for(int i = 0, a;i < cubeX * cubeY * cubeZ; i++) {
            fscan = fscanf(input, "%d", &a);
            scalar.push_back(a); 
        } 
        fclose(input); 

        for(int i = 0; i < cubeX ; i++){
            for(int j = 0; j < cubeY; j++){
                for(int k = 0; k < cubeZ; k++){
                    processCube((float)i, (float)j, (float)k); 
                }
            }
        }
        cout << vertices.size();
        setupMesh();
    }
    // render the mesh
    void Draw()  // Shader &shader
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
    }

private:

    //relative to vertex0, of each of the 8 vertices of a cube
    float vertexI[8][3] =
    {
            {0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{1.0, 1.0, 0.0},{0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0},{1.0, 0.0, 1.0},{1.0, 1.0, 1.0},{0.0, 1.0, 1.0}
    };

    //list of the index of the endpoint vertices for each of the 6 edges(2 triangle) of the tetrahedron
    int edgeConnectionInTetra[6][2] =
    {
            {0,1},  {1,2},  {2,0},  {0,3},  {1,3},  {2,3}
    };

    // coordinate of six tetrahedrons within the cube
    int tetraInCube[6][4] =
    {
            {0,5,1,6},
            {0,1,2,6},
            {0,2,3,6},
            {0,3,7,6},
            {0,7,4,6},
            {0,4,5,6}
    };

    // list the triangle possible for 16 different intersection
    int tetraTriangles[16][7] =
    {
            {-1, -1, -1, -1, -1, -1, -1},
            { 0,  3,  2, -1, -1, -1, -1},
            { 0,  1,  4, -1, -1, -1, -1},
            { 1,  4,  2,  2,  4,  3, -1},

            { 1,  2,  5, -1, -1, -1, -1},
            { 0,  3,  5,  0,  5,  1, -1},
            { 0,  2,  5,  0,  5,  4, -1},
            { 5,  4,  3, -1, -1, -1, -1},

            { 3,  4,  5, -1, -1, -1, -1},
            { 4,  5,  0,  5,  2,  0, -1},
            { 1,  5,  0,  5,  3,  0, -1},
            { 5,  2,  1, -1, -1, -1, -1},

            { 3,  4,  2,  2,  4,  1, -1},
            { 4,  1,  0, -1, -1, -1, -1},
            { 2,  3,  0, -1, -1, -1, -1},
            {-1, -1, -1, -1, -1, -1, -1},
    };



    // For any edge, if one vertex is inside of the surface and the other is outside of the surface
    //  then the edge intersects the surface
    // For each of the 4 vertices of the tetrahedron can be two possible states : either inside or outside of the surface
    // For any tetrahedron the are 2^4=16 possible sets of vertex states
    // This table lists the edges intersected by the surface for all 16 possible vertex states
    // There are 6 edges.  For each entry in the table, if edge #n is intersected, then bit #n is set to 1
    GLuint tetrahedronEdgeFlags[16]=
    {
       0,   13,   19,   30,   38,   43,   53,   56,   56,   53,   43,   38,   30,   19,   13, 0
   
    };


    void processCube(float x, float y, float z){ 
        Vector3f cubePosition[8];
        float  cubeValue[8];
        Vector3f tetraPosition[4];
        float  tetraVal[4];

        //Make a local copy of the cube's corner positions and values
        for( int i = 0; i < 8; i++){
            cubePosition[i] = Vector3f(x, y, z) + Vector3f(vertexI[i]); 
            cubeValue[i] = findScalar(cubePosition[i]); 
        }

        for(int i = 0; i < 6; i++){ // for each tetra
            for(int j = 0; j < 4; j++){ // for each vertex in one tetra
                int v = tetraInCube[i][j]; // finding whcih vertex it is of the cube
                tetraPosition[j] = cubePosition[v]; // storing its position 
                tetraVal[j] = cubeValue[v]; //storing its value 
            }
             marchingTetrahedron(tetraPosition, tetraVal);
        }
    }

    //marchingTetrahedron performs the Marching Tetrahedrons algorithm on a single tetrahedron
    void marchingTetrahedron(Vector3f tetraPosition[4], float tetraVal[4]){  
        int iFlagIndex = 0, edgeFlag;
        float fOffset, fInvOffset; 
        Vector3f asEdgeVertex[6];

        //Find which vertices are inside of the surface and which are outside
        for(int i = 0; i < 4; i++) {
            if(tetraVal[i] <= isoval) 
                iFlagIndex |= 1<<i; 
        }

        //Find which edges are intersected by the surface
        edgeFlag = tetrahedronEdgeFlags[iFlagIndex];  
        //If the tetrahedron is entirely inside or outside of the surface, then there will be no intersections
        if(edgeFlag == 0) return; 

        //Find the point of intersection of the surface with each edge
        for(int i = 0; i < 6; i++){
            //if there is an intersection on this edge
            if(edgeFlag & (1<<i)){
                int v0 = edgeConnectionInTetra[i][0];
                int v1 = edgeConnectionInTetra[i][1];
                fOffset = fGetOffset(tetraVal[v0], tetraVal[v1], isoval);
                fInvOffset = 1.0 - fOffset;
                asEdgeVertex[i] = tetraPosition[v0] * fInvOffset + tetraPosition[v1] * fOffset; 
            }
        }

        //Draw the triangles that were found.  There can be up to 2 per tetrahedron
        for(int i = 0; i < 2; i++)   {
            if(tetraTriangles[iFlagIndex][3*i] < 0) break;

            for(int j = 0; j < 3; j++){
                int v = tetraTriangles[iFlagIndex][3*i+j];
                Vertex ver; 
                ver.Position = asEdgeVertex[v];  // ver.Position.Print();  //  cout << endl;
                float s = findScalar(ver.Position)/256; 
                ver.Scalar = Vector3f(s, 0, 1-s);
                vertices.push_back(ver); 
            }
        }
    }

    //fGetOffset finds the approximate point of intersection of the surface
    // between two points with the values fValue1 and fValue2
    float fGetOffset(float fValue1, float fValue2, float fValueDesired)
    {
            float fDelta = fValue2 - fValue1;
            if(fDelta == 0.0)return 0.5;
            else return (fValueDesired - fValue1)/fDelta;
    }

    float findScalar(Vector3f p){
        int index = p.z * cubeY * cubeX +  p.y * cubeX + p.x;
        if(index < 0) index = 0; 
        if(index >= scalar.size()) index = scalar.size() - 1; 
        return scalar[index]; 
    }

    // initializes all the buffer objects/arrays
    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
       
        // vertex scalar
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Scalar));
        glBindVertexArray(0);
    }
};


#endif
