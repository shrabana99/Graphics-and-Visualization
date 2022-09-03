#version 460
layout (points) in;
layout (points, max_vertices = 6) out;

in vec3 pos[];

uniform mat4 mvp;

uniform sampler3D volumeTexture;

uniform float isoval;


    float a2fVertexOffset[8][3] =
    {
            {0.0, 0.0, 0.0},{1.0, 0.0, 0.0},{1.0, 1.0, 0.0},{0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0},{1.0, 0.0, 1.0},{1.0, 1.0, 1.0},{0.0, 1.0, 1.0}
    };

    int tetraEdgeConnection[6][2] =
    {
            {0,1},  {1,2},  {2,0},  {0,3},  {1,3},  {2,3}
    };

    int tetraInCube[6][4] =
    {
            {0,5,1,6},
            {0,1,2,6},
            {0,2,3,6},
            {0,3,7,6},
            {0,7,4,6},
            {0,4,5,6}
    };


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



    int tetraFlag[16]=
    {
               0,   13,   19,   30,   38,   43,   53,   56,   56,   53,   43,   38,   30,   19,   13, 0
    };


    float fGetOffset(float fValue1, float fValue2, float fValueDesired)
    {
            float fDelta = fValue2 - fValue1;
            if(fDelta == 0.0)return 0.5;
            else return (fValueDesired - fValue1)/fDelta;
    }

    float findScalar(vec3 p){
        p[0] /= 256; p[1] /= 256; p[2] /= 178;
        return texture(volumeTexture, p).r;
    }


    //marchingTetrahedron performs the Marching Tetrahedrons algorithm on a single tetrahedron
    void marchingTetrahedron(vec3 tetraPosition[4], float tetraVal[4]){  
        int iFlagIndex = 0, edgeFlag = 0;
        float fOffset, fInvOffset; 
        vec3 asEdgeVertex[6];

        //Find which vertices are inside of the surface and which are outside
        for(int i = 0; i < 4; i++) {
            if(tetraVal[i] <= isoval) 
                iFlagIndex += (1<<i); 
        }
        
        //Find which edges are intersected by the surface
        if(iFlagIndex > 16 ) return;
        edgeFlag = tetraFlag[iFlagIndex]; 

        //If the tetrahedron is entirely inside or outside of the surface, then there will be no intersections
        if(edgeFlag == 0) {
                return;
        }

        //Find the point of intersection of the surface with each edge
        // Then find the normal to the surface at those points
        for(int i = 0; i < 6; i++){
            //if there is an intersection on this edge
            if(((edgeFlag>>i) & 1) == 1){
                int v0 = tetraEdgeConnection[i][0];
                int v1 = tetraEdgeConnection[i][1];
                fOffset = fGetOffset(tetraVal[v0], tetraVal[v1], isoval);
                fInvOffset = 1.0 - fOffset;

                asEdgeVertex[i] = tetraPosition[v0] * fInvOffset + tetraPosition[v1] * fOffset; 
            }
        }

        //Draw the triangles that were found.  There can be up to 2 per tetrahedron
        for(int i = 0; i < 2; i++)   {
            if(tetraTriangles[iFlagIndex][3*i] < 0) break;
            else{
                for(int j = 0; j < 3; j++){
                        int v = tetraTriangles[iFlagIndex][3*i+j];
                        
                        vec3 p = vec3(asEdgeVertex[v][0], asEdgeVertex[v][1], asEdgeVertex[v][2]);

                        p[0] /= 256; p[1] /= 256; p[2] /= 178;
                        gl_Position = mvp * vec4(p - vec3(0.5, 0.5, 0.5), 1); 
                        EmitVertex();

                }
                EndPrimitive();
            }
        }
    }


    void processCube(vec3 p){ 
        vec3 cubePosition[8];
        float  cubeValue[8];
        vec3 tetraPosition[4];
        float  tetraVal[4];

        //Make a local copy of the cube's corner positions and values
        int c1 =0, c2 = 0;
        for( int i = 0; i < 8; i++){
            cubePosition[i] = p + vec3(a2fVertexOffset[i][0], a2fVertexOffset[i][1], a2fVertexOffset[i][2]); 
            cubeValue[i] = findScalar(cubePosition[i]); 
            if(cubeValue[i] < isoval) c1++; 
            if(cubeValue[i] > isoval) c2++; 
        }
        // early return 
        if(c1 == 8 || c2 == 8) return;

        for(int i = 0; i < 6; i++){ // for each tetra
            for(int j = 0; j < 4; j++){ // for each vertex in one tetra
                int v = tetraInCube[i][j]; // finding whcih vertex it is of the cube
                tetraPosition[j] = cubePosition[v]; // storing its position 
                tetraVal[j] = cubeValue[v]; //storing its value 
            }
             marchingTetrahedron(tetraPosition, tetraVal);
        } 
    }


void main()
{       
       processCube(pos[0]);
}
