//
//  plane.h
//  project
//
//  Created by Isaac Dunnett on 2021-12-02.
//

#ifndef plane_h
#define plane_h

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readInPlane(void);
void readInPropeller(void);
void randomPlaneColors(void);
void drawPlane(void);
void drawPropeller(void);
void loadPlane(void);
void displayPlaneSpeed(GLfloat);
void displayPlaneAltitude(GLfloat);

#endif /* plane_h */
