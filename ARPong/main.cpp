#include "ARPong.h"
#include "histogram.h"
#include "morph.h"
#include "objectDetection.h"
#include "video.h"

#include <GL/freeglut.h>

#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

const char* TRAINING_FILE = "../skin_rgb.txt";

video_stream stream;
frame dtn_frame;
histogram hist;

GLuint texture;

// should we close the application?
bool window_closed = false;

// detection threshold
double tld = .00005;

center_t median = { };

// OpenGL display function, called whenever a new frame is requested
void display() {
	// vertices for simple textured-quad rendering
	const struct { float tu, tv; float x, y, z; } vertices[] = {
		{ 0.0f, 0.0f, -1.0f,-1.0f, 0.0f },
		{ 1.0f, 0.0f,  1.0f,-1.0f, 0.0f },
		{ 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }
	};

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glScaled(-1.0, -1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, dtn_frame.get_buffer_data());
	glInterleavedArrays(GL_T2F_V3F, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();

	glPushMatrix();
	glScaled(-1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glVertex2d(median.x * 2. / WIDTH - 1., median.y * 2. / HEIGHT - 1.);
	glEnd();
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	glutSwapBuffers();
}

void on_close() {
	window_closed = true;
	stream.cleanup();
}

void detect_skin() {
	dtn_frame = stream.current_frame;

	const rgb_byte ZERO = { };
	for(int y = HEIGHT - 1; y; --y) {
		for(int x = WIDTH - 1; x; --x) {
			auto orig = dtn_frame.get_pixel(x, y);
			rgb hist_in = { orig[2], orig[1], orig[0] };
			if(hist.value(hist_in) < tld) {
				orig[0] = orig[1] = orig[2] = 0;
			}
		}
	}
}

bool main_loop_iter() {
	// If we got a video frame, render it and run the game
	if(!window_closed && stream.next_frame()) {
		/// TODO: Game logic and networking calls go here

		detect_skin();
		erode(dtn_frame);
		median = calculate_median(dtn_frame);

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
	ifstream training(TRAINING_FILE);
	hist = load_histogram(training);

	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ARPong");
	glutDisplayFunc(display);
	glutCloseFunc(on_close);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, WIDTH, HEIGHT);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// keep running until the video source quits or someone closes us
	while(main_loop_iter()) { }
}