#include "video.h"
#include <escapi.h>
#include <iostream>

using namespace std;

bool video_stream::escapi_initialized = false;

video_stream::video_stream() : deinitialized(false) {
	if(!escapi_initialized) {
		// Initialize the DLL and ask how many webcams it found
		int devices = setupESCAPI();
		if(!devices) {
			cerr << "ESCAPI found no devices.\n";
			throw runtime_error("setupESCAPI");
		}

		escapi_initialized = true;
	}

	SimpleCapParams scp = { 
		reinterpret_cast<int*>(current_frame.get_buffer_data()), 
		WIDTH, 
		HEIGHT 
	};

	if(!initCapture(DEVICE, &scp)) {
		cerr << "Couldn't initialize camera.  Get a better computer.\n";
		throw runtime_error("initCapture");
	}
}

void video_stream::cleanup() {
	if(!deinitialized) {
		deinitCapture(DEVICE);
		deinitialized = true;
	}
}

video_stream::~video_stream() {
	cleanup();
}

rgb_byte video_stream::get_pix(int x, int y) const {
	const uint8_t* p = current_frame.get_pixel(x, y);
	rgb_byte r = { p[2], p[1], p[0] };
	return r;
}

void video_stream::set_pix(int x, int y, const rgb_byte& pix) {
	uint8_t* b = current_frame.get_pixel(x, y);

	// Video data is in BGRA order
	*b++ = pix.b;
	*b++ = pix.g;
	*b++ = pix.r;
}

// advance to the next frame and return whether that operation succeeded
bool video_stream::next_frame() {
	doCapture(DEVICE);
	return !isCaptureDone(DEVICE);
}