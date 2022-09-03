// http://www.songho.ca/opengl/gl_camera.html#overview

#ifndef CAMERA_H
#define CAMERA_H

#include <GL/freeglut.h>
#include "math_utils.h"

class Camera
{
public:

    Camera(Vector3f pos) {
        position = pos; 
        setParameters(); 
    }

    void onKeyPress(unsigned char key)
    {
        // printf("%d\n", key); 
        Matrix4f rotateC = rollCamera();  
        Matrix4f translateC = translateCamera(); 
        if(key == '1') {
            viewMatrix =  rotateC * translateC ; 
        }
        else if(key == '2') {
            viewMatrix =  translateC * rotateC ; 
        }
    }
    
    Matrix4f GetMatrix(){
        return viewMatrix; 
    }


private:

    Matrix4f viewMatrix; 
    Vector3f worldUp    = Vector3f(0.0f, 1.0f, 0.0f); // world's +y axis 

    // camera vectors
    Vector3f position   = Vector3f(0.0f, 0.0f, 1.0f); // cam position or eye position  
    Vector3f target     = Vector3f(0.0f, 0.0f, 0.0f); // target postion, always looking along -Z axis.
    
    // right : cam +x axis, up : cam +y axis, forward : cam +z axis, 
    Vector3f up, forward, right; 

    // for 1 & 2 with projection
    float speed = 0.01f; //  angle -= 0.1; 
    float angle = 0.0;

    Matrix4f translateCamera(){
        position -= Vector3f(1, 0, 0) * speed; 

        // setParameters();

        Matrix4f camTranslate;
        camTranslate.InitTranslationTransform(-position.x, -position.y, -position.z);  
        return camTranslate; 
    }

    Matrix4f rollCamera(){
        angle -= 0.1;  // printf("%f\n", angle); 
        float cs = cos(ToRadian(angle));
        float sn = sin(ToRadian(angle));
        
        Vector3f newRight   =  right * cs + up * sn;
        Vector3f newUp      =  up * cs - right * sn;

        newRight.Normalize();
        newUp.Normalize(); 

        up = newUp;
        right = newRight; 
        
        // forward = right.Cross(up);
        return Matrix4f     (right.x,   right.y,    right.z,    0, 
                            up.x,       up.y,       up.z,       0,
                            forward.x,  forward.y,  forward.z,  0,
                            0,          0,          0,          1);
        
    }

    void setParameters(){
        forward = position - target; 
        forward.Normalize(); 

        right = worldUp.Cross(forward);
        right.Normalize(); 

        up = forward.Cross(right);
        up.Normalize();
        lookAt();  
    }

    void lookAt(){
        Matrix4f camRotation(right.x,   right.y,    right.z,    0, 
                            up.x,       up.y,       up.z,       0,
                            forward.x,  forward.y,  forward.z,  0,
                            0,          0,          0,          1);
        
        Matrix4f camTranslate;
        camTranslate.InitTranslationTransform(-position.x, -position.y, -position.z);  
        
        viewMatrix = camRotation * camTranslate;
    }

};

#endif 