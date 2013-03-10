#include "ARPong.h"
using namespace std;

// Use the first webcam device we find.  Change this if we ever need to
// support multiple devices.
enum { DEVICE = 0 };

// Buffer into which video frames will be copied.
int frame_buffer[WIDTH * HEIGHT];

static int nu_Anim_isOn = 1;        // ==1 to run animation, ==0 to pause.
int isCleared = 1;                  // toggle animation's screen-clear

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

	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ARPong");
	glutDisplayFunc(glDisplay);

	GLuint texture;
	glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, WIDTH, HEIGHT);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// keep running until the video source quits or someone closes us
	// while(main_loop_iter()) { }

	glutMainLoop();	                // enter GLUT's event-handler; NEVER EXITS.
  runAnimTimer(1);                // start our animation loop.

	// deinitCapture(DEVICE);
}

// OpenGL display function, called whenever a new frame is requested
void glDisplay() {
	// vertices for simple textured-quad rendering
	const struct { float tu, tv; float x, y, z; } vertices[] = {
		{ 0.0f, 0.0f, -1.0f,-1.0f, 0.0f },
		{ 1.0f, 0.0f,  1.0f,-1.0f, 0.0f },
		{ 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }
	};

	glPushMatrix();
	glScaled(-1.0, -1.0, 0.0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, frame_buffer);

	glInterleavedArrays(GL_T2F_V3F, 0, vertices);
	glDrawArrays(GL_QUADS, 0, 4);

	glPopMatrix();

	glFlush();	                // do any and all pending openGL rendering.
	glutSwapBuffers();
}


void myKeySpecial(int key, int xw, int yw)
//------------------------------------------------------------------------------
{
int xpos,ypos;      // mouse position in coords with origin at lower left.
    xpos = xw;
    ypos = getDisplayHeight() - yw; //(window system puts origin at UPPER left)
	switch(key)
	{
        case GLUT_KEY_F1:       // DISPLAY HELP CONTENTS
                cout << "Help File (F1 Key Pressed)\n";
                glPushMatrix();
                    glLoadIdentity();
                    glScaled(0.2,0.2,0.2);
                    glColor3d(0.0, 1.0, 0.0);	// And cyan-colored text on-screen:


                    drawText2D(helv18,1.9, 3.0 +1.5,"HELP (Press F1 to Resume)");
                    drawText2D(helv12,1.9, 2.85+1.5, "Press L mouse button to increase size");
                    drawText2D(helv12,1.9, 2.7 +1.5, "Press R mouse button to decrease size");
                    drawText2D(helv12,1.9, 2.55+1.5, "Press M mouse button to reset size");
                    drawText2D(helv12,1.9, 2.4 +1.5, "Press R to toggle refreshing the screen");
                    drawText2D(helv12,1.9, 2.25+1.5, "Press P to toggle the animation");
                    drawText2D(helv12,1.9, 2.1+1.5,  "Press (.) to speed up the wheel clockwise");
                    drawText2D(helv12,1.9, 1.95+1.5, "Press (,) to speed up the wheel counter-cw");
                                if(nu_Anim_isOn ==1) runAnimTimer(0);
                                else runAnimTimer(1);
                glPopMatrix();
                cout << "\nHELP FILE DISPLAYED!\n";
            break;
		default:
			cout << "Special Key Pressed, Value: "<< (int)key << "\n)";
			break;
	}
    glFlush();	        // do any and all pending openGL rendering.
    glutSwapBuffers();	// For double-buffering: show what we drew.
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
        glutTimerFunc(NU_PAUSE, myTimer, 1);
    }
}

void myTimer (int value)
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