#pragma once

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <GL/freeglut.h>
#include <escapi.h>

#include "CTransRot.h"
#include "objectsLightsMats.h"
#include "textfileProg.h"

// font strings for drawText2D();
//-------------------------------
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18

// The width and height we are going to work at.  ESCAPI automatically scales
// larger images down to this.
enum { WIDTH = 800, HEIGHT = 600 };
static int nu_Anim_isOn = 1;        // ==1 to run animation, ==0 to pause.
static int glCurrentCam=2;                // Cam Choice - change w/ 'c' key

const double pi = atan(1.0)*4.0;        // double constant for PI
const double rad = (pi/180.0);          // double constant for convert PI->RAD
static bool key_state[256] = { false };        // Key state container
float moveSpeed=0.001;           // Camera movement speed

#define NU_PAUSE 10 // wait time (in milliseconds (mS)) that we use for GLUT's

void glDisplay(void);                                 // GLUT display function
void glReshape( int width, int height );
void glKeyboard(unsigned char key, int xw, int yw);   // GLUT keyboard function
void glKeyUpFunc(unsigned char key, int x, int y);
void glKeySpecial(int key, int xw, int yw);           // GLUT special key function
void runAnimTimer(int isOn);      // 0: Stop, 1: Resume
void glTimer(int value);          // Used by runAnimTimer() to control animation
void glHidden(int isVisible);     // Called by GLUT each time the display
                                  // gets covered (visibility==0) or uncovered (==1)

int getDisplayHeight(void) { return HEIGHT; }
int getDisplayWidth(void)  { return WIDTH; }

void movePlayer(void);
void moveEnemy(void);
void drawScene(void);
void drawSphere(double r, int lats, int longs);
void drawAxes(int colr);
void drawText2D(void *pFont, double x0, double y0, const char *pString);