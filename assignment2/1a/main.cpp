#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "file_utils.h"
#include "math_utils.h"
#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include "lightBulb.h"

using namespace std;

/********************************************************************/
/*   Variables */

char * theProgramTitle = "Sample";
int theWindowWidth = 700, theWindowHeight = 700;
int theWindowPositionX = 100, theWindowPositionY = 100;
bool isFullScreen = false;
bool isAnimating = true;
float rotation = 1.5f;
float alpha = 0.0f;
bool boxvis = false;

GLuint worldLocation, viewLocation, projectionLocation, lightLocation, eyeLocation;
GLuint posID, cam, colorID; 

Vector3f cameraPos = Vector3f(0.0f, 0.0f, 1.7f);  
float FOV = 45.0f, znear = 0.01, zfar = 10.0f; // field of view


/* Constants */
const int ANIMATION_DELAY = 20; /* milliseconds between rendering */

const char* offFileName = "../Data/1duk.off";

Shader objShader, lightShader;  
Mesh model; 
Camera camera(cameraPos); 
Box lightbox;

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
	lightbox.createBox(); 
	model.readMesh(offFileName);  // cout << sizeof (model) << " " << model.vertices.size(); 
}

void onInit(int argc, char * argv[])
/* pre:  glut window has been initialized
   post: model has been initialized */ {
	/* by default the back ground color is black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();
	objShader.CompileShaders("shader.vs", "shader.fs"); 
	lightShader.CompileShaders("shaderB.vs", "shaderB.fs");  cout << "shader ok\n"; 

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
	// World.InitOrthoProjTransform( model.minX, model.maxX, model.minY, model.maxY, model.minZ, model.maxZ, 1.2);
	

	int id = objShader.use(); 
	wScale.InitScaleTransform(0.025, 0.025, 0.025); 
	wTranslate.InitTranslationTransform(0, -0.5, 0);
	World = wTranslate * wScale; 
	glUniformMatrix4fv(glGetUniformLocation(id, "world"), 1, GL_TRUE, &World.m[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_TRUE, &View.m[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_TRUE, &Projection.m[0][0]);

	glUniform3f(glGetUniformLocation(id, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	float lightX = 1.0 * cosf(rotation); 
	float lightY = 0.0 ; 
	float lightZ = 1.0 * sinf(rotation);
	glUniform3f(glGetUniformLocation(id, "lightPos"), lightX, lightY, lightZ);

	model.Draw();

	if(boxvis){
		id = lightShader.use(); 
		wScale.InitScaleTransform(0.025, 0.025, 0.025); 
		wTranslate.InitTranslationTransform(lightX, lightY, lightZ);
		World = wTranslate * wScale; 
		glUniformMatrix4fv(glGetUniformLocation(id, "world"), 1, GL_TRUE, &World.m[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_TRUE, &View.m[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_TRUE, &Projection.m[0][0]);

		glUniform3f(glGetUniformLocation(id, "lightPos"), lightX, lightY, lightZ);
		lightbox.draw();

	}




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
			rotation += alpha; 

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

	if(key == '1'){
		alpha = 0.01;
	}
	if(key == '2'){
		boxvis = true;
	}

	/* notify window that it has to be re-rendered */
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

