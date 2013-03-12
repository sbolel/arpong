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

  // Left wall
    glBegin(GL_QUADS);  // right
      glColor3f(0.9f, 0.6f, 0.125f);   // light brown
      glVertex3f( ARENA_WIDTH, ARENA_HEIGHT, 0.0f);
      glVertex3f( ARENA_WIDTH, -ARENA_HEIGHT, 0.0f);
      glVertex3f( ARENA_WIDTH, -ARENA_HEIGHT, ARENA_LENGTH);
      glVertex3f( ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
    glEnd();
    glBegin(GL_QUADS);  // back
      glColor3f(0.7f, 0.1f, 0.1f);   // dark brown
      glVertex3f( ARENA_WIDTH, ARENA_HEIGHT, 0.0f);
      glVertex3f( ARENA_WIDTH+WALL_THICK, ARENA_HEIGHT, 0.0f);
      glVertex3f( ARENA_WIDTH+WALL_THICK, -ARENA_HEIGHT, 0.0f);
      glVertex3f( ARENA_WIDTH, -ARENA_HEIGHT, 0.0f);
    glEnd();
    glBegin(GL_QUADS);  // top
      glColor3f(0.9f, 0.6f, 0.125f);   // light brown
      glVertex3f( ARENA_WIDTH, ARENA_HEIGHT, 0.0f);
      glVertex3f( ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
      glVertex3f( ARENA_WIDTH+WALL_THICK, ARENA_HEIGHT, ARENA_LENGTH);
      glVertex3f( ARENA_WIDTH+WALL_THICK, ARENA_HEIGHT, 0.0f);
    glEnd();

  // right wall
    glBegin(GL_QUADS);  // left
      glColor3f(0.9f, 0.6f, 0.125f);   // light brown
      glVertex3f( -ARENA_WIDTH, ARENA_HEIGHT, 0.0f);
      glVertex3f( -ARENA_WIDTH, -ARENA_HEIGHT, 0.0f);
      glVertex3f( -ARENA_WIDTH, -ARENA_HEIGHT, ARENA_LENGTH);
      glVertex3f( -ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
    glEnd();
    glBegin(GL_QUADS);  // back
      glColor3f(0.7f, 0.1f, 0.1f);   // dark brown
      glVertex3f( -ARENA_WIDTH, ARENA_HEIGHT, 0.0f);
      glVertex3f( -ARENA_WIDTH, -ARENA_HEIGHT, 0.0f);
      glVertex3f( -ARENA_WIDTH-WALL_THICK, -ARENA_HEIGHT, 0.0f);
      glVertex3f( -ARENA_WIDTH-WALL_THICK, ARENA_HEIGHT, 0.0f);
    glEnd();
    glBegin(GL_QUADS);  // top
      glColor3f(0.9f, 0.6f, 0.125f);   // light brown
      glVertex3f( -ARENA_WIDTH, ARENA_HEIGHT, 0.0f);
      glVertex3f( -ARENA_WIDTH-WALL_THICK, ARENA_HEIGHT, 0.0f);
      glVertex3f( -ARENA_WIDTH-WALL_THICK, ARENA_HEIGHT, ARENA_LENGTH);
      glVertex3f( -ARENA_WIDTH, ARENA_HEIGHT, ARENA_LENGTH);
    glEnd();

  //floor
    glBegin(GL_QUADS);  // left
      glColor3f(0.3f, 0.1f, 0.1f);   // dark
      glVertex3f( +ARENA_WIDTH, -ARENA_HEIGHT, 0.0f);
      glVertex3f( -ARENA_WIDTH, -ARENA_HEIGHT, 0.0f);
      glVertex3f( -ARENA_WIDTH, -ARENA_HEIGHT, ARENA_LENGTH);
      glVertex3f( +ARENA_WIDTH, -ARENA_HEIGHT, ARENA_LENGTH);
    glEnd();
    // glBegin(GL_QUADS);
    //   glVertex3f( 3.0, 7.0, -50.0);
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