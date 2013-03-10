#include "ARPong.h"
using namespace std;

// Use the first webcam device we find.  Change this if we ever need to
// support multiple devices.
enum { DEVICE = 0 };

// Buffer into which video frames will be copied.
int frame_buffer[WIDTH * HEIGHT];
int isCleared = 1;                  // toggle animation's screen-clear

CTransRot charPlayer(1);       // Mouse/Keyboard settings for camera coord system.
CTransRot charEnemy(2);       // Mouse/Keyboard settings for camera coord system.

// Set up the image capture library ESCAPI
void setup_escapi() {
	// Initialize the DLL and ask how many webcams it found
	int devices = setupESCAPI();
	if(!devices) {
		cerr << "ESCAPI found no devices.\n";
		throw runtime_error("setupESCAPI");
	}
	// Tell the library where our buffer is and how big it is
	SimpleCapParams params = { frame_buffer, WIDTH, HEIGHT };
	if(!initCapture(DEVICE, &params)) {
		cerr << "initCapture failed, please install a better webcam\n";
		throw runtime_error("initCapture");
	}
}

bool main_loop_iter() {
	doCapture(DEVICE);

	// If we got a video frame, render it and run the game
	if(!isCaptureDone(DEVICE)) {
		/// TODO: Game logic and networking calls go here

		// Tell GLUT to render this frame and manually proceed to the next one
		glutPostRedisplay();
		glutMainLoopEvent();
		return true;
	}
	// Can't capture more video, exit the program
	else {
		return false;
	}
}

int main(int argc, char** argv) {
	// setup_escapi();
  GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0 };
  GLfloat sun_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
  GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };

	glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ARPong");

  glutDisplayFunc(glDisplay);
  glutIdleFunc(glDisplay);        // GLUT idle function
	glutReshapeFunc(glReshape);
	glutKeyboardFunc(glKeyboard);
  glutKeyboardUpFunc(glKeyUpFunc);// keystate function
	glutSpecialFunc(glKeySpecial);
  glutIgnoreKeyRepeat(true);
  glEnable (GL_DEPTH_TEST);       // enable DEPTH test

  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
  glEnable(GL_LIGHT0);                // Set up sunlight.
  glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);
  glEnable(GL_COLOR_MATERIAL);        // Configure glColor().
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	GLuint texture;
  glClearColor(0.03, 0.03, 0.2, 0.0);     // Dark blue background set
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glShadeModel(GL_SMOOTH);
  runAnimTimer(1);                // start our animation loop.

  // keep running until the video source quits or someone closes us
  // while(main_loop_iter()) { }

  glutMainLoop();                 // enter GLUT's event-handler; NEVER EXITS.

	// deinitCapture(DEVICE);
  return 0;                       // SUCCESSFUL exit, C++ Compatibility
}


void glReshape( int width, int height )
//------------------------------------------------------------------------------
{
  glViewport((WIDTH-HEIGHT)/2,0, HEIGHT, HEIGHT);
  glMatrixMode(GL_PROJECTION);  // select camera-setting matrix stack
  glLoadIdentity();             // clear it: identity matrix.
  gluPerspective (45, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);        //return to modelview coords
  glutPostRedisplay();      // request redraw--we changed window size!
}


// OpenGL display function, called whenever a new frame is requested
void glDisplay() {
  movePlayer();
  moveEnemy();
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity ();
  gluLookAt(  0.0f, 0.0f, -2.0f,
      0.0f, 0.0f,  30.0f,
      0.0f, 1.0f,  2.0f);
  // vertices for simple textured-quad rendering
  const struct { float tu, tv; float x, y, z; } vertices[] = {
    { 0.0f, 0.0f, -1.0f,-1.0f, 0.0f },
    { 1.0f, 0.0f,  1.0f,-1.0f, 0.0f },
    { 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }
  };

// Movement and Rotation
  drawScene();
  drawAxes(0);
  glPushMatrix();
    charPlayer.apply_T_Matrix();
  	// glScaled(-1.0, -1.0, 0.0);
  	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, frame_buffer);
  	// glInterleavedArrays(GL_T2F_V3F, 0, vertices);
  	// glDrawArrays(GL_QUADS, 0, 4);
    drawAxes(0);
	glPopMatrix();
  glPushMatrix();
    glTranslated(0.0,0.0,30.0);
    glRotated(180, 0.0,1.0,0.0);
    charEnemy.apply_T_Matrix();
    // glScaled(-1.0, -1.0, 0.0);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, frame_buffer);
    // glInterleavedArrays(GL_T2F_V3F, 0, vertices);
    // glDrawArrays(GL_QUADS, 0, 4);
    drawAxes(0);
  glPopMatrix();
  glPushMatrix();
    glTranslated(0.0,0.0,13.0);
    glColor3f(0.0, 1.0, 0.25);
    glutSolidSphere(0.5, 20, 20);
  glPopMatrix();

	glFlush();	                // do any and all pending openGL rendering.
	glutSwapBuffers();
}


void glKeyboard(unsigned char key, int xw, int yw)
//------------------------------------------------------------------------------
{
  key_state[key] = true;
  int xpos,ypos;  // mouse position in coords with origin at lower left.

  xpos = xw;
  ypos = getDisplayHeight() - yw; //(window system puts origin at UPPER left)

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
		case ' ':		          // User pressed the spacebar.
		case 27:		          // User pressed the 'Esc'  key...
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
void glKeyUpFunc(unsigned char key, int x, int y){
    key_state[key] = false;
}

void glKeySpecial(int key, int xw, int yw)
//------------------------------------------------------------------------------
{
int xpos,ypos;      // mouse position in coords with origin at lower left.
    xpos = xw;
    ypos = getDisplayHeight() - yw; //(window system puts origin at UPPER left)
	switch(key)
	{
    case GLUT_KEY_F1:       // DISPLAY HELP CONTENTS
      cout << "\nARPong Help File (Press F1 to Resume)\n";
      cout << "----------------------------\n";   // Display HELP contents
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
//  Using the current color and specified font, write the text in 'pString' to
// the display window.  Place the lower left corner of the first line of text
//  at screen position x0,y0 in the current coordinate system.
//      Example:  DrawText2D(helv18, 0.3,0.4 ,"Hello!");
//
// Available fonts:
// helv10 (HELVETICA size 10)   rom10 (TIMES_ROMAN size 10)
// helv12 (HELVETICA size 12)   rom24 (TIMES_ROMAN size 24)
// helv18 (HELVETICA size 18)
{
  int i, imax;					// character counters
	glRasterPos2d(x0, y0);		// set text's lower-left corner position
	imax = 1023;				// limit the number of chars we print.
	for(i=0; pString[i] != '\0' && i<imax; i++)	// for each char,
	{
		glutBitmapCharacter(pFont, pString[i]);
	}
}

void runAnimTimer(int isOn)
//-----------------------------------------------------------------------------
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
//------------------------------------------------------------------------------
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

void glHidden (int isVisible)
//------------------------------------------------------------------------------
// Called by GLUT when our on-screen window gets covered or un-covered. When
// called, it turns off animation and re-display when our display window is
// covered or stowed (not visible). SEE ALSO: runAnimTimer() and glTimer()
{
    if(isVisible == GLUT_VISIBLE)  runAnimTimer(1); // Visible! YES! Animate!
    else runAnimTimer(0);                           // No. Stop animating.
}

void movePlayer(void){
float yaw = charPlayer.y_rot;
  if(key_state['a'] == true) {
      charPlayer.x_pos += moveSpeed;
  }
  if(key_state['d'] == true) {
      charPlayer.x_pos -= moveSpeed;
  }
  if(key_state['w'] == true) {
    charPlayer.y_pos += moveSpeed;
  }
  if(key_state['s'] == true) {
    charPlayer.y_pos -= moveSpeed;
  }
  if(key_state['z'] == true) {
    charPlayer.z_pos += moveSpeed;
  }
  if(key_state['x'] == true) {
    charPlayer.z_pos -= moveSpeed;
  }
}

void moveEnemy(void){
float yaw = charEnemy.y_rot;
  if(key_state['f'] == true) {
    charEnemy.x_pos -= moveSpeed;
  }
  if(key_state['h'] == true) {
    charEnemy.x_pos += moveSpeed;
  }
  if(key_state['t'] == true) {
    charEnemy.y_pos += moveSpeed;
  }
  if(key_state['g'] == true) {
    charEnemy.y_pos -= moveSpeed;
  }
  if(key_state['z'] == true) {
    charEnemy.z_pos += moveSpeed;
  }
  if(key_state['x'] == true) {
    charEnemy.z_pos -= moveSpeed;
  }
}

void drawAxes(int colr){
//------------------------------------------------------------------------------
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

void drawSphere(double r, int lats, int longs) {
    int i, j;
    for(i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0);
        double zr0 =  cos(lat0);

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0, y * zr0, z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);
        }
        glEnd();
    }
 }