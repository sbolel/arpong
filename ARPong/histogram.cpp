#include "histogram.h"

#include <iostream>
#include <numeric>

using namespace std;

namespace {
	// number of bins for each dimension of our histogram
	enum { BINS = 32 };

	// structure to hold rg coordinates
	struct rg { double r, g; };

	// Convert from RGB space to RG
	rg rgb_to_rg(const rgb& rgb) {
		double l = rgb.r + rgb.g + rgb.b;
		rg res = { };
		if(l) {
			res.r = rgb.r / l;
			res.g = rgb.g / l;
		}

		return res;
	}

	// Project a double from the continuous space [0, MAXVAL] onto the
	// discrete space [0, BINS)
	inline size_t project(double val, double MAXVAL) {
		return size_t(val / MAXVAL * BINS - .0001);
	}
}

// Initialize an empty histogram with BINS bins
histogram::histogram() : 
	h_(BINS, vector<double>(BINS, 0.0)) { }

int rgb_min = 30;
int rgb_max = 255 - rgb_min;

// Get the value for a given pixel
double histogram::value(const rgb& color) {
	if((color.b < rgb_min && color.g < rgb_min && color.r < rgb_min) ||
	   (color.b > rgb_max && color.g > rgb_max && color.r > rgb_max)) {
		   return 0;
	}
	rg rg = rgb_to_rg(color);
	return h_[project(rg.r, 1.0)][project(rg.g, 1.0)];
}

// Increase the value for the given color in this histogram
void histogram::increment_value(const rgb& color) {
	rg rg = rgb_to_rg(color);
	h_[project(rg.r, 1.0)][project(rg.g, 1.0)] += 1.0;
}

// Normalize the histogram such that the sum of all values is 1.0
void histogram::normalize() {
	double sum = 0.0;
	for(const auto& row : h_) {
		sum += accumulate(row.begin(), row.end(), 0.0);
	}
	for(auto& row : h_) {
		for(double& el : row) {
			el /= sum;
		}
	}
}

histogram load_histogram(std::istream& is) {
	histogram h;
	int r, g, b;
	char sep;
	while(is >> r >> sep >> g >> sep >> b >> sep) {
		rgb sample = { r, g, b };
		h.increment_value(sample);
	}

	h.normalize();
	return h;
}