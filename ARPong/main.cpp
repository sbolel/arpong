#include "ARPong.h"
using namespace std;

// Global player variables
player_class player(1);
player_class enemy(2);
player_class ball(3);

void updateClient(server_data sd){
  enemy.x = sd.x;
  ball.x = sd.b_x;
  ball.z = sd.b_z;
}

void tcp_comm(void *) {
  WSACleanup();
  cout << "---------------------------------\n";
  if (player.id==1) {
    ListenOnPort(9000);
    while(1) {
      Sleep(20);
      ServerWrite(player.x, ball.x, ball.z);
      enemy.x = ServerRead();
    }
  }
  else {
    bool connect_status = 0;
    while (connect_status==0) {
      cout << "Enter server IP: ";
      std::string ip_str;
      getline(cin,ip_str,'\n');
      char * server_ip = new char[ip_str.size()+1];
      std::copy(ip_str.begin(), ip_str.end(), server_ip);
      connect_status = ConnectToHost(9000, server_ip);
      delete server_ip;
    }
    while(1) {
      Sleep(30);
      updateClient(ClientRead());
      ClientWrite(player.x);
    }
  }
}

void setup_player(void) {
  cout << "Enter player number: ";
  std::string id;
  getline(cin,id,'\n');
  player.id = std::stoi(id);
  ball.x = 0.0;
  ball.z = 15.0;
}


int main(int argc, char** argv) {
  setup_player();
  glSetupOpenGL(argc, argv);
  _beginthread(tcp_comm, 0, (void*)1);
  glutMainLoop();
  return 0;
}

void glDisplay() {

  moveObjects();

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();



// Movement and Rotation
  drawScene();

  glPushMatrix();
    player.Tx();
    drawAxes(0);
	glPopMatrix();
  glPushMatrix();
    glTranslated(0.0,0.0,30.0);
    glRotated(180, 0.0,1.0,0.0);
    enemy.Tx();
    drawAxes(0);
  glPopMatrix();
  glPushMatrix();
    glColor3f(0.0, 1.0, 0.25);
    if(player.id==2) {
      glRotated(180, 0.0,1.0,0.0);
      glTranslated(0.0,0.0,-30.0);
    }
    ball.Txz();
    glutSolidSphere(0.25, 20, 20);
    glScaled(0.7,0.7,0.7);
    drawAxes(0);
  glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void glReshape( int width, int height ) {
  glViewport((WIDTH-HEIGHT)/2,0, HEIGHT, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (45, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0, 15.0);
  glMatrixMode(GL_MODELVIEW);
  glutPostRedisplay();
}

void glKeyboard(unsigned char key, int xw, int yw) {
  key_state[key] = true;
  switch(key) {
    case'a': case's':
      cout << "Move player\n";
      break;
    case'd': case'f':
      cout << "Move enemy\n";
      break;
    case'j': case'k':
      cout << "Move ball\n";
      break;
    case 'c':
      if(glutClearStatus==0) glutClearStatus = 1;
      else                   glutClearStatus = 0;
      break;
    case 'p':
      if(glutAnimationStatus ==1) glRunTimer(0);
      else                        glRunTimer(1);
      break;
    case ' ': case 27: case 'Q': case 'q':
      exit(0);
      break;
    default:
      cout << "Invalid key\n";
      break;
  }
}

void glKeyUpFunc(unsigned char key, int x, int y) {
    key_state[key] = false;
}

void glKeySpecial(int key, int xw, int yw) {
  // mouse pos in coords with origin at lower left
  //(window system puts origin at UPPER left)
  int xpos,ypos;
      xpos = xw;
      ypos = HEIGHT - yw;
  switch(key) {
    default:
      // cout << "Special Key Pressed, Value: "<< (int)key << "\n)";
      break;
  }
}

void drawText2D(void *pFont, double x0, double y0, const char *pString) {
  int i, imax;
  glRasterPos2d(x0, y0);
  imax = 1023;
  for(i=0; pString[i] != '\0' && i<imax; i++) {
    glutBitmapCharacter(pFont, pString[i]);
  }
}

void glHidden (int isVisible) {
    if(isVisible == GLUT_VISIBLE)
      glRunTimer(1);
    else
      glRunTimer(0);
}

void glRunTimer(int isOn) {
    glutAnimationStatus = isOn;
    if(isOn == 1)
      glutTimerFunc(NU_PAUSE, glTimer, 1);
}

void glTimer (int value) {
    if(value != glutAnimationStatus) {
        cout << "\n(Anim cancelled during GLUT wait)\n";
        glutPostRedisplay();
        glRunTimer(1);
    }
}

void glSetupOpenGL(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("ARPong");
  glutDisplayFunc(glDisplay);
  glutIdleFunc(glDisplay);
  glutReshapeFunc(glReshape);
  glutKeyboardFunc(glKeyboard);
  glutKeyboardUpFunc(glKeyUpFunc);
  glutSpecialFunc(glKeySpecial);
  glutIgnoreKeyRepeat(true);

  glEnable (GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);          // Set up sunlight.
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_TEXTURE_2D);

  GLuint texture;
  glShadeModel(GL_SMOOTH);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
  glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glClearColor(0.03, 0.03, 0.2, 0.0);
  glRunTimer(1);
}

void moveObjects(void){
  if (player.id==1)
    moveBall();
  movePlayer();
}

void movePlayer(void) {
  if(key_state['a'] == true) {
    player.xInc(moveSpeed);
  }
  if(key_state['s'] == true) {
    player.xDec(moveSpeed);
  }
}

void moveEnemy(void) {
  if(key_state['f'] == true) {
    enemy.xDec(moveSpeed);
  }
  if(key_state['d'] == true) {
    enemy.xInc(moveSpeed);
  }
}

void moveBall(void) {
  if(key_state['j'] == true) {
      ball.x += moveSpeed;
  }
  if(key_state['k'] == true) {
      ball.x -= moveSpeed;
  }
  if(key_state['i'] == true) {
      ball.z += moveSpeed;
  }
  if(key_state['m'] == true) {
      ball.z -= moveSpeed;
  }
}