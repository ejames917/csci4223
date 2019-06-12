/* Evan James ---- CSCI 4229
* HW3 - Lighting and textures
*
* Functions adapted from ex9
*
*
*  Key bindings:
*  m          Toggle between persepctive and first person
*  z          Change displays
*  x          Toggle axes
*  +/-        Changes field of view for perspective
*  arrows     Change view angle in Orthographic and Perspecive mode
*  WASD       Move around in FPS mode
*  PgDn/PgUp  Zoom in and out in perspective, move eye position up and down in FPS mode
*  Scroll     Change size of world10
*  0          Reset view angle
*  ESC        Exit
*/

#include "CSCIx229.h"


/* PROJECTION VARIABLES */
int mode=1;       //  Projection  0==First-person, 1==Perspective
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world

/* FIRST PERSON VARIABLES */
//first person location
double xOffset = 0;
double yOffset = 2.0;
double zOffset = -60.0;
double AX = 0; // x-coordinate of where the camera is looking
double AY = 0; // y-coordinate of where the camera is looking
double AZ = 0; // z-coordinate of where the camera is looking

double UX = 0; // x-coordinate of the up vector
double UY = 1; // y-coordinate of the up vector
double UZ = 0; // z-coordinate of the up vector

/* OTHER GLOBALS */
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int depth=0;      //  Depth test
int axes=0;       //  Display axes
int ntex=0;       //  Cube faces
int disp=4;       //  Display mode
int light=1;      //  Lighting
int color=0;      //  Colored faces
int alpha=100;    //  Transparency
int debug=1;      //  Debugging
int left_click_down = 0;
int window_width = 600;
int window_height = 600;
int saved_th = 0;
int saved_ph = 0;
double idle[6] = {0.0,0.0,0.0,0.0,0.0,0.0};


/* LIGHT VALUES */
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[7]; // Texture names


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
*  Draw vertex in polar coordinates with normal
*/
static void Vertex(double th,double ph)
{
  double x = Sin(th)*Cos(ph);
  double y = Cos(th)*Cos(ph);
  double z =         Sin(ph);
  //  For a sphere at the origin, the position
  //  and normal vectors are the same
  glNormal3d(x,y,z);
  glVertex3d(x,y,z);
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
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor4f(1,1,1,0.01*alpha);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   //  Enable blending
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE);
   //  Front
   if (color) glColor4f(1,0,0,0.01*alpha);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[1]);
   glBegin(GL_QUADS);
   glNormal3f( 0.0, 0.0, 1.0);
   glTexCoord2f(0,0); glVertex3f(-1.0,-1.0, 1.0);
   glTexCoord2f(1,0); glVertex3f(+1.0,-1.0, 1.0);
   glTexCoord2f(1,1); glVertex3f(+1.0,+1.0, 1.0);
   glTexCoord2f(0,1); glVertex3f(-1.0,+1.0, 1.0);
   glEnd();
   //  Back
   if (color) glColor4f(0,0,1,0.01*alpha);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0.0, 0.0,-1.0);
   glTexCoord2f(0,0); glVertex3f(+1.0,-1.0,-1.0);
   glTexCoord2f(1,0); glVertex3f(-1.0,-1.0,-1.0);
   glTexCoord2f(1,1); glVertex3f(-1.0,+1.0,-1.0);
   glTexCoord2f(0,1); glVertex3f(+1.0,+1.0,-1.0);
   glEnd();
   //  Right
   if (color) glColor4f(1,1,0,0.01*alpha);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[3]);
   glBegin(GL_QUADS);
   glNormal3f(+1.0, 0.0, 0.0);
   glTexCoord2f(0,0); glVertex3f(+1.0,-1.0,+1.0);
   glTexCoord2f(1,0); glVertex3f(+1.0,-1.0,-1.0);
   glTexCoord2f(1,1); glVertex3f(+1.0,+1.0,-1.0);
   glTexCoord2f(0,1); glVertex3f(+1.0,+1.0,+1.0);
   glEnd();
   //  Left
   if (color) glColor4f(0,1,0,0.01*alpha);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[4]);
   glBegin(GL_QUADS);
   glNormal3f(-1.0, 0.0, 0.0);
   glTexCoord2f(0,0); glVertex3f(-1.0,-1.0,-1.0);
   glTexCoord2f(1,0); glVertex3f(-1.0,-1.0,+1.0);
   glTexCoord2f(1,1); glVertex3f(-1.0,+1.0,+1.0);
   glTexCoord2f(0,1); glVertex3f(-1.0,+1.0,-1.0);
   glEnd();
   //  Top
   if (color) glColor4f(0,1,1,0.01*alpha);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glNormal3f( 0.0,+1.0, 0.0);
   glTexCoord2f(0,0); glVertex3f(-1.0,+1.0,+1.0);
   glTexCoord2f(1,0); glVertex3f(+1.0,+1.0,+1.0);
   glTexCoord2f(1,1); glVertex3f(+1.0,+1.0,-1.0);
   glTexCoord2f(0,1); glVertex3f(-1.0,+1.0,-1.0);
   glEnd();
   //  Bottom
   if (color) glColor4f(1,0,1,0.01*alpha);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[6]);
   glBegin(GL_QUADS);
   glNormal3f( 0.0,-1.0, 0.0);
   glTexCoord2f(0,0); glVertex3f(-1.0,-1.0,-1.0);
   glTexCoord2f(1,0); glVertex3f(+1.0,-1.0,-1.0);
   glTexCoord2f(1,1); glVertex3f(+1.0,-1.0,+1.0);
   glTexCoord2f(0,1); glVertex3f(-1.0,-1.0,+1.0);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
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
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    //Save transformation
    glPushMatrix();
    //Offset
    glTranslated(x,y,z);
    glRotated(th,0,0,1);

    //Make top and bottom grey
    glColor3f(0.5, 0.0, 1.0);
    //Draw bottom of cylinder at origin
    glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0,-1,0);
    glVertex3f(0.0, 0.0, 0.0);
    for(i=0; i <= 360; i+=step)
    {
      glVertex3f(0, r*Cos(i), r*Sin(i));
      glVertex3f(0.0,0.0,0.0);
    }
    glEnd();

    //Draw top of cylinder at (h,0,0)
    glColor3f(0.5, 0.5, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0,1,0);
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
      glNormal3f(Cos(i),0,Sin(i));
      glVertex3f(0, r*Cos(i), r*Sin(i));
      glVertex3f(h, r*Cos(i), r*Sin(i));
    }
    glEnd();

    glPopMatrix();
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,
                 double r, float color[])
{
    int th,ph;
    float Emission[]  = {color[0],color[1],color[2],1.0};
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    //  White ball
    glColor3f(color[0],color[1],color[2]);
    float shinyvec[1] = {4};
    glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT,GL_SPECULAR,color);
    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
    //  Bands of latitude
    for (ph=-90;ph<90;ph+=10)
    {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*10)
      {
        Vertex(th,ph);
        Vertex(th,ph+10);
      }
      glEnd();
    }
    //  Undo transofrmations
    glPopMatrix();
}

/*
 * Draw an arcade cabinet
 * At (x,y,z)
 * Dimensions (dx, dy, dz)
 * Rotated (th) about the y axis
 */
static void cabinet(double x, double y, double z,
                    double dx, double dy, double dz, double th)
{
    float xMax;
    // Save transformation
    glPushMatrix();
    //Offset, scale, and rotate
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    //Left outer side
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0,0,1);
    for(int y=0; y<=1; y+=.05)
    {
      if(y>=.5 && y<=.8)
        xMax=.3;
      else
        xMax=.4;
      glVertex3f(0,y,0);
      glVertex3f(xMax,y,0);
    }
    glEnd();

    glPopMatrix();
}

/*
 * Draw the room where everything will be contained
 */
static void arcade()
{
    //Save matrix
    glPushMatrix();
    //  Enable textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
    //  Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    //Floor
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glNormal3f(0.0,1.0,0.0);
    glTexCoord2f(0,0); glVertex3f(-10.0,0.0,-10.0);
    glTexCoord2f(3,0); glVertex3f(-10.0,0.0,10.0);
    glTexCoord2f(3,3); glVertex3f(10.0,0.0,10.0);
    glTexCoord2f(0,3); glVertex3f(10.0,0.0,-10.0);
    glEnd();
    //Walls!
    //Left
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(-10,0,-10);
    glTexCoord2f(5,0); glVertex3f(-10,10,-10);
    glTexCoord2f(5,5); glVertex3f(-10,10,10);
    glTexCoord2f(0,5); glVertex3f(-10,0,10);
    glEnd();
    //Right
    glBegin(GL_QUADS);
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(10,0,-10);
    glTexCoord2f(5,0); glVertex3f(10,10,-10);
    glTexCoord2f(5,5); glVertex3f(10,10,10);
    glTexCoord2f(0,5); glVertex3f(10,0,10);
    glEnd();
    //Back
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-10,0,10);
    glTexCoord2f(5,0); glVertex3f(-10,10,10);
    glTexCoord2f(5,5); glVertex3f(10,10,10);
    glTexCoord2f(0,5); glVertex3f(10,0,10);
    glEnd();
    //Front
    glBegin(GL_QUADS);
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(-10,0,-10);
    glTexCoord2f(5,0); glVertex3f(-10,10,-10);
    glTexCoord2f(5,5); glVertex3f(10,10,-10);
    glTexCoord2f(0,5); glVertex3f(10,0,-10);
    glEnd();
    //Ceiling
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(-10,10,-10);
    glTexCoord2f(5,0); glVertex3f(-10,10,10);
    glTexCoord2f(5,5); glVertex3f(10,10,10);
    glTexCoord2f(0,5); glVertex3f(10,10,-10);
    glEnd();
    //Light hanging from Ceiling
    cylinder(0,8,0, .1,3, 90); //Pole
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_TRIANGLE_FAN); //Cone of light
    glTexCoord2f(0,0); glVertex3f(0,9,0);
    for(int i=0; i<=360; i+=5)
    {
      glTexCoord2f(0,0);
      glVertex3f(Cos(i),7,Sin(i));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //cicle at bottom of cone
    glVertex3f(0,7,0);
    for(int i=0; i<=360; i+=5)
    {
      glVertex3f(Cos(i),7,Sin(i));
    }
    glEnd();

    //  Undo transformations and textures
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
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
  glutSwapBuffers();
}

/*
 * Function to make sure the mouse controls don't go too far
 */
void checkOffsets()
{
    if(xOffset > 29.5)
        xOffset = 29.5;
    else if(xOffset < -105.5)
        xOffset = -105.5;
    if(zOffset > 120)
        zOffset = 120;
    else if(zOffset < -60)
        zOffset = -60;
    if(yOffset > 28)
        yOffset = 28;
    else if(yOffset < 2)
        yOffset = 2;
}


/*
 * Called to assist in mouse maniuplation of view angle
 */
double mouse_rotation(double delta, double mid)
{
  return 180 * (delta / mid);
}

/*
  *  GLUT calls this routine when an arrow key is pressed
  */
  void special(int key,int x,int y)
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
    //  Update projection
    Project(fov,asp,dim);
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
    //  Reset view angle
    else if (ch == '0')
    {
      th = ph = 0;
      xOffset = 0;
      yOffset = 2.0;
      zOffset = -60.0;
    }
    //  Toggle axes
    else if (ch == 'x' || ch == 'X')
    axes = 1-axes;
    //  Toggle lighting
    else if (ch == 'l' || ch == 'L')
    light = 1-light;
    //  Switch projection mode
    else if (ch == 'p' || ch == 'P')
    {
      mode = (mode+1)%3;
      th = ph = 0;
      xOffset = 0;
      yOffset = 0;
      zOffset = 2*dim;
    }
    // Switch what shapes to display
    else if (ch == 'z' || ch == 'Z')
    {
       disp++;
       disp %= 6;
    }
    //  Change field of view angle
    else if (ch == '-' && ch>1)
    fov--;
    else if (ch == '+' && ch<179)
    fov++;
    else if (ch == 'r')
    {
      xOffset = -31.0;
      yOffset = 4.0;
      zOffset = 0.0;
      th = 0;
      ph  = 180;
    }
    //FIRST PERSON NAVIGATION WITH WASD
    else if(ch == 'w' || ch == 'W')
    {
        xOffset -= 2*Sin(th);
        zOffset += 2*Cos(th);
        checkOffsets();
    }
    else if(ch == 's' || ch == 'S')
    {
        xOffset += 2*Sin(th);
        zOffset -= 2*Cos(th);
        checkOffsets();
    }
    else if(ch == 'a' || ch == 'A')
    {
        xOffset -= 2*Sin(th-90);
        zOffset += 2*Cos(th-90);
        checkOffsets();
    }
    else if(ch == 'd' || ch == 'D')
    {
        xOffset += 2*Sin(th-90);
        zOffset -= 2*Cos(th-90);
        checkOffsets();
    }
    else if(ch == '[')
    {
        yOffset += 1;
        checkOffsets();
    }
    else if(ch == ']')
    {
        yOffset -= 1;
        checkOffsets();
    }

    //  Reproject
    Project(fov,asp,dim);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
  }

/*
 * Handle mouse input
 */
void mouse(int button, int state, int x, int y)
  {
    // Wheel reports as button 3(scroll up) and button 4(scroll down)
    if ((button == 3) || (button == 4)) // It's a wheel event
    {
      if (button == 3)
      dim += 0.1;
      else if(dim>1)
      dim -= 0.1;
    }
    if ((button == 0) && (state == GLUT_DOWN || state == GLUT_UP))
    {
      if (state == GLUT_DOWN)
      left_click_down = 1;
      else if (state == GLUT_UP)
      left_click_down = 0;
      double midx = (window_width / 2);
      double midy = (window_height / 2);
      double deltax = (midx - x);
      double deltay = (midy - y);
      saved_th = th - mouse_rotation(deltax , midx);
      saved_ph = ph - mouse_rotation(deltay , midy);
    }
    //  Update projection
    Project(fov,asp,dim);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
  }

  void motionmouse(int x, int y)
  {
    if (left_click_down)
    {
      double midx = (window_width / 2);
      double midy = (window_height / 2);
      double deltax = (midx - x);
      double deltay = (midy - y);
      th = mouse_rotation(deltax , midx) + saved_th;
      ph = mouse_rotation(deltay , midy) + saved_ph;
      th %= 360;
      ph %= 360;
    }
    //  Update projection
    Project(fov,asp,dim);
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
   Project(fov,asp,dim);
 }


 void idlefunc()
 {
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    idle[0] = fmod(90*t,360);
    idle[1] = fmod(45*t,360);
    idle[2] = fmod(30*t,360);
    idle[3] = fmod(22.5*t,360);
    idle[4] = fmod(18*t,360);
    idle[5] = fmod(15*t,360);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
 }

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const char* modeDisp = "";
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGl
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   // Set view
   if(mode == 0)
    {
        //Set up perspective projection
        double Ex = 2*dim*Sin(th)*Cos(ph);
        double Ey = 2*dim*Sin(ph);
        double Ez = -2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez ,0,0,0, 0,Cos(ph),0);
    }
    else
    {
        //Set up first person projection
        double Cx = -2*dim*Sin(th)*Cos(ph) + xOffset;
        double Cy = 2*dim*Sin(ph) + yOffset;
        double Cz = 2*dim*Cos(th)*Cos(ph) + zOffset;
        gluLookAt(xOffset,yOffset,zOffset ,Cx,Cy,Cz, 0,Cos(ph),0);
    }
   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   float white[]     = {1,1,1,1};

    //  Light direction
    float Position[5][4]  = {
      {68*Cos(idle[0])-38,30,20 , 1},
      {68*Cos(idle[1])-38,31,40 , 1},
      {68*Cos(idle[2])-38,32,60 , 1},
      {68*Cos(idle[3])-38,33,80 , 1},
      {68*Cos(idle[4])-38,34,100 , 1},
    };
    //  Draw light position as ball (still no lighting here)
    glColor3f(1,1,1);
    ball(Position[0][0],Position[0][1],Position[0][2],1,white);
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  glColor sets ambient and diffuse color materials
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //  Enable light 0
    glEnable(GL_LIGHT0);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT0,GL_POSITION,Position[0]);
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,32.0f);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);


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
        cylinder(0.0,-1.0,0.0, 1.5,3.0, 90);
        break;
     case 3:
        cylField();
        break;
     case 4:
        arcade();
        break;
   }

   //  Draw axes
   glDisable(GL_LIGHTING);
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
   if(debug)
   {
   //  Display parameters
   glWindowPos2i(5,5);
   if(mode==0)
      Print("Projection:%s Camera Position:(%.2f, %.2f, %.2f) Direction Vector:(%.2f, 1.0, %.2f)", modeDisp, xOffset, yOffset, zOffset, AX, AZ);
   else
      Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,modeDisp);
   glWindowPos2i(5,45);
   Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
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
   glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA);
   glutInitWindowSize(600,600);
   glutCreateWindow("HW3: Evan James");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutMouseFunc(mouse);
   glutMotionFunc(motionmouse);
   glutKeyboardFunc(key);
   glutIdleFunc(idlefunc);
   //Load textures
   texture[0] = LoadTexBMP("textures/tile.bmp");
   texture[1] = LoadTexBMP("textures/wall.bmp");
   texture[2] = LoadTexBMP("textures/ceiling.bmp");
   texture[3] = LoadTexBMP("textures/lamp.bmp");

   glutMainLoop();
   return 0;
}
