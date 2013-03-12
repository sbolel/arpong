#pragma once

#include <atomic>
#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;

struct paddle {
	vec2 pos;
	vec2 dimensions;

	bool contains(const vec2&, float radius);
};

struct ball {
	vec3 pos;
	vec3 velocity;
	float radius;
};

struct game {
	ball b;
	paddle p1, p2;

	void update(float time);
};