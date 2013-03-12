#pragma once

#include <glm/glm.hpp>

struct client_data {
	glm::vec2 paddle;
};

struct server_data {
	glm::vec2 paddle;
	glm::vec3 ball;
};