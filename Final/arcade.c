#include "arcade.h"

float red[3] = {1,0,0};
float green[3] = {0,1,0};
float blue[3] = {0,0,1};
float yellow[3] = {1,1,0};
float white[3] = {1,1,1};
/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
void ball(double x,double y,double z,
                 double r, float color[], int light)
{
    int th,ph,inc=10;
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(r,r,r);
    // Ball color
    glColor3f(color[0],color[1],color[2]);
    if(light)
    {
      float shinyvec[1] = {4};
      float Emission[]  = {color[0],color[1],color[2],1.0};
      glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
      glMaterialfv(GL_FRONT,GL_SPECULAR,color);
      glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
    }
    //  Bands of latitude
    for (ph=-90;ph<90;ph+=inc)
    {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
        Vertex(th,ph);
        Vertex(th,ph+inc);
      }
      glEnd();
    }
    //  Undo transofrmations
    glPopMatrix();
}


/*
*  Draw vertex in polar coordinates with normal
*/
void Vertex(double th,double ph)
{
  double x = Sin(th)*Cos(ph);
  double y = Cos(th)*Cos(ph);
  double z =         Sin(ph);
  //  For a sphere at the origin, the position
  //  and normal vectors are the same
  glNormal3d(x,y,z);
  glVertex3d(x,y,z);
}

void ceiling(double x, double y, double z, double xScale, double yScale, double zScale, double rotateX, double rotateY, unsigned int ceiling_texture)
{
  glPushMatrix();
  glTranslated(x, y, z);
  glRotated(rotateY, 0,1,0);
  glRotated(rotateX, 1,0,0);
  glScaled(xScale, yScale, zScale);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,ceiling_texture);
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glTexCoord2f(1,0); glVertex3f(1,0,0);
  glTexCoord2f(1,1); glVertex3f(1,0,1);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(0,1); glVertex3f(0,0,1);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void wall(double x, double y, double z,
  double xScale, double yScale, double zScale,
  double rotateX, double rotateY, unsigned int wall_texture )
  {
    glPushMatrix();
    glTranslated(x, y,z);
    glRotated(rotateY, 0,1,0);
    glRotated(rotateX, 1,0,0);
    glScaled(xScale, yScale, zScale);
    double bottom = 0.0;
    double top = 3.0;
    double length = 3.0;
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,wall_texture);
    glBegin(GL_QUADS);
    glColor3f(0.8,0.4,0.4);
    glNormal3f( 1, 0, 0);
    glTexCoord2f(0,0);  glVertex3f(0,bottom,0);
    glTexCoord2f(1,0);  glVertex3f(0,bottom,length);
    glTexCoord2f(1,1);  glVertex3f(0,top,length);
    glTexCoord2f(0,1);  glVertex3f(0,top,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);


  glPopMatrix();
}

void mural(unsigned int mural_texture, int lighting )
  {
    float shinyvec[1] = {4};
    float white[] = {1,1,1,1};
    float Emission[]  = {0.2, 0.2, 0.2, 0.2,1.0};


    glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT,GL_EMISSION,Emission);

    glPushMatrix();


    glColor3f(1,1,1);



    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D,mural_texture);
    glBegin(GL_QUADS);
    glColor3f(0.6,0.6,0.6);
    glNormal3f(0,0,-1);
    glTexCoord2f(0,0);  glVertex3f(33.5, 7,120);
    glTexCoord2f(1,0);  glVertex3f(-110.5,7,120);
    glTexCoord2f(1,1);  glVertex3f(-110.5,48,120);
    glTexCoord2f(0,1);  glVertex3f(33.5,48,120);
    glEnd();
    glDisable(GL_TEXTURE_2D);


    glPopMatrix();
  }

void skeeball(double x, double y, double z,
  double xScale, double yScale, double zScale,
  double rotateX, double rotateY, unsigned int wood_tex,
  unsigned int skeeball_tex, unsigned int skeeball_body_tex)
{
  glPushMatrix();
  glTranslated(x, y,z);
  glRotated(rotateY, 0,1,0);
  glRotated(rotateX, 1,0,0);
  glScaled(xScale, yScale, zScale);
  //Sides of skeeball
  //Right side outer wall
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, skeeball_body_tex);
  glBegin(GL_QUAD_STRIP);
  glColor3f(.412,.412,.412);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(0,1); glVertex3f(1,.2,0);
  glTexCoord2f(1,1); glVertex3f(0,.4,0);
  glTexCoord2f(1,0); glVertex3f(1,.6,0);
  glEnd();
  //Right side inter wall
  glBegin(GL_QUAD_STRIP);
  glColor3f(.412,.412,.412);
  glTexCoord2f(0,0); glVertex3f(0,0,.1);
  glTexCoord2f(0,1); glVertex3f(1,.2,.1);
  glTexCoord2f(1,1); glVertex3f(0,.4,.1);
  glTexCoord2f(1,0); glVertex3f(1,.6,.1);
  glEnd();
  //Strip on top
  glBegin(GL_QUAD_STRIP);
  glColor3f(.663,.663,.663);
  glNormal3f(0,.8,.2);
  glTexCoord2f(0,0); glVertex3f(0,.4,0);
  glTexCoord2f(0,1); glVertex3f(1,.6,0);
  glTexCoord2f(1,1); glVertex3f(0,.4,.1);
  glTexCoord2f(1,0); glVertex3f(1,.6,.1);
  glEnd();
  //Front cover
  glBegin(GL_QUAD_STRIP);
  glColor3f(.663,.663,.663);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(0,1); glVertex3f(0,.4,0);
  glTexCoord2f(1,1); glVertex3f(0,0,.1);
  glTexCoord2f(1,0); glVertex3f(0,.4,.1);
  glEnd();
  //Left side
  //Outer wall
  glBegin(GL_QUAD_STRIP);
  glColor3f(.412,.412,.412);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(0,0,0.8);
  glTexCoord2f(0,1); glVertex3f(1,.2,0.8);
  glTexCoord2f(1,1); glVertex3f(0,.4,0.8);
  glTexCoord2f(1,0); glVertex3f(1,.6,0.8);
  glEnd();
  //Inner wall
  glBegin(GL_QUAD_STRIP);
  glColor3f(.412,.412,.412);
  glTexCoord2f(0,0); glVertex3f(0,0,.9);
  glTexCoord2f(0,1); glVertex3f(1,.2,.9);
  glTexCoord2f(1,1); glVertex3f(0,.4,.9);
  glTexCoord2f(1,0); glVertex3f(1,.6,.9);
  glEnd();
  //Top strip
  glBegin(GL_QUAD_STRIP);
  glColor3f(.663,.663,.663);
  glNormal3f(0,.8,.2);
  glTexCoord2f(0,0); glVertex3f(0,.4,.8);
  glTexCoord2f(0,1); glVertex3f(1,.6,.8);
  glTexCoord2f(1,1); glVertex3f(0,.4,.9);
  glTexCoord2f(1,0); glVertex3f(1,.6,.9);
  glEnd();
  //Front cover
  glBegin(GL_QUAD_STRIP);
  glColor3f(.663,.663,.663);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(0,0,.8);
  glTexCoord2f(0,1); glVertex3f(0,.4,.8);
  glTexCoord2f(1,1); glVertex3f(0,0,.9);
  glTexCoord2f(1,0); glVertex3f(0,.4,.9);
  glEnd();
  //Ball return
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.5,0.5,0.5);
  glNormal3f(0,.8,.2);
  glTexCoord2f(0,0); glVertex3f(.01,.15,0);
  glTexCoord2f(0,1); glVertex3f(1,.35,0);
  glTexCoord2f(1,1); glVertex3f(.01,.15,.8);
  glTexCoord2f(1,0); glVertex3f(1,.35,.8);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(.01,.15,0);
  glTexCoord2f(0,1); glVertex3f(.01,.15,.8);
  glTexCoord2f(1,1); glVertex3f(.01,.3,0);
  glTexCoord2f(1,0); glVertex3f(.01,.3,.8);
  glEnd();
  //Back STUFF
  //Left
  glBegin(GL_QUAD_STRIP);
  glColor3f(.412, .412, .412);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,0,0);
  glTexCoord2f(0,1); glVertex3f(1.0, .7, 0);
  glTexCoord2f(1,1); glVertex3f(1.4, 0, 0);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, 0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,0,.1);
  glTexCoord2f(0,1); glVertex3f(1,.7,.1);
  glTexCoord2f(1,1); glVertex3f(1.4,0,.1);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, .1);
  glEnd();
  //lil cap
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(1, .6, 0);
  glTexCoord2f(0,1); glVertex3f(1, .7, 0);
  glTexCoord2f(1,1); glVertex3f(1, .6, .1);
  glTexCoord2f(1,0); glVertex3f(1, .7, .1);
  glEnd();
  //top strip
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(1, .7, 0);
  glTexCoord2f(0,1); glVertex3f(1.4, .7, 0);
  glTexCoord2f(1,1); glVertex3f(1, .7, .1);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, .1);
  glEnd();
  //Right
  glBegin(GL_QUAD_STRIP);
  glColor3f(.412, .412, .412);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,0,.8);
  glTexCoord2f(0,1); glVertex3f(1.0, .7, .8);
  glTexCoord2f(1,1); glVertex3f(1.4, 0, .8);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, .8);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,0,.9);
  glTexCoord2f(0,1); glVertex3f(1,.7,.9);
  glTexCoord2f(1,1); glVertex3f(1.4,0,.9);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, .9);
  glEnd();
  //lil cap
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(1, .6, .8);
  glTexCoord2f(0,1); glVertex3f(1, .7, .8);
  glTexCoord2f(1,1); glVertex3f(1, .6, .9);
  glTexCoord2f(1,0); glVertex3f(1, .7, .9);
  glEnd();
  //top strip
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(1, .7, .8);
  glTexCoord2f(0,1); glVertex3f(1.4, .7, .8);
  glTexCoord2f(1,1); glVertex3f(1, .7, .9);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, .9);
  glEnd();
  //Sides of target
  //Left side
  glBegin(GL_TRIANGLE_STRIP);
  glColor3f(.412, .412, .412);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1.1, .7, 0);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, 0);
  glTexCoord2f(1,1); glVertex3f(1.4, 1.4, 0);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1.1, .7, .1);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, .1);
  glTexCoord2f(1,1); glVertex3f(1.4, 1.4, .1);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0, .5, .5);
  glTexCoord2f(0,0); glVertex3f(1.1, .7, 0);
  glTexCoord2f(0,1); glVertex3f(1.4, 1.4, 0);
  glTexCoord2f(1,1); glVertex3f(1.1, .7, .1);
  glTexCoord2f(1,0); glVertex3f(1.4, 1.4, .1);
  glEnd();
  //Right side
  glBegin(GL_TRIANGLE_STRIP);
  glColor3f(.412, .412, .412);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1.1, .7, .8);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, .8);
  glTexCoord2f(1,1); glVertex3f(1.4, 1.4, .8);
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1.1, .7, .9);
  glTexCoord2f(1,0); glVertex3f(1.4, .7, .9);
  glTexCoord2f(1,1); glVertex3f(1.4, 1.4, .9);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0, .5, .5);
  glTexCoord2f(0,0); glVertex3f(1.1, .7, .8);
  glTexCoord2f(0,1); glVertex3f(1.4, 1.4, .8);
  glTexCoord2f(1,1); glVertex3f(1.1, .7, .9);
  glTexCoord2f(1,0); glVertex3f(1.4, 1.4, .9);
  glEnd();
  //Thing at top
  //Back
  glBegin(GL_QUAD_STRIP);
  glColor3f(.822,.822,.822);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(1.4, 1.35, 0);
  glTexCoord2f(0,1); glVertex3f(1.4, 1.35, .9);
  glTexCoord2f(1,1); glVertex3f(1.4, 1.75, 0);
  glTexCoord2f(1,0); glVertex3f(1.4, 1.75, .9);
  glEnd();
  //right Side
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1, 1.75, .9);
  glTexCoord2f(0,1); glVertex3f(1.4, 1.75, .9);
  glTexCoord2f(1,1); glVertex3f(1, 1.35, .9);
  glTexCoord2f(1,0); glVertex3f(1.4, 1.35, .9);
  glEnd();
  //left Side
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1, 1.75, 0);
  glTexCoord2f(0,1); glVertex3f(1.4, 1.75, 0);
  glTexCoord2f(1,1); glVertex3f(1, 1.35, 0);
  glTexCoord2f(1,0); glVertex3f(1.4, 1.35, 0);
  glEnd();
  //bottom
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(1, 1.35, .9);
  glTexCoord2f(0,1); glVertex3f(1, 1.35, 0);
  glTexCoord2f(1,1); glVertex3f(1.4, 1.35, .9);
  glTexCoord2f(1,0); glVertex3f(1.4, 1.35, 0);
  glEnd();
  //top
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(1, 1.75, .9);
  glTexCoord2f(0,1); glVertex3f(1, 1.75, 0);
  glTexCoord2f(1,1); glVertex3f(1.4, 1.75, .9);
  glTexCoord2f(1,0);glVertex3f(1.4, 1.75, 0);
  glEnd();
  //right post
  targetHole(1.05,.7,.85, 90,.2, 1);
  //left post
  targetHole(1.05,.7,.05, 90,.2,1);
  //Back cover
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(1.4, 1.75, 0);
  glTexCoord2f(0,1); glVertex3f(1.4, 1.75, .9);
  glTexCoord2f(1,1); glVertex3f(1.4, 0, 0);
  glTexCoord2f(1,0); glVertex3f(1.4, 0, .9);
  glEnd();
  //Front
  glBindTexture(GL_TEXTURE_2D, skeeball_tex);
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(1, 1.35, 0);
  glTexCoord2f(1,0); glVertex3f(1, 1.35, .9);
  glTexCoord2f(0,1); glVertex3f(1, 1.75, 0);
  glTexCoord2f(1,1); glVertex3f(1, 1.75, .9);
  glEnd();

  //Rolling surface
  glBindTexture(GL_TEXTURE_2D, wood_tex);
  glBegin(GL_QUAD_STRIP);
  glColor3f(.957,.643,.376);
  glNormal3f(0,.8,.2);
  glTexCoord2f(0,0); glVertex3f(.1,.3,0);
  glTexCoord2f(0,1); glVertex3f(1,.5,0);
  glTexCoord2f(1,1); glVertex3f(.1,.3,.8);
  glTexCoord2f(1,0); glVertex3f(1,.5,.8);
  glEnd();
  //ramp
  double step = .01;
  double X = .9;
  glBegin(GL_QUAD_STRIP);
  glColor3f(.957,.643,.376);
  for(int i=0; i<=180; i+=10)
  {
    glNormal3f(Cos(i), Sin(i), 0);
    glTexCoord2f(1,1); glVertex3f(X,.45+Sin(i)/8,.1);
    glTexCoord2f(0,0); glVertex3f(X,.45+Sin(i)/8,.8);
    X += step;
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
  //Target
  target(.7,.8,.7, 21,90,0, wood_tex);
  //Balls
  ball(.1,.2,.2, .05,white,0);
  ball(.1,.2,.3, .05,white,0);
  ball(.07,.2,.4, .05,white,0);
  ball(.06,.2,.5, .05,white,0);
  ball(.06,.2,.6, .05,white,0);
  ball(.14,.2,.55, .05,white,0);

  glPopMatrix();
}

void target(double xScale, double yScale, double zScale, double rotateX, double rotateY, double rotateZ, unsigned int wood_tex)
{
  glPushMatrix();
  glTranslated(1.1,.6,.8);
  glRotated(rotateY, 0,1,0);
  glRotated(rotateX, 1,0,0);
  glRotated(rotateZ, 0,0,1);
  glScaled(xScale, yScale, zScale);
  //Back board
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, wood_tex);
  glBegin(GL_QUADS);
  glColor3f(.957, .643, .376);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(0,1); glVertex3f(0,1,0);
  glTexCoord2f(1,1); glVertex3f(1,1,0);
  glTexCoord2f(1,0); glVertex3f(1,0,0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  //goal holes
  glColor3f(.822, .822, .822);
  targetHole(.5,.39,0,0,1.3,.1);
  targetHole(.5,.61,0,0,1.0,.1);
  targetHole(.5,.42,0,0,3.0,.1);
  targetHole(.5,.82,0,0,1.1,.1);
  targetHole(.15,.85,0,0,1.0,.1);
  targetHole(.85,.85,0,0,1.0,.1);


  glPopMatrix();
}

/*
 *  Draw a peg
 */
void targetHole(float x,float y, float z, float rotx, float r, float h)
{
   int th;
   r/=10;
   float inr=r-.02;
   h *= -1;

   glPushMatrix();
   glTranslatef(x,y,z);
   glRotated(rotx, 1,0,0);
   //  Outside
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=15)
   {
      glNormal3f(Cos(th),Sin(th),0);
      glVertex3f(r*Cos(th),r*Sin(th),0);
      glVertex3f(r*Cos(th),r*Sin(th),h);
   }
   glEnd();
   //  Inside
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=15)
   {
      glNormal3f(-Cos(th),-Sin(th),0);
      glVertex3f(inr*Cos(th),inr*Sin(th),h);
      glVertex3f(inr*Cos(th),inr*Sin(th),0);
   }
   glEnd();
   // inside Edge
   glNormal3f(0,0,-1);
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=15)
   {
      glVertex3f(r*Cos(th),r*Sin(th),h);
      glVertex3f(inr*Cos(th),inr*Sin(th),h);
   }
   glEnd();


   // outside Edge
   glNormal3f(0,0,-1);
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=15)
   {
      glVertex3f(r*Cos(th),r*Sin(th),0);
      glVertex3f(inr*Cos(th),inr*Sin(th),0);
   }
   glEnd();
   glPopMatrix();
}

void floor_panel(double x, double y, double z, double xScale, double yScale, double zScale, double rotateX, double rotateY, unsigned int floor_texture)
{
  glPushMatrix();
  glTranslated(x, y,z);
  glRotated(rotateY, 0,1,0);
  glRotated(rotateX, 1,0,0);
  glScaled(xScale, yScale, zScale);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,floor_texture);
  glBegin(GL_QUADS);
  glNormal3f( 0, 1, 0);
  glTexCoord2f(1,0);  glVertex3f(3,0,0);
  glTexCoord2f(1,1);  glVertex3f(0,0,0);
  glTexCoord2f(0,1);  glVertex3f(0,0,1);
  glTexCoord2f(0,0);  glVertex3f(3,0,1);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void picture(double x, double y, double z, double xScale, double yScale, double zScale, double rotateY, unsigned int frame_texture, unsigned int picture_texture)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(rotateY, 0,1,0);
  glScaled(xScale, yScale, zScale);
  //Picture
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,picture_texture);
  glBegin(GL_QUADS);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(.05,.05,0);
  glTexCoord2f(0,1); glVertex3f(.05,.95,0);
  glTexCoord2f(1,1); glVertex3f(.95,.95,0);
  glTexCoord2f(1,0); glVertex3f(.95,.05,0);
  glEnd();
  glDisable(GL_TEXTURE_2D);

  //frame
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, frame_texture);
  //Left
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(1,0); glVertex3f(0,1,0);
  glTexCoord2f(0,1); glVertex3f(.05,.05,0);
  glTexCoord2f(1,1); glVertex3f(.05,.95,0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(0,0,.05);
  glTexCoord2f(1,0); glVertex3f(0,1,.05);
  glTexCoord2f(0,1); glVertex3f(.05,.05,.05);
  glTexCoord2f(1,1); glVertex3f(.05,.95,.05);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(0,1,.05);
  glTexCoord2f(1,0); glVertex3f(0,0,.05);
  glTexCoord2f(0,1); glVertex3f(0,1,0);
  glTexCoord2f(1,1); glVertex3f(0,0,0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(.05,.95,.05);
  glTexCoord2f(1,0); glVertex3f(.05,.95,0);
  glTexCoord2f(0,1); glVertex3f(.05,.05,.05);
  glTexCoord2f(1,1); glVertex3f(.05,.05,0);
  glEnd();
  //Top
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(1,0); glVertex3f(0,1,0);
  glTexCoord2f(0,1); glVertex3f(1,1,0);
  glTexCoord2f(0,0); glVertex3f(.05,.95,0);
  glTexCoord2f(1,1); glVertex3f(.95,.95,0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(1,0); glVertex3f(0,1,.05);
  glTexCoord2f(0,1); glVertex3f(1,1,.05);
  glTexCoord2f(0,0); glVertex3f(.05,.95,.05);
  glTexCoord2f(1,1); glVertex3f(.95,.95,.05);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(0,1,.05);
  glTexCoord2f(1,0); glVertex3f(1,1,.05);
  glTexCoord2f(0,1); glVertex3f(0,1,0);
  glTexCoord2f(1,1); glVertex3f(1,1,0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(.05,.95,.05);
  glTexCoord2f(1,0); glVertex3f(.95,.95,.05);
  glTexCoord2f(0,1); glVertex3f(.05,.95,0);
  glTexCoord2f(1,1); glVertex3f(.95,.95,0);
  glEnd();
  //Right
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(1,0,0);
  glTexCoord2f(1,0); glVertex3f(1,1,0);
  glTexCoord2f(1,1); glVertex3f(.95,.05,0);
  glTexCoord2f(0,1); glVertex3f(.95,.95,0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(1,0,.05);
  glTexCoord2f(1,0); glVertex3f(1,1,.05);
  glTexCoord2f(1,1); glVertex3f(.95,.05,.05);
  glTexCoord2f(0,1); glVertex3f(.95,.95,.05);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,1,.05);
  glTexCoord2f(1,0); glVertex3f(1,0,.05);
  glTexCoord2f(1,1); glVertex3f(1,1,0);
  glTexCoord2f(0,1); glVertex3f(1,0,0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(.95,.95,.05);
  glTexCoord2f(1,0); glVertex3f(.95,.05,.05);
  glTexCoord2f(0,1); glVertex3f(.95,.95,0);
  glTexCoord2f(1,1); glVertex3f(.95,.05,0);
  glEnd();
  //Bottom
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(0,0,0);
  glTexCoord2f(1,0); glVertex3f(1,0,0);
  glTexCoord2f(0,1); glVertex3f(.05,.05,0);
  glTexCoord2f(1,1); glVertex3f(.95,.05,0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(0,0,.05);
  glTexCoord2f(1,0); glVertex3f(1,0,.05);
  glTexCoord2f(0,1); glVertex3f(.05,.05,.05);
  glTexCoord2f(1,1); glVertex3f(.95,.05,.05);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(1,0); glVertex3f(0,0,.05);
  glTexCoord2f(0,0); glVertex3f(1,0,.05);
  glTexCoord2f(0,1); glVertex3f(0,0,0);
  glTexCoord2f(1,1); glVertex3f(1,0,0);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(.05,.05,.05);
  glTexCoord2f(1,1); glVertex3f(.05,.05,.05);
  glTexCoord2f(1,0); glVertex3f(.95,.05,.05);
  glTexCoord2f(0,1); glVertex3f(.95,.05,0);
  glEnd();

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void cabinet(double x, double y, double z, double xScale, double yScale, double zScale, double rotateX, double rotateY, unsigned int title_tex, unsigned int body_tex,
              unsigned int side_tex)
{
  glPushMatrix();
  glTranslated(x, y,z);
  glRotated(rotateY, 0,1,0);
  glRotated(rotateX, 1,0,0);
  glScaled(xScale, yScale, zScale);
  double Xmin=0,Xmax=1,t=0,Y;
  double deriv[3] = {0,0,1};
  double step=.05;
  //Side of cabient
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, side_tex);
  glBegin(GL_QUAD_STRIP);
  for(Y=0; Y<=1; Y+=step)
  {
    if(Y>=.4 && Y<=.45)
      Xmin-=.15;
    else if(Y>=.55 && Y<=.6)
      Xmin+=.15;
    else if(Y>.6 && Y<=.8)
      bezier(t,&Xmin,Y,deriv);
    else if(Y>.8)
      Xmax-=.05;
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(Xmin, Y, 0);
    glTexCoord2f(1,0); glVertex3f(Xmax, Y, 0);
    t+=step;
  }
  glEnd();
  //depth!
  Xmin=0,Xmax=1,t=0;
  glBegin(GL_QUAD_STRIP);
  for(Y=0; Y<=1; Y+=step)
  {
    if(Y>=.4 && Y<=.45)
      Xmin-=.15;
    else if(Y>=.55 && Y<=.6)
      Xmin+=.15;
    else if(Y>.6 && Y<=.8)
      bezier(t,&Xmin,Y,deriv);
    else if(Y>.8)
      Xmax-=.05;
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(Xmin, Y, .05);
    glTexCoord2f(1,0); glVertex3f(Xmax, Y, .05);
    t+=step;
  }
  glEnd();
  //connect the two on front
  Xmin=0,t=0;
  glBegin(GL_QUAD_STRIP);
  glColor3f(.622,.622,.622);
  for(Y=0; Y<=1; Y+=step)
  {
    glNormal3f(1,0,0);
    if(Y>=.4 && Y<=.45)
      Xmin-=.15;
    else if(Y>=.55 && Y<=.6)
      Xmin+=.15;
    else if(Y>.6 && Y<=.8)
      bezier(t,&Xmin,Y, deriv);
    glNormal3dv(deriv);
    glTexCoord2f(0,0); glVertex3f(Xmin, Y, 0);
    glTexCoord2f(1,0); glVertex3f(Xmin, Y, .05);
    t+=step;
  }
  glEnd();

  //Other side of cabient
  Xmin=0,Xmax=1,t=0;
  glBegin(GL_QUAD_STRIP);
  for(Y=0; Y<=1; Y+=step)
  {
    if(Y>=.4 && Y<=.45)
      Xmin-=.15;
    else if(Y>=.55 && Y<=.6)
      Xmin+=.15;
    else if(Y>.6 && Y<=.8)
      bezier(t,&Xmin,Y,deriv);
    else if(Y>.8)
      Xmax-=.05;
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(Xmin, Y, .9);
    glTexCoord2f(1,0); glVertex3f(Xmax, Y, .9);
    t+=step;
  }
  glEnd();
  //depth!
  Xmin=0,Xmax=1,t=0;
  glBegin(GL_QUAD_STRIP);
  for(Y=0; Y<=1; Y+=step)
  {
    if(Y>=.4 && Y<=.45)
      Xmin-=.15;
    else if(Y>=.55 && Y<=.6)
      Xmin+=.15;
    else if(Y>.6 && Y<=.8)
      bezier(t,&Xmin,Y,deriv);
    else if(Y>.8)
      Xmax-=.05;
    glNormal3f(0,0,1);
    glTexCoord2f(0,0); glVertex3f(Xmin, Y, .85);
    glTexCoord2f(1,0); glVertex3f(Xmax, Y, .85);
    t+=step;
  }
  glEnd();
  //connect the two on front
  Xmin=0,t=0;
  glBegin(GL_QUAD_STRIP);
  glColor3f(.622,.622,.622);
  for(Y=0; Y<=1; Y+=step)
  {
    glNormal3f(1,0,0);
    if(Y>=.4 && Y<=.45)
      Xmin-=.15;
    else if(Y>=.55 && Y<=.6)
      Xmin+=.15;
    else if(Y>.6 && Y<=.8)
      bezier(t,&Xmin,Y, deriv);
    glNormal3dv(deriv);
    glTexCoord2f(0,0); glVertex3f(Xmin, Y, .9);
    glTexCoord2f(1,0); glVertex3f(Xmin, Y, .85);
    t+=step;
  }
  glEnd();
  //Top front of machine with game name
  glBindTexture(GL_TEXTURE_2D, title_tex);
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,0,1);
  glTexCoord2f(0,1); glVertex3f(Xmin+.05, .95, 0);
  glTexCoord2f(0,0); glVertex3f(Xmin+.05, .8, 0);
  glTexCoord2f(1,1); glVertex3f(Xmin+.05, .95, .9);
  glTexCoord2f(1,0); glVertex3f(Xmin+.05, .8, .9);
  glEnd();
  //Connect top
  glBindTexture(GL_TEXTURE_2D, body_tex);
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(Xmin+.05, .95, .9);
  glTexCoord2f(1,0); glVertex3f(Xmin+.05, .95, 0);
  glTexCoord2f(1,1); glVertex3f(Xmax, .95, .9);
  glTexCoord2f(0,1); glVertex3f(Xmax, .95, 0);
  glEnd();
  //Back diagonal
  glBegin(GL_QUAD_STRIP);
  glNormal3f(.02,.02,0);
  glTexCoord2f(0,0); glVertex3f(Xmax, .95, .9);
  glTexCoord2f(1,0); glVertex3f(Xmax, .95, 0);
  glTexCoord2f(1,1); glVertex3f(1, .75, .9);
  glTexCoord2f(0,1); glVertex3f(1, .75, 0);
  glEnd();
  //Back
  glBegin(GL_QUAD_STRIP);
  glNormal3f(1,0,0);
  glTexCoord2f(0,0); glVertex3f(1,0,.9);
  glTexCoord2f(1,0); glVertex3f(1,.75,.9);
  glTexCoord2f(1,1); glVertex3f(1,0,0);
  glTexCoord2f(0,1); glVertex3f(1,.75,0);
  glEnd();
  //Under the top Thing
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(Xmin+.05, .8, 0);
  glTexCoord2f(1,0); glVertex3f(Xmax-.05, .8, 0);
  glTexCoord2f(1,1); glVertex3f(Xmin+.05, .8, .9);
  glTexCoord2f(0,1); glVertex3f(Xmax-.05, .8, .9);
  glEnd();
  //Lower front with coin slot and STUFF
  glBegin(GL_QUAD_STRIP);
  glColor3f(.5,.5,.5);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(.1, .45, .05);
  glTexCoord2f(1,0); glVertex3f(.1, 0, .05);
  glTexCoord2f(1,1); glVertex3f(.1, .45, .85);
  glTexCoord2f(0,1); glVertex3f(.1, 0, .85);
  glEnd();
  //Screen
  glBegin(GL_QUAD_STRIP);
  glColor3f(0,0,0);
  glNormal3f(.28, -.44, 0);
  glVertex3f(.2, .55, .05);
  glVertex3f(.75, .9, .05);
  glVertex3f(.2, .55, .85);
  glVertex3d(.75, .9, .85);
  glEnd();
  //controls front
  glBegin(GL_QUAD_STRIP);
  glColor3f(.622, .622, .622);
  glNormal3f(0,0,1);
  glTexCoord2f(0,0); glVertex3f(-.1, .45, .05);
  glTexCoord2f(1,0); glVertex3f(-.1, .55, .05);
  glTexCoord2f(1,1); glVertex3f(-.1, .45, .85);
  glTexCoord2f(0,1); glVertex3f(-.1, .55, .85);
  glEnd();
  //Top
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-.1, .55, .05);
  glTexCoord2f(1,0); glVertex3f(.2, .55, .05);
  glTexCoord2f(1,1); glVertex3f(-.1, .55, .85);
  glTexCoord2f(0,1); glVertex3f(.2, .55, .85);
  glEnd();
  //Bottom
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0); glVertex3f(-.1, .45, .05);
  glTexCoord2f(1,0); glVertex3f(.2, .45, .05);
  glTexCoord2f(1,1); glVertex3f(-.1, .45, .85);
  glTexCoord2f(0,1); glVertex3f(.2, .45, .85);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  //controls
  //joystick and buttons
  targetHole(.05,.55,.3, 90, .05,.05);
  ball(.05,.6,.3, .025,red,0);
  button(.05,.55,.5, 1,1,1, blue);
  button(.05,.55,.6, 1,1,1, red);
  button(.1,.55,.55, 1,1,1, green);
  button(0,.55,.55, 1,1,1, yellow);

  //A E S T H E T I C S
  //Left of stick
  glBegin(GL_TRIANGLE_FAN);
  glColor3f(0,0,0);
  glNormal3f(0,1,0);
  glVertex3f(.03,.551,.25);
  double r=.02;
  for(int th=0; th<=360; th+=15)
  {
    glVertex3f(r*Cos(th)+.05,.551,r*Sin(th)+.25);
  }
  glEnd();
  //Right
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,1,0);
  glVertex3f(.03,.551,.35);
  for(int th=0; th<=360; th+=15)
  {
    glVertex3f(r*Cos(th)+.05,.551,r*Sin(th)+.35);
  }
  glEnd();
  //Front
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,1,0);
  glVertex3f(0,.551,.3);
  for(int th=0; th<=360; th+=15)
  {
    glVertex3f(r*Cos(th),.551,r*Sin(th)+.3);
  }
  glEnd();
  //Back
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,1,0);
  glVertex3f(.1,.551,.3);
  for(int th=0; th<=360; th+=15)
  {
    glVertex3f(r*Cos(th)+.1,.551,r*Sin(th)+.3);
  }
  glEnd();
  //Movement paths
  glBegin(GL_QUAD_STRIP);
  glNormal3f(0,1,0);
  glVertex3f(.1,.551,.28);
  glVertex3f(0,.551,.28);
  glVertex3f(.1,.551,.32);
  glVertex3f(0,.551,.32);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glNormal3d(0,1,0);
  glVertex3f(.03,.551,.25);
  glVertex3f(.07,.551,.25);
  glVertex3f(.03,.551,.35);
  glVertex3d(.07,.551,.35);
  glEnd();


  glPopMatrix();
}

void bezier(double t, double* x, double y, double* deriv)
{
    double p0[2] = {0,.6};
    double p1[2] = {.3,.8};
    double pc[2] = {*x,y};
    *x = (1-t)*((1-t)*(p0[0])+t*(p1[0]))+t*((1-t)*(p1[0])+t*(pc[0]));
    deriv[0] = -(p0[0])*(2*t-2)+(2*(pc[0])-4*(p1[0]))*t+2*(p1[0]);
    deriv[1] = -(p0[1])*(2*t-2)+(2*(pc[1])-4*(p1[1]))*t+2*(p1[1]);
}

void button(double x, double y, double z, double xScale, double yScale, double zScale, float color[])
{
  float th, step=15, h=.01, r=.025;
  glPushMatrix();
  glTranslated(x, y, z);
  //glScaled(xScale, yScale, zScale);
  glColor3f(color[0], color[1], color[2]);
  glBegin(GL_QUAD_STRIP);
  for (th=0;th<=360;th+=step)
  {
     glNormal3f(Cos(th),0,Sin(th));
     glVertex3f(r*Cos(th),0,r*Sin(th));
     glVertex3f(r*Cos(th),h,r*Sin(th));
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0,h,0);
  for(th=0; th<=360; th +=step)
  {
    glNormal3f(0,1,0);
    glVertex3f(r*Cos(th),h,r*Sin(th));
  }
  glEnd();

  glPopMatrix();
}
