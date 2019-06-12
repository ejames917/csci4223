/* Evan James ---- CSCI 4229
* HW3 - Lighting and textures
*
* Functions adapted from ex9
*
*
*  Key bindings:
*  m          Toggle between persepctive and first person
*  d          Change displays
*  x          Toggle axes
*  +/-        Changes field of view for perspective
*  arrows     Change view angle in Orthographic and Perspecive mode
*  WASD       Move around in FPS mode
*  PgDn/PgUp  Zoom in and out in perspective, move eye position up and down in FPS mode
*  0          Reset view angle
*  ESC        Exit
*/

#include "CSCIx229.h"

//Macro for boolean values
typedef enum { false, true } bool;


/* PROJECTION VARIABLES */
int mode=1;       //  Projection  0==First-person, 1==Perspective
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world

/* FIRST PERSON VARIABLES */
//Eye coordinates
double Ex=0.0, Ey=2.0, Ez=-30.0;
//Camera look coordinates
double Cx=0.0, Cy=0.0, Cz=0.0;
//Rotation variable for FPS
float angle = 0.0;
//Change in angle for mouse input
float deltaAngle = 0.0;
float deltaMove = 0.0;
int xOrigin = -1;

/* OTHER GLOBALS */
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int depth=0;      //  Depth test
int axes=0;       //  Display axes
int ntex=0;       //  Cube faces
int disp=0;       //  Display mode
int light=1;      //  Lighting
int color=0;      //  Colored faces
int alpha=100;    //  Transparency
int debug=1;      //  Debugging

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
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void ball(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
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
    glTexCoord2f(0,0); glVertex3f(-15.0,0.0,-15.0);
    glTexCoord2f(3,0); glVertex3f(-15.0,0.0,15.0);
    glTexCoord2f(3,3); glVertex3f(15.0,0.0,15.0);
    glTexCoord2f(0,3); glVertex3f(15.0,0.0,-15.0);
    glEnd();
    //Walls!
    //Left
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(1,0,0);
    glTexCoord2f(0,0); glVertex3f(-15,0,-15);
    glTexCoord2f(5,0); glVertex3f(-15,15,-15);
    glTexCoord2f(5,5); glVertex3f(-15,15,15);
    glTexCoord2f(0,5); glVertex3f(-15,0,15);
    glEnd();
    //Right
    glBegin(GL_QUADS);
    glNormal3f(-1,0,0);
    glTexCoord2f(0,0); glVertex3f(15,0,-15);
    glTexCoord2f(5,0); glVertex3f(15,15,-15);
    glTexCoord2f(5,5); glVertex3f(15,15,15);
    glTexCoord2f(0,5); glVertex3f(15,0,15);
    glEnd();
    //Back
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(-15,0,15);
    glTexCoord2f(5,0); glVertex3f(-15,15,15);
    glTexCoord2f(5,5); glVertex3f(15,15,15);
    glTexCoord2f(0,5); glVertex3f(15,0,15);
    glEnd();
    //Front
    glBegin(GL_QUADS);
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0); glVertex3f(-15,0,-15);
    glTexCoord2f(5,0); glVertex3f(-15,15,-15);
    glTexCoord2f(5,5); glVertex3f(15,15,-15);
    glTexCoord2f(0,5); glVertex3f(15,0,-15);
    glEnd();
    //Ceiling
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);
    glNormal3f(0,-1,0);
    glTexCoord2f(0,0); glVertex3f(-15,15,-15);
    glTexCoord2f(5,0); glVertex3f(-15,15,15);
    glTexCoord2f(5,5); glVertex3f(15,15,15);
    glTexCoord2f(0,5); glVertex3f(15,15,-15);
    glEnd();
    //Light hanging from Ceiling
    cylinder(0,13,0, .1,3, 90); //Pole
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_TRIANGLE_FAN); //Cone of light
    glTexCoord2f(.5,.5); glVertex3f(0,14,0);
    for(int i=0; i<=360; i+=5)
    {
      glTexCoord2f(0,0);
      glVertex3f(Cos(i),12,Sin(i));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //cicle at bottom of cone
    glVertex3f(0,12,0);
    for(int i=0; i<=360; i+=5)
    {
      glVertex3f(Cos(i),12,Sin(i));
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
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  if(mode==0) //FPS mode, so keys control position in world
  {
    if(key == GLUT_KEY_UP)
    {
      Ex -= 2*Sin(th);
      Ez += 2*Cos(th);
    }
    else if(key == GLUT_KEY_DOWN)
    {
      Ex += 2*Sin(th);
      Ez -= 2*Cos(th);
    }
    else if(key == GLUT_KEY_LEFT)
    {
      Ex -= 2*Sin(th-90);
      Ez += 2*Cos(th-90);
    }
    else if(key == GLUT_KEY_RIGHT)
    {
      Ex += 2*Sin(th-90);
      Ez -= 2*Cos(th-90);
    }
    else if(key == GLUT_KEY_PAGE_UP)
      Ey++;
    else if(key == GLUT_KEY_PAGE_DOWN)
      Ey--;
  }
  else
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
  }
   //  PageUp key - increase dim
   if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  F12 key - enable displaying debug info
   else if (key==GLUT_KEY_F1)
      debug++;
      debug%=2;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update ion
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
      Ex = Ey = Ez = 0.0;
      Cx = Cz = 0.0;
   }
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
   {
      mode++;
      mode %= 2;
   }
   // Switch what shapes to display
   else if (ch == 'q' || ch == 'Q')
   {
      disp++;
      disp %= 6;
   }
   //  Toggle colored faces
   else if (ch == 'c' || ch == 'C')
      color = 1-color;
   //  Toggle textures mode
   else if (ch == 't')
      ntex = 1-ntex;
   //  Light elevation
   if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Repitition
   else if (ch=='+' && alpha<100)
      alpha += 5;
   else if (ch=='-' && alpha>0)
      alpha -= 5;
   else
   {
     //  Change field of view angle
     if (ch == '-' && ch>1)
        fov--;
     else if (ch == '+' && ch<179)
        fov++;
   }
   //  Reproject
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

 /*
  *  GLUT calls this routine when the window is resized
  */
 void idle()
 {
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(90*t,360.0);
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
   if(!mode)
   {
     double Cx = -2*dim*Sin(th)*Cos(ph) + Ex;
     double Cy = 2*dim*Sin(ph) + Ey;
     double Cz = 2*dim*Cos(th)*Cos(ph) + Ez;
     gluLookAt(Ex,Ey,Ez ,Cx,Cy,Cz, 0,1,0);
     modeDisp = "First-person";
   }
   else
   {
     double Ex = 2*dim*Sin(th)*Cos(ph);
     double Ey = 2*dim*Sin(ph);
     double Ez = -2*dim*Cos(th)*Cos(ph);
     gluLookAt(Ex,Ey,Ez ,0,0,0, 0,Cos(ph),0);
     modeDisp = "Perspective";
   }
   //  Translate intensity to color vectors
   float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
   if(disp==5) //In the arcade
   {
      ylight = 5;
      float Position[] = {0,11,0,1};
      //  OpenGL should normalize normal vectors
      glEnable(GL_NORMALIZE);
      //  Enable lighting
      glEnable(GL_LIGHTING);
      //  glColor sets ambient and diffuse color materials
      glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      glEnable(GL_LIGHT1);
      //  Set ambient, diffuse, specular components and position of light 0
      glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
      glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
      glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);
      glLightfv(GL_LIGHT1,GL_POSITION,Position);
   }

      //  Light direction
      float Position[]  = {5*Cos(zh),ylight,5*Sin(zh),1};
      //  Draw light position as ball (still no lighting here)
      glColor3f(1,1,1);
      ball(Position[0],Position[1],Position[2] , 0.1,0.1,0.1 , 0);
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
      glLightfv(GL_LIGHT0,GL_POSITION,Position);

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
      Print("Projection:%s Camera Position:(%.2f, %.2f, %.2f) Direction Vector:(%.2f, 1.0, %.2f)", modeDisp, Ex, Ey, Ez, Cx, Cz);
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
 * Function to process mouse button state Change
 */
static void mouseButton(int button, int state, int x, int y)
{
  //Only start mouse input if left button is pressed
  if(button == GLUT_LEFT_BUTTON)
  {
    //When button is released
    if(state == GLUT_UP)
    {
      angle += deltaAngle;
      xOrigin = -1;
    }
    else //state == GLUT_DOWN
    {
      xOrigin = x;
    }
  }
}

/*
 * Function to process motion of mouse while button is pressed
 */
static void mouseMove(int x, int y)
{
  //This will only be true if the left mouse button is pressed
  if(xOrigin >= 0)
  {
    //Update deltaAngle
    deltaAngle = (x - xOrigin) * 5;
    //Update camera direction
    Cx = Sin(angle + deltaAngle);
    Cz = -Cos(angle + deltaAngle);
  }
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
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   glutMouseFunc(mouseButton);
   glutMotionFunc(mouseMove);
   //Load textures
   texture[0] = LoadTexBMP("textures/tile.bmp");
   texture[1] = LoadTexBMP("textures/wall.bmp");
   texture[2] = LoadTexBMP("textures/ceiling.bmp");
   texture[3] = LoadTexBMP("textures/lamp.bmp");

   glutMainLoop();
   return 0;
}
