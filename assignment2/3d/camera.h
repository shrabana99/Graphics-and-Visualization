// http://www.songho.ca/opengl/gl_camera.html#overview
// https://learnopengl.com/Getting-started/Camera

#ifndef CAMERA_H
#define CAMERA_H

#include <GL/freeglut.h>
#include "math_utils.h"

class Camera
{
public:

    Camera(Vector3f pos) {
        position = pos; 
        setParameters(); // right.Print(); up.Print();  forward.Print(); 
    }

    void onKeyPress(unsigned char key)
    {
        if(key == 'l'){
            position -= right*speed; 
        }
        else if(key == 'r'){
            position += right*speed; 
        }
        else if(key == 'f'){
            position -= forward*speed;
        }
        else if(key == 'b'){
            position += forward*speed;
        }
        lookAt();
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

    float speed = 0.01f; 
    float angle = 0.0;

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