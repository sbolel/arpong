#include "objectDetection.h"

center_t calculate_median(const frame& img) {
	double x_sum = 0.0, y_sum = 0.0;
	int pixels = 0;
	for(int x = 0; x < WIDTH; ++x) {
		for(int y = 0; y < HEIGHT; ++y) {
			const uint8_t* pix = img.get_pixel(x, y);
			if(!is_black(pix)) {
				x_sum += x;
				y_sum += y;
				++pixels;
			}
		}
	}

	x_sum /= pixels;
	y_sum /= pixels;
	center_t res = { x_sum, y_sum };
	return res;
}

void calculate_median(const frame& img, int &paddleX, int &paddleY) {
	//given a buffer with a skin detection filter applied to it,
	// search for the x and y coordinates that approximately bound
	// a hand - where there is skin detected on  X% of a horizontal
	// line and y% of a vertical line. 
	//--dependent on the hand being the primary thing in the picture. 
	int xmin, xmax, ymin, ymax;
	int y_threshold = WIDTH/5;
	for(int j = 0; j < HEIGHT; j++) {
	    int count = 0;
		for(int i=0; i<WIDTH;i++) {
			const uint8_t* pix = img.get_pixel(i, j);
			if(pix[0] || pix[1] || pix[2])
				count++;
		}
		if (count > y_threshold) {
			ymin = j;
			break;
		}
	}
	for(int j=HEIGHT-1; j >= 0; j--) {
	   int count = 0;
	   for(int i=0; i<WIDTH;i++) {
		    const uint8_t* pix = img.get_pixel(i, j);
			if(pix[0] || pix[1] || pix[2])
				count++;
	   }
	   if (count > y_threshold) {
		  ymax = j;
		  break;
	   }
	}
	paddleY = (ymin + ymax) / 2;
	int x_threshold = (ymax - ymin)/2;
	for(int i=0; i<WIDTH; i++) {
	    int count = 0;
		for(int j = ymin; j < ymax;j++) {
			const uint8_t* pix = img.get_pixel(i, j);
			if(pix[0] || pix[1] || pix[2])
				count++;
		}
		if (count > x_threshold) {
			xmin = i;
			break;
		}
	}
	for(int i = WIDTH-1; i>=0; i--) {
	    int count=0;
		for(int j=ymin;j<ymax;j++) {
			const uint8_t* pix = img.get_pixel(i, j);
			if(pix[0] || pix[1] || pix[2])
				count++;
		}
		if(count > y_threshold) {
			xmax = i;
			break;
		}
	}
	paddleX = (xmin + xmax) / 2;
}