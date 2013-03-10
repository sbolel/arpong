#pragma once

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <escapi.h>

#include "lib/CTransRot.h"

// The width and height we are going to work at.  ESCAPI automatically scales
// larger images down to this.
enum { WIDTH = 800, HEIGHT = 600 };
static int nu_Anim_isOn = 1;        // ==1 to run animation, ==0 to pause.

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
void glReshape( int width, int height );
void glKeyboard(unsigned char key, int xw, int yw);   // GLUT keyboard function
void glKeySpecial(int key, int xw, int yw);           // GLUT special key function
void runAnimTimer(int isOn);      // 0: Stop, 1: Resume
void glTimer(int value);          // Used by runAnimTimer() to control animation
void glHidden(int isVisible);     // Called by GLUT each time the display
                                  // gets covered (visibility==0) or uncovered (==1)

int getDisplayHeight(void) { return HEIGHT; }
int getDisplayWidth(void)  { return WIDTH; }

void drawText2D(void *pFont, double x0, double y0, const char *pString);


void drawHelpText(void)
{
  glPushMatrix();
      glLoadIdentity();
      glScaled(0.2,0.2,0.2);
      glColor3d(0.0, 1.0, 0.0); // And cyan-colored text on-screen:
      drawText2D(helv18, -5.0, 4.0,"ARPong HELP (Press F1 to Resume)");
      drawText2D(helv12, -5.0, 4.0-0.2,"________________________________");
      drawText2D(helv12, -5.0, 4.0-0.6,"Sample help text");
      drawText2D(helv12, -5.0, 4.0-0.9,"Sample help text");
      drawText2D(helv12, -5.0, 4.0-1.2,"Sample help text");
      drawText2D(helv12, -5.0, 4.0-1.5,"Sample help text");
      drawText2D(helv12, -5.0, 4.0-1.8,"Sample help text");
      drawText2D(helv12, -5.0, 4.0-2.1,"Sample help text");
                  if(nu_Anim_isOn ==1) runAnimTimer(0);
                  else runAnimTimer(1);
  glPopMatrix();
}