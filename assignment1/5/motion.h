#ifndef MOTION_H
#define MOTION_H


#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math_utils.h"

class Motion {
public:
    Motion(float r, float s){
        velocity.x = RandomFloat(); 
        velocity.y = RandomFloat(); 
        velocity.z = RandomFloat(); 
        
        // velocity = Vector3f(1.0f, 0, 0); 
        velocity.Normalize(); 
        radius = r; 
        speed = s; 
    }
    Motion(Vector3f &x, float r, float s) {
        velocity = x;
        velocity.Normalize(); 
        radius = r; 
        speed = s; 
    }

    Vector3f inMotion(){
        // speed+= 0.001f; 
        findPosition();
        return position; 
    }

private: 
    Vector3f velocity; 
    float speed = 0.1, radius; 
    Vector3f position = Vector3f(0, 0, 0); 
    int stop = 0, stopCount = 5; 

    bool touching(){
        if( position.x + radius >= 1 || position.x - radius <= -1 ||
            position.y + radius >= 1 || position.y - radius <= -1 ||
            position.z + radius >= 1 || position.z - radius <= -1){
                return true;
        }   
        return false; 
    }

    void findPosition(){
        if(touching()){
            // squeeze unsqueeze
            if(stop < stopCount) {
                stop++; return; 
            }
            stop = 0; 
        }

        if(position.x + radius >= 1 || position.x - radius <= -1) {
            velocity.x *= -1; 
        }  

        if(position.y + radius >= 1 || position.y - radius <= -1) { 
            velocity.y *= -1; 
        }   

        if(position.z + radius >= 1 || position.z - radius <= -1) {
            velocity.z *= -1; 
        }  

        position += velocity * speed;   // printf("%f %f %f\n", position.x, position.y, position.z);    
    }
}; 

#endif
