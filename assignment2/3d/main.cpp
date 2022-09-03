#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "file_utils.h"
#include "math_utils.h"
// #include "mesh.h"
#include "plane.h"
#include "camera.h"



using namespace std;

/********************************************************************/
/*   Variables */

char * theProgramTitle = "Sample";
int theWindowWidth = 700, theWindowHeight = 700;
int theWindowPositionX = 100, theWindowPositionY = 100;
bool isFullScreen = false;
bool isAnimating = true;
float rotation = 0.0f;
float speed = 0.0f;
float isoval = 0.1;

GLuint worldLocation, viewLocation, projectionLocation;
GLuint posID, cam, colorID; 

Vector3f cameraPos = Vector3f(0.0f, 0.0f, 1.7f);  
float FOV = 45.0f, znear = 0.01, zfar = 10.0f; // field of view


/* Constants */
const int ANIMATION_DELAY = 20; /* milliseconds between rendering */


#include "shader.h"
#include "boundingBox.h"
Shader boxShader, planeShader;  
boundingBox box;

const char* offFileName2 = "../Data/bt.off";

// Cube cube;
Plane plane; 
// Mesh model; 
Camera camera(cameraPos); 

PersProjInfo persProj(FOV, 1, 1, znear, zfar); 

/********************************************************************
  Utility functions 
 */

/* post: compute frames per second and display in window's title bar */
void computeFPS() {
	static int frameCount = 0;
	static int lastFrameTime = 0;
	static char * title = NULL;
	int currentTime;

	if (!title)
		title = (char*) malloc((strlen(theProgramTitle) + 20) * sizeof (char));
	frameCount++;
	currentTime = glutGet((GLenum) (GLUT_ELAPSED_TIME));
	if (currentTime - lastFrameTime > 1000) {
		sprintf(title, "%s [ FPS: %4.2f ]",
			theProgramTitle,
			frameCount * 1000.0 / (currentTime - lastFrameTime));
		glutSetWindowTitle(title);
		lastFrameTime = currentTime;
		frameCount = 0;
	}
}


static void CreateVertexBuffer() {
	box.createBox(); 
	plane.readMesh(offFileName2); 
}

/********************************************************************
 Callback Functions
 These functions are registered with the glut window and called 
 when certain events occur.
 */

void onInit(int argc, char * argv[])
/* pre:  glut window has been initialized
   post: model has been initialized */ {
	/* by default the back ground color is black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer(); 
	boxShader.CompileShaders("shader.vs", "shader.fs"); 
	planeShader.CompileShaders("shaderP.vs", "shaderP.fs", "shaderP.gs");  cout << "shader ok\n"; 
	
	// CompileShaders();

	/* set to draw in window based on depth  */
	glEnable(GL_DEPTH_TEST); 
}

static void onDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4f Projection, persP;

    // Projection.InitOrthoProjTransform( model.minX, model.maxX, model.minY, model.maxY, model.minZ, model.maxZ, 1.2);
	Projection.InitPersProjTransform(persProj); 
	
	Matrix4f View =  camera.GetMatrix(); 
 
	Matrix4f World, wScale, wTranslate, wRotate;
	// World.InitScaleTransform(1.0/10 , 1.0/10 , 1.0/10);
	// World.InitOrthoProjTransform( 0, 256, 0, 256, 0.1, 256, 1.2);
	

	// glUniformMatrix4fv(worldLocation, 1, GL_TRUE, &World.m[0][0]);
	// glUniformMatrix4fv(viewLocation, 1, GL_TRUE, &View.m[0][0]);
	// glUniformMatrix4fv(projectionLocation, 1, GL_TRUE, &Projection.m[0][0]);


	int id = boxShader.use(); 
	// World.InitRotateTransform(0, rotation, 0);
	glUniformMatrix4fv(glGetUniformLocation(id, "world"), 1, GL_TRUE, &World.m[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_TRUE, &View.m[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_TRUE, &Projection.m[0][0]);
	box.draw(); 

	id = planeShader.use();
	// set the matrix in vertex shader
	World =  Projection * (View *  World);

	glUniformMatrix4fv(glGetUniformLocation(id, "mvp"), 1, GL_TRUE, &World.m[0][0]);
	glUniform1f(glGetUniformLocation(id, "isoval"), isoval);

	plane.Draw(); 


	/* check for any errors when rendering */
	GLenum errorCode = glGetError();
	if (errorCode == GL_NO_ERROR) {
		/* double-buffering - swap the back and front buffers */
		glutSwapBuffers();
	} else {
		fprintf(stderr, "OpenGL rendering error %d\n", errorCode);
	}
}

/* pre:  glut window has been resized
 */
static void onReshape(int width, int height) {
	glViewport(0, 0, width, height);
	if (!isFullScreen) {
		theWindowWidth = width;
		theWindowHeight = height;
	}
	// update scene based on new aspect ratio....
}

/* pre:  glut window is not doing anything else
   post: scene is updated and re-rendered if necessary */
static void onIdle() {
	static int oldTime = 0;
	if (isAnimating) {
		int currentTime = glutGet((GLenum) (GLUT_ELAPSED_TIME));
		/* Ensures fairly constant framerate */
		if (currentTime - oldTime > ANIMATION_DELAY) {
			// do animation....
			rotation += 0.1;
			speed += 0.001; 

			oldTime = currentTime;
			/* compute the frame rate */
			computeFPS();
			/* notify window it has to be repainted */
			glutPostRedisplay();
		}
	}
}

/* pre:  mouse is dragged (i.e., moved while button is pressed) within glut window
   post: scene is updated and re-rendered  */


static void onMouseMotion(int x, int y) {
	/* notify window that it has to be re-rendered */
    // cout << x <<  " " << y << endl;	
	glutPostRedisplay();
}


/* pre:  mouse button has been pressed while within glut window
   post: scene is updated and re-rendered */
static void onMouseButtonPress(int button, int state, int x, int y) {

	/*
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Left button pressed 
		rotation += 0.1;
		//mouseoldx = x; mouseoldy = y; 
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		// Left button un pressed
	}
	/* notify window that it has to be re-rendered */
	glutPostRedisplay();
}

/* pre:  key has been pressed
   post: scene is updated and re-rendered */
static void onAlphaNumericKeyPress(unsigned char key, int x, int y) {
	camera.onKeyPress(key);

	/* notify window that it has to be re-rendered */

	if(key == '+') isoval += 0.1;
	if(key == '-') isoval -= 0.1;
	glutPostRedisplay();
}

/* pre:  arrow or function key has been pressed
   post: scene is updated and re-rendered */
static void onSpecialKeyPress(int key, int x, int y) {
	/* please do not change function of these keys */
	switch (key) {
			/* toggle full screen mode */
		case GLUT_KEY_F1:
			isFullScreen = !isFullScreen;
			if (isFullScreen) {
				theWindowPositionX = glutGet((GLenum) (GLUT_WINDOW_X));
				theWindowPositionY = glutGet((GLenum) (GLUT_WINDOW_Y));
				glutFullScreen();
			} else {
				glutReshapeWindow(theWindowWidth, theWindowHeight);
				glutPositionWindow(theWindowPositionX, theWindowPositionY);
			}
			break;
	}

	/* notify window that it has to be re-rendered */
	glutPostRedisplay();
}

/* pre:  glut window has just been iconified or restored 
   post: if window is visible, animate model, otherwise don't bother */
static void onVisible(int state) {
	if (state == GLUT_VISIBLE) {
		/* tell glut to show model again */
		glutIdleFunc(onIdle);
	} else {
		glutIdleFunc(NULL);
	}
}

static void InitializeGlutCallbacks() {
	/* tell glut how to display model */
	glutDisplayFunc(onDisplay);
	/* tell glutwhat to do when it would otherwise be idle */
	glutIdleFunc(onIdle);
	/* tell glut how to respond to changes in window size */
	glutReshapeFunc(onReshape);
	/* tell glut how to handle changes in window visibility */
	glutVisibilityFunc(onVisible);
	/* tell glut how to handle key presses */
	glutKeyboardFunc(onAlphaNumericKeyPress);
	glutSpecialFunc(onSpecialKeyPress);
	/* tell glut how to handle the mouse */
	glutMotionFunc(onMouseMotion);
	glutMouseFunc(onMouseButtonPress);

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	/* request initial window size and position on the screen */
	glutInitWindowSize(theWindowWidth, theWindowHeight);
	glutInitWindowPosition(theWindowPositionX, theWindowPositionY);
	/* request full color with double buffering and depth-based rendering */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	/* create window whose title is the name of the executable */
	glutCreateWindow(theProgramTitle);

	InitializeGlutCallbacks();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));

	/* initialize model */
	onInit(argc, argv);

	/* give control over to glut to handle rendering and interaction  */
	glutMainLoop();

	/* program should never get here */

	return 0;
}

