#include "glut_callbacks.h"
using namespace std;

void glHidden (int isVisible)
// Called by GLUT when our on-screen window gets covered or un-covered. When
// called, it turns off animation and re-display when our display window is
// covered or stowed (not visible). SEE ALSO: runAnimTimer() and glTimer()
{
    if(isVisible == GLUT_VISIBLE)  runAnimTimer(1); // Visible! YES! Animate!
    else runAnimTimer(0);                           // No. Stop animating.
}

void runAnimTimer(int isOn)
// Call runAnimTimer(0) to STOP animation;
//      runAnimTimer(1) to START or CONTINUE it.
{
    nu_Anim_isOn = isOn;   // Update global var to the most-recent state;
    if(isOn == 1)          // Start or continue animation?
    {
        glutTimerFunc(NU_PAUSE, glTimer, 1);
    }
}

void glTimer (int value)
{
    // GLUT finished waiting.
    if(value != nu_Anim_isOn)
        cout << "\n(Anim cancelled during GLUT wait)\n";
    if(nu_Anim_isOn==1)         // Go on with animation?
    {                           // YES.
        glutPostRedisplay();    // tell GLUT to re-draw the screen, and
        runAnimTimer(1);        // continue with the next time interval.
    }
    // NO. Do nothing. No further animation.
}

void glKeyboard(unsigned char key, int xw, int yw)
{
  key_state[key] = true;
  int xpos,ypos;  // mouse position in coords with origin at lower left.

  xpos = xw;
  ypos = HEIGHT - yw; //(window system puts origin at UPPER left)

  switch(key)
  {
    case'a':
    case's':
    case'd':
    case'w':
      break;
    case 'r':
    case 'R':             // toggle 'isCleared' to enable/disable screen clearing
      if(isCleared==0) isCleared = 1;
      else isCleared = 0;
      break;
    case 'p':
    case 'P':             // Toggle animation on/off.
      if(nu_Anim_isOn ==1) runAnimTimer(0);
      else runAnimTimer(1);
      break;
    case ' ':             // User pressed the spacebar.
    case 27:              // User pressed the 'Esc'  key...
    case 'Q':             // User pressed the 'Q' key...
    case 'q':
      exit(0);            // Done! quit the program.
      break;
    default:
      cout << "Invalid key pressed!\n";
      glColor3d(0.0, 1.0, 1.0);// And Cyan-colored text on-screen:
      drawText2D(helv18, 2.75, -4.75,"Wrong Key Pressed");
      //===============DRAWING DONE.
      break;
  }
}

void glKeyUpFunc(unsigned char key, int x, int y)
{
    key_state[key] = false;
}

void glKeySpecial(int key, int xw, int yw)
{
int xpos,ypos;      // mouse position in coords with origin at lower left.
    xpos = xw;
    ypos = HEIGHT - yw; //(window system puts origin at UPPER left)
  switch(key)
  {
    case GLUT_KEY_F1:
      cout << "\nARPong Help File (Press F1 to Resume)\n";
      cout << "----------------------------\n";
      cout << "() \n";
      cout << "() \n";
      break;
    default:
      cout << "Special Key Pressed, Value: "<< (int)key << "\n)";
      break;
  }
}

void drawText2D(void *pFont, double x0, double y0, const char *pString)
//-----------------------------------------------------------------------------
// helv10 (HELVETICA size 10)   helv12 (HELVETICA size 12)  helv18 (HELVETICA size 18)
// rom10 (TIMES_ROMAN size 10)  rom24 (TIMES_ROMAN size 24)
{
  int i, imax;          // character counters
  glRasterPos2d(x0, y0);    // set text's lower-left corner position
  imax = 1023;        // limit the number of chars we print.
  for(i=0; pString[i] != '\0' && i<imax; i++) // for each char,
  {
    glutBitmapCharacter(pFont, pString[i]);
  }
}