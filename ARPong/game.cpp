#include "game.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace glm;

// Player 1 is at Z=-1, Player 2 is at Z=1

const vec3 MIN(-1, -1, -1), MAX(1, 1, 1);

void game::update(float time) {
	b.pos += b.velocity * time;

	// hit the X boundary
	if(b.pos.x - b.radius < MIN.x) {
		b.pos.x = MIN.x;
		b.velocity.x *= -1;
	}
	else if(b.pos.x + b.radius > MAX.x) {
		b.pos.x = MAX.x;
		b.velocity.x *= -1;
	}

	// Hit the Y boundary
	if(b.pos.y - b.radius < MIN.y) {
		b.pos.y = MIN.y;
		b.velocity.y *= -1;
	}
	else if(b.pos.y + b.radius > MAX.y) {
		b.pos.y = MAX.y;
		b.velocity.y *= -1;
	}

	// Hit Player 1
	if(b.pos.z - b.radius < MIN.z) {
		if(p1.contains(vec2(b.pos), b.radius)) {
			b.velocity.z *= -1;
			b.pos.z = MIN.z;
		}
		else {
			cout << "Player 1 loses!\n";
		}
	}
	// Hit Player 2
	else if(b.pos.z + b.radius > MAX.z) {
		if(p2.contains(vec2(b.pos), b.radius)) {
			b.velocity.z *= -1;
			b.pos.z = MAX.z;
		}
		else {
			cout << "Player 2 loses!\n";
		}
	}
}

bool paddle::contains(const vec2& b, float radius) {
	using std::abs;
	if(abs(b.x - pos.x) > radius + dimensions.x) {
		return false;
	}
	else if(abs(b.y - pos.y) > radius + dimensions.y) {
		return false;
	}
	return true;
}
