#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/freeglut.h>
#include <escapi.h>

#include "library/player_class.h"
#include "library/glut_callbacks.h"
#include "library/glut_draw.h"
#include "library/glut_materials_lights.h"

const double pi = atan(1.0)*4.0;
const double rad = (pi/180.0);
const float moveSpeed=0.001;

// Buffer for copying video frams
int frame_buffer[WIDTH * HEIGHT];
// Use the first webcam device we find
// Change to support multiple devices
enum { DEVICE = 0 };

// Player and Enemy objects
player_class charPlayer(1);
player_class charEnemy(2);

void glDisplay(void);
void glReshape(int width, int height);

void movePlayer(void);
void moveEnemy(void);