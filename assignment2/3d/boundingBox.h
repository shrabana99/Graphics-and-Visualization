#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H


#include <string>
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_utils.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;


class boundingBox
{
private:
    /* data */
    unsigned int VBO, cubeVAO;


public:
    void createBox(/* args */){
        float vertices[6*8*3] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f, 0.5f, 
         0.5f, -0.5f, 0.5f, 
         0.5f, -0.5f, 0.5f, 
         0.5f,  0.5f, 0.5f,  
         0.5f,  0.5f, 0.5f,  
        -0.5f,  0.5f, 0.5f, 
        -0.5f,  0.5f, 0.5f, 
        -0.5f, -0.5f, 0.5f, 


        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,  
        -0.5f, 0.5f, -0.5f,  
        -0.5f, 0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f,  

        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,  
        0.5f, 0.5f, -0.5f,  
        0.5f, 0.5f,  0.5f,  
        0.5f,  0.5f,  0.5f, 
        0.5f, -0.5f,  0.5f,  
        0.5f, -0.5f,  0.5f, 
        0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f, -0.5f,  
        0.5f, -0.5f,  0.5f,  
        0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f,  

       -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,  
        0.5f, 0.5f, -0.5f,  
        0.5f, 0.5f,  0.5f,  
        0.5f, 0.5f,  0.5f, 
        -0.5f, 0.5f,  0.5f,  
        -0.5f, 0.5f,  0.5f, 
        -0.5f, 0.5f, -0.5f,  
    };

         glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(cubeVAO);

        // position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
        
    }

    void draw(){
       // render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_LINES, 0, 48);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(0);

    }
};



#endif
