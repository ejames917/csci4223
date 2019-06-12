#ifndef arcade
#define arcade

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "CSCIx229.h"

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void ceiling(double x, double y, double z, double xScale, double yScale, double zScale, double rotateX, double rotateY, unsigned int ceiling_texture);

void wall(double x, double y, double z, double xScale, double yScale, double zScale, double rotateX, double rotateY, unsigned int wall_texture );

void mural(unsigned int mural_texture, int lighting);

void upcurve(double x, double y, double z, double xScale, double yScale, double zScale, double rotateX, double rotateY );

void floor_panel(double x, double y, double z, double xScale, double yScale, double zScale, double rotateX, double rotateY, unsigned int floor_texture);

void skeeball(double x, double y, double z, double xScale, double yScale, double zScale, double rotateX, double rotateY, unsigned int wood_tex, unsigned int skeeball_tex, unsigned int skeeball_body_tex);

void target(double xScale, double yScale, double zScale, double rotateX, double rotateY, double rotateZ, unsigned int wood_tex);

void targetHole(float x,float y, float z, float rotZ, float r, float h);

void picture(double x, double y, double z, double xScale, double yScale, double zScale, double rotateY, unsigned int frame_texture, unsigned int picture_texture);

void ball(double x,double y,double z, double r, float color[], int light);

void Vertex(double th,double ph);

void cabinet(double x, double y, double z, double xScale, double yScale, double zScale, double rotateX, double rotateY, unsigned int title_tex, unsigned int body_tex, unsigned int side_tex);

void bezier(double t, double* x, double y, double* deriv);

void button(double x, double y, double z, double xScale, double yScale, double zScale, float color[]);

#ifdef __cplusplus
}
#endif

#endif
