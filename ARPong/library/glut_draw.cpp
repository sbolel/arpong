#include "glut_draw.h"

void drawAxes(int colr){
// When 'colr' = 0 draws red,grn,blu coordinate system axes;
// Otherwise draws       cyan,magenta,yellow axes
  glDisable(GL_LIGHTING);//-----------------Draw axes:
    glBegin(GL_LINES);      // start drawing lines:
            if(colr) glColor3f( 0.0, 1.0, 1.0); // cyan or
            else     glColor3f( 1.0, 0.0, 0.0); // Red X axis
      glVertex3f( 0.0, 0.0, 0.0);
      glVertex3f( 1.0, 0.0, 0.0);

            if(colr) glColor3f( 1.0, 0.0, 1.0); // Magenta or
            else     glColor3f( 0.0, 1.0, 0.0); // Green Y axis
      glVertex3f( 0.0, 0.0, 0.0);
      glVertex3f( 0.0, 1.0, 0.0);

            if(colr) glColor3f( 1.0, 1.0, 0.0); // Yellow or
            else     glColor3f( 0.0, 0.0, 1.0); // Blue Z axis
      glVertex3f( 0.0, 0.0, 0.0);
      glVertex3f( 0.0, 0.0, 1.0);
    glEnd();          // end drawing lines
  glEnable(GL_LIGHTING);// ------------DONE drawing axes.
}

void drawScene(void) {
  glPushMatrix();
    glBegin(GL_QUADS);
      glColor3f(0.0, 0.25, 0.8);
      glVertex3f( 5.0, 2.0, 0.0);
      glVertex3f( 5.0, -2.0, 0.0);
      glVertex3f( 5.0, -2.0, 30.0);
      glVertex3f( 5.0, 2.0, 30.0);
    glEnd();
    glBegin(GL_QUADS);
      glColor3f(0.0, 0.25, 0.8);
      glVertex3f( -5.0, 2.0, 0.0);
      glVertex3f( -5.0, -2.0, 0.0);
      glVertex3f( -5.0, -2.0, 30.0);
      glVertex3f( -5.0, 2.0, 30.0);
    glEnd();
    // glBegin(GL_QUADS);
    //   glVertex3f( 10.0, 6.0, -50.0);
    //   glVertex3f( -10.0, 6.0, -50.0);
    //   glVertex3f( -10.0, -6.0, -50.0);
    //   glVertex3f( 10.0, -6.0, -50.0);
    // glEnd();
    // glBegin(GL_QUADS);
    //   glVertex3f( 10.0, 6.0, 0.0);
    //   glVertex3f( 10.0, -6.0, 0.0);
    //   glVertex3f( 10.0, -6.0, -50.0);
    //   glVertex3f( 10.0, 6.0, -50.0);
    // glEnd();
  glPopMatrix();
}