#include "ARPong.h"
#include "histogram.h"
#include "objectDetection.h"
#include "video.h"

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

const char* TRAINING_FILE = "../skin_rgb.txt";

video_stream stream;
frame dtn_frame;
histogram hist;

GLuint texture;

// should we close the application?
bool window_closed = false;

// detection threshold
double tld = .000001;

class sliding_window {
	enum { SIZE = 10 };
	list<glm::ivec2> history;

public:
	void push_value(glm::ivec2 p) {
		history.push_back(p);
		if(history.size() > SIZE) {
			history.pop_front();
		}
	}

	glm::ivec2 value() const {
		glm::vec2 tmp(0);
		int factor = 1;
		for(auto& p : history) {
			tmp += glm::vec2(p) / pow(2.0f, factor++);
		}

		return glm::ivec2(tmp);
	}
};

sliding_window cursor_pos;

glm::vec2 ball(0.0, 0.0), velocity(0.0, 1.0), accel(0.0, -1.0);

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

	auto cur = cursor_pos.value();
	auto cur_vec = glm::vec2(cur.x * 2. / WIDTH - 1, cur.y * 2. / HEIGHT - 1.);
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
		glVertex2f(cur_vec.x - 0.1, -0.8);
		glVertex2f(cur_vec.x + 0.1, -0.8);
		glVertex2f(cur_vec.x + 0.1, -0.9);
		glVertex2f(cur_vec.x - 0.1, -0.9);
	glEnd();
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glVertex2f(ball.x, ball.y);
	glEnd();
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	glutSwapBuffers();
}

void on_close() {
	window_closed = true;
	stream.cleanup();
}

frame detect_skin(const frame& in) {
	dtn_frame = in;

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

	return dtn_frame;
}

bool main_loop_iter() {
	// If we got a video frame, render it and run the game
	if(!window_closed && stream.next_frame()) {
		/// TODO: Game logic and networking calls go here

		dtn_frame = detect_skin(stream.current_frame);
		cursor_pos.push_value(calculate_median(dtn_frame));
		auto cur = cursor_pos.value();
		auto cur_vec = glm::vec2(cur.x * 2. / WIDTH - 1, cur.y * 2. / HEIGHT - 1.);

		float time = 0.01;
		velocity += accel * time;
		ball += velocity * time;
		if(ball.y < -0.8) {
			if(std::abs(ball.x - cur_vec.x) < 0.1 && ball.y > -1) {
				ball.y = -0.8;
				velocity *= -1;
				velocity.x += ball.x - cur_vec.x;
			}
			else {
				cout << "You lose!\n";
				ball = glm::vec2(0, 0);
				velocity = glm::vec2(0, 1);
			}
		}
		if(ball.x < -1) {
			ball.x = -1;
			velocity.x *= -1;
		}
		if(ball.x > 1) {
			ball.x = 1;
			velocity.x *= -1;
		}

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