#include "ARPong.h"
#include "game.h"
#include "histogram.h"
#include "objectDetection.h"
#include "render.h"
#include "tcp_client.h"
#include "tcp_server.h"
#include "video.h"

#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

namespace {
	const string TRAINING_FILE = "../skin_rgb.txt";

	// video stream we're reading from
	video_stream stream;

	// window for hand-cursor positioning
	sliding_window cursor_pos;

	game global_game;

	enum PlayerType { SERVER = 1, CLIENT = 2 };
	PlayerType player_number;
	string server_ip;

	// should we close the application?
	bool window_closed = false;
}

// Clean up the application before we exit
void on_close() {
	window_closed = true;
	stream.cleanup();
}

// frame that's had the detection algorithm run through it
frame dtn_frame;

// histogram that does the skin tone detection
histogram hist;

// detection threshold
double tld = .000001;

// Run skin-detection algorithm
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

void net_loop() {
	if(player_number == SERVER) {   
		ListenOnPort(9000);
	}
	else {
		bool connect_status = false;
		while(!connect_status) {
			connect_status = ConnectToHost(9000, (char*)server_ip.c_str());
		}
	}
	while(!window_closed) {
		auto paddle = vec2(cursor_pos.value());

		paddle.x = paddle.x / WIDTH * 2 - 1;
		paddle.y = paddle.y / HEIGHT * 2 - 1;

		if(player_number == SERVER) {
			auto data = ServerRead();
			global_game.p1.pos = paddle;
			global_game.p2.pos = data.paddle;
			ServerWrite(paddle, global_game.b.pos);
		}
		else {
			auto data = ClientRead();
			global_game.p1.pos = paddle;
			global_game.p2.pos = data.paddle;
			global_game.b.pos = data.ball;
			ClientWrite(paddle.x, paddle.y);
		}
	}
}

bool main_loop_iter() {
	// If we got a video frame, render it and run the game
	if(!window_closed && stream.next_frame()) {
		dtn_frame = detect_skin(stream.current_frame);
		cursor_pos.push_value(calculate_median(dtn_frame));

		// Tell GLUT to render this frame and manually proceed to the next one
		glutPostRedisplay();
		glutMainLoopEvent();

		auto c = cursor_pos.value();
		cout << c.x << ',' << c.y << '\n';

		return true;
	}
	// Can't capture more video, exit the program
	else {
		return false;
	}
}

int main(int argc, char** argv) {
	player_number = SERVER;

	global_game.b.radius = 0.25;

	// The client provides the IP address of the server.  the server starts
	// with no arguments.
	if(argc > 1) {
		server_ip = argv[1];
		player_number = CLIENT;
	}
	else {
		global_game.b.pos = glm::vec3(0, 0, 0);
		global_game.b.velocity = glm::vec3(0, 0, 0.5);
	}

	ifstream training(TRAINING_FILE);
	hist = load_histogram(training);

	setupGlut(argc, argv, global_game);
	glutCloseFunc(on_close);

	thread t(net_loop);

	// keep running until the video source quits or someone closes us
	while(main_loop_iter()) { }
}