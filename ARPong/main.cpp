#include "ARPong.h"
#include "histogram.h"
#include "video.h"

#include <GL/freeglut.h>

#include <algorithm>
#include <fstream>
using namespace std;

const char* TRAINING_FILE = "../skin_rgb.txt";

video_stream stream;
uint32_t dtn_buffer[WIDTH * HEIGHT];
histogram hist;
double tld = .00001;

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, dtn_buffer);

	glInterleavedArrays(GL_T2F_V3F, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();
	glutSwapBuffers();
}

void detect_skin() {
	copy(begin(stream.buffer), end(stream.buffer), begin(dtn_buffer));
	const rgb_byte ZERO = { };
	for(int y = HEIGHT - 1; y; --y) {
		for(int x = WIDTH - 1; x; --x) {
			auto orig = reinterpret_cast<uint8_t*>(get_pix_ptr(dtn_buffer, x, y));
			rgb hist_in = { orig[2], orig[1], orig[0] };
			if(hist.value(hist_in) < tld) {
				orig[0] = orig[1] = orig[2] = 0;
			}
		}
	}
}

bool main_loop_iter() {
	// If we got a video frame, render it and run the game
	if(stream.next_frame()) {
		/// TODO: Game logic and networking calls go here

		detect_skin();

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
}