#include <iosfwd>
#include <vector>

#include "ARPong.h"

struct rgb { double r, g, b; };

class histogram {
public:
	typedef std::vector<std::vector<double>> hist_data_t;

	histogram();
	double value(const rgb& color);
	void increment_value(const rgb& color);
	void normalize();
	void print() const;

private:
	std::vector<std::vector<double>> h_;
};

histogram load_histogram(std::istream&);