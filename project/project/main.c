//Isaac Dunnett (B00800378)
//CSCI 3136: Intro to Graphics and Animation
//Final Project

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include "scene.h"
#include "plane.h"

GLint windowWidth = 800;
GLint windowHeight = 400;
const float pi = 3.14159;
GLfloat lightPosition1[] = { 0.0, 300.0, -150.0, 1.0 };

const int gridSize = 100;

bool wireFrameGrid = false;
bool mountains = true, planeStats = false;
GLfloat altitudeSpeed = 0, turnSpeed = 0, planeSpeed = 0;
GLfloat propellerAngle = 0;
GLfloat planeYAxisAngle = 0;

GLdouble cameraX = 0, cameraY = 4, cameraZ = 10;
GLdouble planeX = 0, planeY = 3, planeZ = 5;

GLfloat zeroMaterial[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat blue[] = {0, 0, 1, 1}, yellow[] = {1, 1, 0, 1}, black[] = {0, 0, 0, 1}, purple[] = {0.5, 0, 1, 1}, red[] = {1, 0, 0, 1}, green[] = {0, 1, 0, 1}, white[] = {1, 1, 1, 1};

int rollDirection = 0;
GLfloat rollAngle = 0;

//Display callback, clears frame buffer and depth buffer,
//Draws planets, stars & spaceship
void myDisplay(void)
{
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // load the identity matrix into the model view matrix
    glLoadIdentity();
    // set the camera position
    gluLookAt(cameraX, cameraY + 1, cameraZ, planeX, planeY + 1, planeZ, 0, 1, 0);
//    gluLookAt(0, 2, 10, 0, 2, 0, 0, 1, 0);
    
    // position light 0
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
    
    if(wireFrameGrid)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
//    drawUnitVectors();
//    drawGrid(gridSize, wireFrameGrid);
    
    drawDisk();
    drawCylinder();
    
    if(mountains){
        glPushMatrix();
        glTranslatef(150, -1, 100);
        glScalef(10, 10, 10);
        drawIsland1(wireFrameGrid);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-200, -1, -250);
        glScalef(10, 10, 10);
        drawIsland2(wireFrameGrid);
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-250, -1, 250);
        glScalef(10, 10, 10);
        drawIsland3(wireFrameGrid);
        glPopMatrix();
    }
    
    glPushMatrix();
    glTranslatef(planeX, planeY, planeZ);
    glRotatef(planeYAxisAngle *-57.3, 0, 1, 0);
    if(rollDirection != 0){
        glRotatef(-rollAngle, 1, 0, 0);
    } else{
        glRotatef(-turnSpeed*500, 1, 0, 0);
    }
    glScalef(3, 3, 3);
    drawPlane();
    // ***propeller 1***
    glPushMatrix();
    glTranslatef(-0.575, -0.15, 0.35);
    glRotatef(-propellerAngle*20, 1, 0, 0);
    glTranslatef(0.575, 0.15, -0.35);
    drawPropeller();
    glPopMatrix();
    // **************
    // ***propeller 2***
    glPushMatrix();
    glTranslatef(0, 0, -0.7);
    glTranslatef(-0.575, -0.15, 0.35);
    glRotatef(-propellerAngle*20, 1, 0, 0);
    glTranslatef(0.575, 0.15, -0.35);
    drawPropeller();
    glPopMatrix();
    // **************
    glPopMatrix();
    
    if(planeStats){
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 1);
        glRasterPos3f(planeX, planeY+4.75, planeZ);
        displayPlaneAltitude(planeY);
        glRasterPos3f(planeX, planeY+4.25, planeZ);
        displayPlaneSpeed(planeSpeed);
        glEnable(GL_LIGHTING);
    }
    
    // swap the drawing buffers
    glutSwapBuffers();
}

//Updates the animation when idle.
void myIdle(void)
{
    propellerAngle += planeSpeed;
    if(planeSpeed >= 0){
        planeX += (planeX - cameraX)/10 * planeSpeed;
        planeZ += (planeZ - cameraZ)/10 * planeSpeed;
    }
    cameraY += altitudeSpeed;
    planeY += altitudeSpeed;
    planeYAxisAngle += turnSpeed;
    cameraX = planeX + 10 * cos(planeYAxisAngle);
    cameraZ = planeZ + 10 * sin(planeYAxisAngle);
    if(rollDirection > 0){
        rollAngle += 10;
        if(rollAngle > 360){
            rollDirection = 0;
            rollAngle = 0;
        }
    }else if (rollDirection < 0){
        rollAngle -= 10;
        if(rollAngle < -360){
            rollDirection = 0;
            rollAngle = 0;
        }
    }
    // redraw the new state
    glutPostRedisplay();
}

//regular key press functions
void onKeyPress(unsigned char key, int x, int y){
    if(key == 's')
        wireFrameGrid = !wireFrameGrid;
    
    if(key == 'm')
        mountains = !mountains;
    
    if(key == 'p')
        planeStats = !planeStats;
    
    if(key == 'c')
        randomPlaneColors();
    
    if(key == 'f')
        glutFullScreen();
    
    if(key == 'q')
        exit(0);
}

//regular key press functions
void onSpecialKeyUp(int key, int x, int y){
    if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN){
        altitudeSpeed = 0.0;
    }
    if(key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT){
        turnSpeed = 0.0;
    }
}

//special key press functions
void onSpecialKeyPress(int key, int x, int y){
    if(key == GLUT_KEY_UP){
        altitudeSpeed = 0.5;
    }
    if(key == GLUT_KEY_DOWN){
        altitudeSpeed = -0.5;
    }
    if(key == GLUT_KEY_RIGHT){
        rollDirection = 1;
    }
    if(key == GLUT_KEY_LEFT){
        rollDirection = -1;
    }
    if(key == GLUT_KEY_PAGE_UP)
        planeSpeed += 0.1;
    if(key == GLUT_KEY_PAGE_DOWN && planeSpeed >= 0.1)
        planeSpeed -= 0.1;
}

void passiveMotion(int x, int y){
    turnSpeed = (x - (GLfloat)windowWidth/2)/10000;
}

void resize(int newWidth, int newHeight)
{
    // update the new width
    windowWidth = newWidth;
    // update the new height
    windowHeight = newHeight;
    
    // update the viewport to still be all of the window
    glViewport (0, 0, windowWidth, windowHeight);
    
    // change into projection mode so that we can change the camera properties
    glMatrixMode(GL_PROJECTION);
    
    // load the identity matrix into the projection matrix
    glLoadIdentity();
    
    // gluPerspective(fovy, aspect, near, far)
    gluPerspective(45, (float)windowWidth/(float)windowHeight, 0.1, 5000);
    
    // change into model-view mode so that we can change the object positions
    glMatrixMode(GL_MODELVIEW);
}

//Initializes the OpenGL rendering context for display.
void initializeGL(void)
{
    // define the light color and intensity
    GLfloat ambientLight[]    = { 0.0, 0.0, 0.0, 1.0 };  // relying on global ambient
    GLfloat diffuseLight[]    = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specularLight[]    = { 1.0, 1.0, 1.0, 1.0 };
    
    //  the global ambient light level
    GLfloat globalAmbientLight[] = { 0.4, 0.4, 0.4, 1.0 };
    
    // set the global ambient light level
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);
    
    // define the color and intensity for light 0
    glLightfv(GL_LIGHT0, GL_AMBIENT,   ambientLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR,  diffuseLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,   specularLight);
    
    // enable lighting
    glEnable(GL_LIGHTING);
    // enable light 0
    glEnable(GL_LIGHT0);
    
    
    // turn on depth testing so that polygons are drawn in the correct order
    glEnable(GL_DEPTH_TEST);
    
    // make sure the normals are unit vectors
    glEnable(GL_NORMALIZE);

    glEnable(GL_LINE_SMOOTH);
        
    // change into projection mode so that we can change the camera properties
    glMatrixMode(GL_PROJECTION);
    
    // gluPerspective(fovy, aspect, near (positive), far (positive))
    gluPerspective(45, (float)windowWidth/(float)windowHeight, 0.1, 5000);
    
    // change into model-view mode so that we can change the object positions
    glMatrixMode(GL_MODELVIEW);
    
    // load the identity matrix into the projection matrix
    glLoadIdentity();
}

//Sets up the openGL rendering context and the windowing
//system, then begins the display loop.
int main(int argc, char** argv)
{
    // initialize the toolkit
    glutInit(&argc, argv);
    // set display mode
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    // set window size
    glutInitWindowSize(windowWidth,windowHeight);
    // set window position on screen
    glutInitWindowPosition(400, 100);
    // open the screen window
    glutCreateWindow("Project");
    
    // register redraw function
    glutDisplayFunc(myDisplay);
    // register the idle function
    glutIdleFunc(myIdle);
    // register the key functions
    glutKeyboardFunc(onKeyPress);
    glutSpecialUpFunc(onSpecialKeyUp);
    glutSpecialFunc(onSpecialKeyPress);
    
    glutPassiveMotionFunc(passiveMotion);
    // register the resize function
    glutReshapeFunc(resize);
    
    //initialize the rendering context
    initializeGL();
    
    srand((unsigned) time(NULL) * getpid());
    
    loadPlane();
    generateIslands();
    loadScene();
    
    // go into a perpetual loop
    glutMainLoop();
}

