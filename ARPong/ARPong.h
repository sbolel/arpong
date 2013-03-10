#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <escapi.h>

// The width and height we are going to work at.  ESCAPI automatically scales
// larger images down to this.
enum { WIDTH = 800, HEIGHT = 600 };


// font strings for drawText2D();
// (use as value for pFont argument)
//-------------------------------
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18

#define NU_PAUSE 10 // wait time (in milliseconds (mS)) that we use for GLUT's

void glDisplay(void);                                 // GLUT display function
void glKeyboard(unsigned char key, int xw, int yw);   // GLUT keyboard function
void glKeySpecial(int key, int xw, int yw);           // GLUT special key function
void runAnimTimer(int isOn);      // 0: Stop, 1: Resume
void myTimer(int value);          // Used by runAnimTimer() to control animation
void myHidden(int isVisible);     // Called by GLUT each time the display
                                  // gets covered (visibility==0) or uncovered (==1)

int getDisplayHeight(void) { return HEIGHT; }
int getDisplayWidth(void)  { return WIDTH; }

void drawText2D(void *pFont, double x0, double y0, const char *pString);


