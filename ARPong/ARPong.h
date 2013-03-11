#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <process.h>
#include <GL/freeglut.h>
#include <escapi.h>

#include "library/player_class.h"
#include "library/glut_draw.h"
#include "library/glut_materials_lights.h"

// font strings for drawText2D();
//-------------------------------
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18

#define NU_PAUSE 10

// The width and height we are going to work at.  ESCAPI automatically scales
// larger images down to this.
enum { WIDTH = 800, HEIGHT = 600 };

static int glutClearStatus = 1;
static int glutAnimationStatus = 1;
static bool key_state[256] = {false};        // Key state container

const double pi = atan(1.0)*4.0;
const double rad = (pi/180.0);
const GLfloat moveSpeed=0.001;

// Buffer for copying video frams
int frame_buffer[WIDTH * HEIGHT];
// Use the first webcam device we find
// Change to support multiple devices
enum { DEVICE = 0 };

// Player and Enemy objects
player_class charPlayer(1);
player_class charEnemy(2);

GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0 };
GLfloat sun_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };

void glSetupOpenGL(int argc, char** argv);
void glDisplay(void);
void glReshape(int width, int height);
void glTimer(int value);
void glRunTimer(int isOn);
void glHidden(int isVisible);
void glKeyboard(unsigned char key, int xw, int yw);
void glKeyUpFunc(unsigned char key, int x, int y);
void glKeySpecial(int key, int xw, int yw);
void drawText2D(void *pFont, double x0, double y0, const char *pString);

void movePlayer(void);
void moveEnemy(void);

