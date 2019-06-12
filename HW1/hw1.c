/* Evan James ---- CSCI 4229
* HW1 = Lorenz Attractor
*
* Axes drawing modified from EX6 in class
*
* Key bindings:
* s ---------- Decrement Lorenz s value by .25
* S ---------- Increment Lorenz s value by .25
* b ---------- Decrement Lorenz b value by .05
* B ---------- Increment Lorenz b value by .05
* r ---------- Decrement Lorenz r value by .5
* R ---------- Increment Lorenz r value by .5
* 1 ---------- Reset Lorenz parapmeters to default values
* 2 ---------- Reset view angle to x,y
* 3 ---------- Reset view angle to x,z
* 4 ---------- Reset view angle to y,z
* t ---------- Toggle tracer on/off
* Arrow keys - Change view angle
* ESC -------- Exit
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
double dim=2;   // Dimension of orthogonal box

/* Lorzen params, may be changed */
double s  = 10;
double b  = 2.6666;
double r  = 28;

int t; //Holds time
int tracer = 0; //Should I trace boolean

int q = 50000; //Effects how often the tracer redisplays

typedef struct {
  double x;
  double y;
  double z;
} points_t;

/*
* Function to output raster text
* Should use VARARGS for more flexibility
*/
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void display()
{
    //Clear image
    glClear(GL_COLOR_BUFFER_BIT);
    //Reset previous tranformations
    glLoadIdentity();
    //Set view angle
    glRotated(ph,1,0,0);
    glRotated(th,0,1,0);

    glOrtho(-1, 1, -1, 1, -1, 1);


    glBegin(GL_LINES);

    /* Lrenz x, y, z variables */
    double lx = 1;
    double ly = 1;
    double lz = 1;

    double dt = 0.001; //Time increment
    int scale = 30; //scaling factor

    glColor3f(0.0,0.0,1.0); //Sets color to blue
    glVertex4d(lx,ly,lz,scale);

    points_t lorenz[q];

    for(int i=0;i<50000;i++)
    {
      /* Calculate the next point in Lorenz attractor, taken from lorenz.c on Moodle */
      points_t p = {lx, ly, lz};
      lorenz[i] = p;


      //switch color every 5000 steps
      if(i<5000) {
        glColor3f(0.836, 0.078, 0.235); //Crimson
      } else if (i < 10000) {
        glColor3f(1.0, 0.271, 0.0); //OrangeRed
      } else if (i < 15000) {
        glColor3f(1.0, 0.843, 0.0); //Gold
      } else if (i < 20000) {
        glColor3f(0.855, 0.439, 0.839); //Orchid
      } else if (i < 25000) {
        glColor3f(0.294, 0.0, 0.510); //Indigo
      } else if (i < 30000) {
        glColor3f(0.196, 0.804, 0.196); //LineGreen
      } else if (i < 35000) {
        glColor3f(0.0, 0.392, 0.0); //DarkGreen
      } else if (i < 40000) {
        glColor3f(0.0, 0.502, 0.502); //Teal
      } else if (i < 45000) {
        glColor3f(0.118, 0.565, 1.0); //DodgerBlue
      }

      //Taken from lorenz.c
      double dx = s*(ly-lx);
      double dy = lx*(r-lz)-ly;
      double dz = lx*ly - b*lz;
      lx += dt*dx;
      ly += dt*dy;
      lz += dt*dz;

      glVertex4d(lx, ly, lz, scale);
      glVertex4d(lx, ly, lz, scale);
    }
    glEnd();

    if(tracer%2 == 1)
    {
      glColor3f(1.0, 1.0, 1.0); //Set color to white
      glPushMatrix();
      glTranslatef(lorenz[t].x/scale, lorenz[t].y/scale, lorenz[t].z/scale);
      glutSolidSphere(.033, 50, 50);
      glPopMatrix();
    }

    //Draw axes in white
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(1,0,0);
    glVertex3d(0,0,0);
    glVertex3d(0,1,0);
    glVertex3d(0,0,0);
    glVertex3d(0,0,1);
    glEnd();

    //Label axes
    glRasterPos3d(1,0,0);
    Print("X");
    glRasterPos3d(0,1,0);
    Print("Y");
    glRasterPos3d(0,0,1);
    Print("Z");
    //Display Parameters
    glWindowPos2i(5,5);
    Print("Viewing Angle = %d,%d", th, ph);
    glWindowPos2i(10, 22);
    Print("s=%.02f, b=%.04f, r=%.02f",s,b,r);

    //Flush and swap buffers
    glFlush();
    glutSwapBuffers();
}


void key(unsigned char ch, int x, int y)
{
  //Exit on ESC
  if(ch == 27) {
    exit(0);
  } else if(ch == 's') { //Decrement s
    s -= .25;
  } else if(ch == 'S') { //Increment s
    s += .25;
  } else if(ch == 'b') { //Decrement b
    b -= .05;
  } else if(ch == 'B') { //Increment b
    b += .05;
  } else if(ch == 'r') { //Decrement r
    r -= .5;
  } else if(ch == 'R') { //Increment r
    r += .5;
  } else if(ch == '1') { //Reset to default Lorenz params
    s = 10;
    b = 2.6666;
    r = 28;
  } else if(ch == '2') { //Reset view angle to x,y
    th = ph = 0;
  } else if(ch == '3') { //Reset view angle to x,z
    th = 0;
    ph = -90;
  } else if(ch == '4') { //Reset view angle to y,z
    th = 90;
    ph = 0;
  } else if(ch == 't') { //Toggle tracer
    tracer++;
  }

  //Tell GLUT to redisplay scene
  glutPostRedisplay();
}

/* GLUT calls this when arrow keys are pressed */
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_RIGHT) { //Right arrow - increase asimuth by 5 degrees
      th += 5;
    } else if(key == GLUT_KEY_LEFT) { //Left arrow - decrease asimuth by 5 degrees
      th -= 5;
    } else if(key == GLUT_KEY_UP) { //Up arrow - increase electaion by 5 degrees
      ph += 5;
    } else if(key == GLUT_KEY_DOWN) { //Down arrow - decrease electaion by 5 degrees
      ph -= 5;
    }

    //Angles must be +/- 360 degrees
    th %= 360;
    ph %= 360;

    //Tell GLUT to redisplay the scene
    glutPostRedisplay();
}

/* GLUT calls this when window is resized */
void reshape(int width, int height)
{
  //Get ratio of width:height of window
  double w2h = (height>0) ? (double)width/height : 1;
  //Set viewport to entire window
  glViewport(0,0,width,height);
  //Tell OpenGL we want to manipulate projection matrix
  glMatrixMode(GL_PROJECTION);
  //Undo percious transformations
  glLoadIdentity();
  //Orthogonal projection box adjusted for aspect ratio of window
  glOrtho(-dim*w2h, +dim*w2h, -dim, +dim, -dim, +dim);
  //Switch to maipulatin the model matrix
  glMatrixMode(GL_MODELVIEW);
  //Undo pervious transformations
  glLoadIdentity();
}

/*GLUT calls thsi when there isn't anything to do */
void idle()
{
  t = glutGet(GLUT_ELAPSED_TIME) % q;
  glutPostRedisplay();
}


int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(500, 500);
  glutCreateWindow("HW1: Evan James");
  //  Tell GLUT to call "display" when the scene should be drawn
  glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
  glutReshapeFunc(reshape);
  //  Tell GLUT to call "special" when an arrow key is pressed
  glutSpecialFunc(special);
  //  Tell GLUT to call "key" when a key is pressed
  glutKeyboardFunc(key);
  //  Te;; GLUT to calle "idle" when nothing is happening
  glutIdleFunc(idle);
  //  Pass control to GLUT so it can interact with the user
  glutMainLoop();
  //  Return code
  return 0;
}
