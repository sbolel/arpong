#pragma once
#include <iostream>
#include <GL/freeglut.h>

// font strings for drawText2D();
//-------------------------------
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18

#define NU_PAUSE 10 // wait time (in milliseconds (mS)) that we use for GLUT's

// The width and height we are going to work at.  ESCAPI automatically scales
// larger images down to this.
enum { WIDTH = 800, HEIGHT = 600 };

static int isCleared = 1; // toggle animation's screen-clear
static int nu_Anim_isOn = 1;        // ==1 to run animation, ==0 to pause.
static bool key_state[256] = {false};        // Key state container

void runAnimTimer(int isOn);
void glTimer(int value);
void glHidden(int isVisible);
void glKeyboard(unsigned char key, int xw, int yw);
void glKeyUpFunc(unsigned char key, int x, int y);
void glKeySpecial(int key, int xw, int yw);
void drawText2D(void *pFont, double x0, double y0, const char *pString);