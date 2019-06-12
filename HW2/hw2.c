/* Evan James ---- CSCI 4229
* HW2 - Drawing in 3D
*
* Functions adapted from ex9
*
*
*  Key bindings:
*  m          Toggle between persepctive and orthogonal
*  d          Change displays
*  x          Toggle axes
*  f          Toggle FPS on/ (Takes prescident over orthogonal/perspective)
*  +/-        Changes field of view for perspective
*  arrows     Change view angle in Orthographic and Perspecive mode
*  WASD       Move around in FPS mode
*  PgDn/PgUp  Zoom in and out
*  0          Reset view angle
*  ESC        Exit
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))
#define Tan(th) tan(3.1415927/180*(th))

//Macro for boolean values
typedef enum { false, true } bool;

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int disp=0;       //  Display mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fp=0;         //  Toggle whether FPS is on or off
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world

//Stuff for POV camera
bool firstMouse = true;
int rot = 0.0; //Rotation variable for FPS
//Eye coordinates
double Ex=0.0, Ey=0.0, Ez=0.0;
//Camera look coordinates
double Cx=0.0, Cz=0.0;


/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
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

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if(fp)
   {
     gluPerspective(fov, asp, dim/4, 4*dim);
   }
   else
   {
     if (mode)
        gluPerspective(fov,asp,dim/4,4*dim);
     //  Orthogonal projection
     else
        glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   }
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw a cylinder
 *     bottom face at (x,y,z)
 *     radius, height (r, h)
 *     rotated th about the z axis
 */
static void cylinder(double x,double y,double z,
                 double r, double h,
                 double th)
{
    //Step is the resolution of the cirlce at base of cylinder
    const int step=5;
    int i;
    //Save transformation
    glPushMatrix();
    //Offset
    glTranslated(x,y,z);
    glRotated(th,0,0,1);

    //Make top and bottom grey
    glColor3f(0.8, 0.8, 0.8);
    //Draw bottom of cylinder at origin
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0, 0.0, 0.0);
    for(i=0; i <= 360; i+=step)
    {
      glVertex3f(0, r*Cos(i), r*Sin(i));
    }
    glEnd();

    //Draw top of cylinder at (h,0,0)
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(h, 0.0, 0.0);
    for(i=0; i <= 360; i+=step)
    {
      glVertex3f(h, r*Cos(i), r*Sin(i));
    }
    glEnd();

    //Make walls white
    glColor3f(1.0, 1.0, 1.0);
    //Draw walls of cylinder
    glBegin(GL_QUAD_STRIP);
    for(i=0; i<=360; i+=step)
    {
      glVertex3f(0, r*Cos(i), r*Sin(i));
      glVertex3f(h, r*Cos(i), r*Sin(i));
    }
    glEnd();

    glPopMatrix();
}

/*
 * 3x3 cube o' cubes
 */
static void cubes()
{
  int i,j,k;
  for (i=-1;i<=1;i++)
     for (j=-1;j<=1;j++)
        for (k=-1;k<=1;k++)
           cube(i,j,k , 0.3,0.3,0.3 , 0);
}

/*
 * 3x3 cube o' cylinders
 */
static void cylinders()
{
  int i,j,k;
  for (i=-1;i<=1;i++)
     for (j=-1;j<=1;j++)
        for (k=-1;k<=1;k++)
           cylinder(i,j,k , 0.3,0.5 , 0);
}

static void cylField()
{
  int i;
  float coordRange=2.0;
  for(i=0; i<=8; i++)
  {
    glPushMatrix();
    float randX = (float)rand()/(float)(RAND_MAX/coordRange);
    float randY = (float)rand()/(float)(RAND_MAX/coordRange);
    float randZ = -1 * (float)rand()/(float)(RAND_MAX/coordRange);
    float h = (float)rand()/(float)(RAND_MAX/0.5);
    float r = (float)rand()/(float)(RAND_MAX/0.3);
    float rot = (float)rand()/(float)(RAND_MAX/360.0);
    cylinder(randX,randY,randZ, r,h, rot);
    glPopMatrix();
  }
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  if(!fp)
  {
     //  Right arrow key - increase angle by 5 degrees
     if (key == GLUT_KEY_RIGHT)
        th += 5;
     //  Left arrow key - decrease angle by 5 degrees
     else if (key == GLUT_KEY_LEFT)
        th -= 5;
     //  Up arrow key - increase elevation by 5 degrees
     else if (key == GLUT_KEY_UP)
        ph += 5;
     //  Down arrow key - decrease elevation by 5 degrees
     else if (key == GLUT_KEY_DOWN)
        ph -= 5;
     //  PageUp key - increase dim
     else if (key == GLUT_KEY_PAGE_UP)
        dim += 0.1;
     //  PageDown key - decrease dim
     else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
        dim -= 0.1;
     //  Keep angles to +/-360 degrees
     th %= 360;
     ph %= 360;
   }
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //Toggle FPS
   else if(ch=='f' || ch=='F')
      fp = 1-fp;
   //  Reset view angle
   else if (ch == '0')
   {
      th = ph = 0;
      Ex = Ey = Ez = 0.0;
      Cx = Cz = 0.0;
   }
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   // Switch what shapes to display
   else if (ch == 'q' || ch == 'Q')
   {
      disp++;
      disp %= 4;
   }

   if(fp)
   {
     double dt = 0.05;
     if(ch=='w' || ch=='W')
     {
       Ex += Cx*dt; //Update eye vector
       Ez += Cz*dt;
     }
     else if(ch=='a' || ch=='A')
       rot -= 3;
     else if(ch=='s' || ch=='S')
     {
       Ex -= Cx*dt;
       Ez -= Cz*dt;
     }
     else if(ch=='d' || ch=='D')
       rot += 3;

    //Keep angles to +/- 360 degrees
    rot %= 360;
   }
   else
   {
     //  Change field of view angle
     if (ch == '-' && ch>1)
        fov--;
     else if (ch == '+' && ch<179)
        fov++;
   }
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const char* modeDisp = "";
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   // Set view angle
   if(fp)
   {
     Cx = 2*dim*Sin(rot); //Adjust camera vector
     Cz = -2*dim*Cos(rot);
     gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0.0,1.0,0.0);
     modeDisp = "First-person";
   }
   else
   {
     if(mode) {
       double px = -2*dim*Sin(th)*Cos(ph);
       double py = +2*dim        *Sin(ph);
       double pz = +2*dim*Cos(th)*Cos(ph);
       gluLookAt(px,py,pz , 0,0,0 , 0,Cos(ph),0);
       modeDisp = "Perspective";
     } else {
       glRotatef(ph,1,0,0);
       glRotatef(th,0,1,0);
       modeDisp = "Orthogonal";
     }
   }

   //What to draw?
   switch(disp)
   {
     //Cubes
     case 0:
        cubes();
        break;
     //Cylindar
     case 1:
        cylinders();
        break;
     case 2:
        cylinder(0.0,0.0,0.0, 1.5,3.0, 90);
        break;
     case 3:
        glPushMatrix();
        cylField();
        glPopMatrix();
        break;
   }

   //  Draw axes
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   if(fp)
      Print("Projection:%s Camera Position:(%.2f, %.2f, %.2f) Direction Vector:(%.2f, 1.0, %.2f)", modeDisp, Ex, Ey, Ez, Cx, Cz);
   else
      Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,modeDisp);
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("HW2: Evan James");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //glutIdleFunc(idle);
   // Allow mouse movement when in POV mode
   //glutPassiveMotionFunc(mouseMovement);
   // Put pointer to middle of window upon entering
  //  glutEntryFunc(fixCursor);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
