#include <GL/freeglut.h>
#include <escapi.h>
#include <cstdint>
#include <fstream>
#include <iostream>

using namespace std;

enum { DEVICE = 0, WIDTH = 1024, HEIGHT = 768 };

uint32_t buffer[WIDTH * HEIGHT] = { };

const char* out_name = "../skin_rgb.txt";
ofstream out_file;

inline uint32_t* get_pix_ptr(int x, int y) {
	return &buffer[(WIDTH * (HEIGHT - y) + x)];
}

// coordinates of most recent click
struct { int x, y; } last_click = { -1, -1 };
struct { float r, g, b; } color = { -1, -1, -1 };

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);

	glInterleavedArrays(GL_T2F_V3F, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();
	glutSwapBuffers();
}

// On mouse down, start a selection rectangle.  On mouse up, record all the pixels in the selection
void click(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN) {
			last_click.x = x;
			last_click.y = y;
		}
		else {
			if(last_click.x > 0) {
				for(int x0 = min(last_click.x, x); x0 <= max(last_click.x, x); ++x0) {
					for(int y0 = min(last_click.y, y); y0 <= max(last_click.y, y); ++y0) {
						uint8_t* pix = reinterpret_cast<uint8_t*>(get_pix_ptr(x0, y0));
						color.r = pix[2];
						color.g = pix[1];
						color.b = pix[0];
						out_file << (int)pix[2] << ',' << (int)pix[1] << ',' << (int)pix[0] << '\n';
					}
				}
			}
		}
	}
}

void key(unsigned char, int, int) {
	doCapture(DEVICE);
	if(isCaptureDone(DEVICE)) {
		glutExit();
	}
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	// Set up GLUT parameters and callbacks
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ARPong webcam trainer");
	glutKeyboardFunc(key);
	glutMouseFunc(click);
	glutDisplayFunc(display);

	// Set up texture parameters
	GLuint texture;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, WIDTH, HEIGHT);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// initialize output file
	out_file.open(out_name, ios::ate);

	// Initialize ESCAPI library for video capture
	int devices = setupESCAPI();
	if(!devices) {
		cerr << "ESCAPI found no devices.\n";
		throw runtime_error("setupESCAPI");
	}

	for(int i = 0; i < devices; ++i) {
		char buf[128] = { };
		getCaptureDeviceName(i, buf, sizeof(buf));
		cout << buf << "\n";
	}

	SimpleCapParams scp = { reinterpret_cast<int*>(buffer), WIDTH, HEIGHT };
	if(!initCapture(DEVICE, &scp)) {
		cerr << "Couldn't initialize camera.  Get a better computer.\n";
		throw runtime_error("initCapture");
	}

	doCapture(DEVICE);
	if(isCaptureDone(DEVICE)) {
		cerr << "Could not get camera frame.\n";
		throw runtime_error("isCaptureDone");
	}

	glutMainLoop();
	deinitCapture(DEVICE);
}