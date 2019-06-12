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
#include "arcade.h"


/* PROJECTION VARIABLES */
int mode=1;       //  Projection  0==First-person, 1==Perspective
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=50.0;   //  Size of world

/* FIRST PERSON VARIABLES */
//first person location
double xOffset = 0;
double yOffset = 18.0;
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
int disp=1;       //  Display mode
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
unsigned int texture[7];

/* TEXTURE STUFF */
unsigned int wall_texture = 0; //wall texture
unsigned int mural_texture = 0; //mural for back wall
unsigned int floor_texture = 0;
unsigned int ceiling_tex = 0;
unsigned int frame_tex = 0;
unsigned int poster_tex[5];
unsigned int skeeball_sign = 0;
unsigned int skeeball_wood = 0;
unsigned int games_tex[5];
unsigned int cabinet_body_tex = 0;
unsigned int cabinet_side_tex = 0;
unsigned int skeeball_body_tex = 0;


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
    //F1 key - toggle debug mode
    else if (key == GLUT_KEY_F1)
    debug = 1-debug;
    //F2 when in debug mode changes between arcade and 3x3 cubes
    else if(debug == 1 && key == GLUT_KEY_F2)
    disp = 1-disp;
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
      yOffset = 8.0;
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
      mode = 1-mode;
      th = ph = 0;
      xOffset = 0;
      yOffset = 0;
      zOffset = 2*dim;
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

/*
 * For handling the movement of the mouse when click and dragging
 */
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
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGl
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
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

   //  Light switch
   if (light)
   {
     //  Translate intensity to color vectors
     float Ambient[]   = {0.1,0.1,0.1,1.0};
     float Diffuse[]   = {0.3,0.3,0.3,1};
     float Specular[]  = {0.2,0.2,0.2,1};
     float white[]     = {1,1,1,1};

     float Position[5][4]  = {
       {68*Cos(idle[0])-38,30,20 , 1},
       {68*Cos(idle[1])-38,31,40 , 1},
       {68*Cos(idle[2])-38,32,60 , 1},
       {68*Cos(idle[3])-38,33,80 , 1},
       {68*Cos(idle[4])-38,34,100 , 1},
     };

     ball(Position[1][0],Position[1][1],Position[1][2],1,white, 1);

    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
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
  }

  if(disp)
  {
    //Left wall
    wall(33.5,0,-30,10,18,10,0,180,wall_texture);
    wall(33.5,0,0,10,18,10,0,180,wall_texture);
    wall(33.5,0,30,10,18,10,0,180,wall_texture);
    wall(33.5,0,60,10,18,10,0,180,wall_texture);
    wall(33.5,0,90,10,18,10,0,180,wall_texture);
    wall(33.5,0,120,10,18,10,0,180,wall_texture);
    //Back mural and wall behind it
    mural(mural_texture, light);
    wall(0,0,120.1,10,18,12,0,90,wall_texture);
    wall(-33.5,0,120.1,10,18,12,0,90,wall_texture);
    wall(-67,0,120.1,10,18,12,0,90,wall_texture);
    wall(-100.5,0,120.1,10,18,12,0,90,wall_texture);
    wall(-134,0,120.1,10,18,12,0,90,wall_texture);
    //Right wall
    wall(-110.5,0,-60,10,18,10,0,0,wall_texture);
    wall(-110.5,0,-30,10,18,10,0,0,wall_texture);
    wall(-110.5,0,0,10,18,10,0,0,wall_texture);
    wall(-110.5,0,30,10,18,10,0,0,wall_texture);
    wall(-110.5,0,60,10,18,10,0,0,wall_texture);
    wall(-110.5,0,90,10,18,10,0,0,wall_texture);
    //Floor
    floor_panel(33.5,0,-30, 48,0,30, 0,180, floor_texture);
    floor_panel(33.5,0,0, 48,0,30, 0,180, floor_texture);
    floor_panel(33.5,0,30, 48,0,30, 0,180, floor_texture);
    floor_panel(33.5,0,60, 48,0,30, 0,180, floor_texture);
    floor_panel(33.5,0,90, 48,0,30, 0,180, floor_texture);
    floor_panel(33.5,0,120, 48,0,30, 0,180, floor_texture);
    //ceiling
    double Z=120,Y=54;
    for(int i=0; i<=6; i++)
    {
      ceiling(33.5,Y,Z, 33.5,10,27, 0,180, ceiling_tex);
      ceiling(0,Y,Z, 33.5,10,27, 0,180, ceiling_tex);
      ceiling(-33.5,Y,Z, 33.5,10,27, 0,180, ceiling_tex);
      ceiling(-67,Y,Z, 33.5,10,27, 0,180, ceiling_tex);
      ceiling(-100.5,Y,Z, 35,10,27, 0,180, ceiling_tex);
      Z-=25;
    }


    skeeball(-5,0,-20, 24,15,15, 0,0, skeeball_wood,skeeball_sign,skeeball_body_tex);
    skeeball(-5,0,0, 24,15,15, 0,0, skeeball_wood,skeeball_sign,skeeball_body_tex);
    skeeball(-5,0,20, 24,15,15, 0,0, skeeball_wood,skeeball_sign,skeeball_body_tex);
    skeeball(-5,0,40, 24,15,15, 0,0, skeeball_wood,skeeball_sign,skeeball_body_tex);
    skeeball(-5,0,60, 24,15,15, 0,0, skeeball_wood,skeeball_sign,skeeball_body_tex);

    picture(-110.4,5,100, 15,30,15,90, frame_tex, poster_tex[1]);
    picture(-110.4,10,63, 20,20,20,90, frame_tex, poster_tex[0]);
    picture(-110.4,5,20, 15,30,20,90, frame_tex, poster_tex[2]);
    picture(-110.4,5,-20, 15,30,20,90, frame_tex, poster_tex[3]);

    cabinet(-95,0,80, 13,30,15, 0,180, games_tex[0],cabinet_body_tex,cabinet_side_tex);
    cabinet(-95,0,40, 13,30,15, 0,180, games_tex[1],cabinet_body_tex,cabinet_side_tex);
    cabinet(-95,0,0, 13,30,15, 0,180, games_tex[2],cabinet_body_tex,cabinet_side_tex);
    cabinet(-95,0,-40, 13,30,15, 0,180, games_tex[3],cabinet_body_tex,cabinet_side_tex);
  }
  else
  {
    cubes();
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
   if(mode == 0)
   {
       Print("Projection Type: Perspective");
       glWindowPos2i(5,25);
       Print("Dimension: %.0f", dim);
   }
   else
   {
       Print("Projection Type: First Person");
       glWindowPos2i(5,25);
       Print("User Location: { %.3f, %.3f, %.3f }", xOffset, yOffset, zOffset);
   }
   glWindowPos2i(5,45);
   if(light)
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
   glutCreateWindow("Final: Evan James");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutMouseFunc(mouse);
   glutMotionFunc(motionmouse);
   glutKeyboardFunc(key);
   glutIdleFunc(idlefunc);
   //Load textures
   wall_texture = LoadTexBMP("textures/wall.bmp");
   mural_texture = LoadTexBMP("textures/mural.bmp");
   floor_texture = LoadTexBMP("textures/floor.bmp");
   frame_tex = LoadTexBMP("textures/frame.bmp");
   ceiling_tex = LoadTexBMP("textures/ceiling.bmp");
   poster_tex[0] = LoadTexBMP("textures/dk.bmp");
   poster_tex[1] = LoadTexBMP("textures/invaders.bmp");
   poster_tex[2] = LoadTexBMP("textures/wwe.bmp");
   poster_tex[3] = LoadTexBMP("textures/mk.bmp");
   skeeball_wood = LoadTexBMP("textures/skeewood.bmp");
   skeeball_sign = LoadTexBMP("textures/skeeball.bmp");
   games_tex[0] = LoadTexBMP("textures/galaga.bmp");
   games_tex[1] = LoadTexBMP("textures/joust.bmp");
   games_tex[2] = LoadTexBMP("textures/pacman.bmp");
   games_tex[3] = LoadTexBMP("textures/moonPatrol.bmp");
   cabinet_side_tex = LoadTexBMP("textures/cabinetSide.bmp");
   cabinet_body_tex = LoadTexBMP("textures/cabinetBody.bmp");
   skeeball_body_tex = LoadTexBMP("textures/skeeballBody.bmp");

   glutMainLoop();
   return 0;
}
