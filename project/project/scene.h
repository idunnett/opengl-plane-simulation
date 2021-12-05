//
//  scene.h
//  project
//
//  Created by Isaac Dunnett on 2021-12-01.
//

#ifndef scene_h
#define scene_h

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void drawUnitVectors(void);
void drawGrid(int, bool);
void drawDisk(void);
void drawCylinder(void);
void generateIsland2d(void);
void generateIsland3d(int, int);
void generateIslands(void);
void drawIsland2d(void);
void drawIsland1(bool);
void drawIsland2(bool);
void drawIsland3(bool);
void loadScene(void);

#endif /* scene_h */
