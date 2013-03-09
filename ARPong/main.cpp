#include "ARPong.h"

#include <GL/freeglut.h>
#include <escapi.h>

#include <iostream>

// Use the first webcam device we find.  Change this if we ever need to 
// support multiple devices.
enum { DEVICE = 0 };

// Buffer into which video frames will be copied.
int frame_buffer[WIDTH * HEIGHT];

using namespace std;

// OpenGL display function, called whenever a new frame is requested
void display() {
	// vertices for simple textured-quad rendering
	const struct { float tu, tv; float x, y, z; } vertices[] = {
		{ 0.0f, 0.0f, -1.0f,-1.0f, 0.0f },
		{ 1.0f, 0.0f,  1.0f,-1.0f, 0.0f },
		{ 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }
	};

	glPushMatrix();
	glScaled(-1.0, -1.0, 0.0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, frame_buffer);

	glInterleavedArrays(GL_T2F_V3F, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();
	glutSwapBuffers();
}

// Set up the image capture library ESCAPI
void setup_escapi() {
	// Initialize the DLL and ask how many webcams it found
	int devices = setupESCAPI();
	if(!devices) {
		cerr << "ESCAPI found no devices.\n";
		throw runtime_error("setupESCAPI");
	}

	// Tell the library where our buffer is and how big it is
	SimpleCapParams params = { frame_buffer, WIDTH, HEIGHT };
	if(!initCapture(DEVICE, &params)) {
		cerr << "initCapture failed, please install a better webcam\n";
		throw runtime_error("initCapture");
	}
}

bool main_loop_iter() {
	doCapture(DEVICE);

	// If we got a video frame, render it and run the game
	if(!isCaptureDone(DEVICE)) {
		/// TODO: Game logic and networking calls go here

		// Tell GLUT to render this frame and manually proceed to the next one
		glutPostRedisplay();
		glutMainLoopEvent();
		return true;
	}
	// Can't capture more video, exit the program
	else {
		return false;
	}
}

int main(int argc, char** argv) {
	setup_escapi();

	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ARPong");
	glutDisplayFunc(display);

	GLuint texture;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, WIDTH, HEIGHT);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// keep running until the video source quits or someone closes us
	while(main_loop_iter()) { }

	deinitCapture(DEVICE);
}