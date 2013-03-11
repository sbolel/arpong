#include "video.h"
#include <escapi.h>
#include <iostream>

using namespace std;

bool video_stream::escapi_initialized = false;

video_stream::video_stream() {
	if(!escapi_initialized) {
		// Initialize the DLL and ask how many webcams it found
		int devices = setupESCAPI();
		if(!devices) {
			cerr << "ESCAPI found no devices.\n";
			throw runtime_error("setupESCAPI");
		}

		escapi_initialized = true;
	}

	SimpleCapParams scp = { reinterpret_cast<int*>(buffer), WIDTH, HEIGHT };
	if(!initCapture(DEVICE, &scp)) {
		cerr << "Couldn't initialize camera.  Get a better computer.\n";
		throw runtime_error("initCapture");
	}
}

video_stream::~video_stream() {
	deinitCapture(DEVICE);
}

rgb_byte video_stream::get_pix(int x, int y) const {
	const uint32_t* p = get_pix_ptr(const_cast<uint32_t*>(buffer), x, y);
	rgb_byte r = { (*p >> 16) & 0xFF, (*p >> 8) & 0xFF, *p & 0xFF };
	return r;
}

void video_stream::set_pix(int x, int y, const rgb_byte& pix) {
	uint32_t* p = get_pix_ptr(buffer, x, y);
	uint8_t* b = reinterpret_cast<uint8_t*>(p);

	// Video data is in BGRA order
	*b++ = pix.b;
	*b++ = pix.g;
	*b++ = pix.r;
	*b = pix.a;
}

// advance to the next frame and return whether that operation succeeded
bool video_stream::next_frame() {
	doCapture(DEVICE);
	return !isCaptureDone(DEVICE);
}