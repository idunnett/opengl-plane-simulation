//
//  scene.c
//  project
//
//  Created by Isaac Dunnett on 2021-12-01.
//
//
#include "scene.h"

extern GLfloat zeroMaterial[];
extern GLfloat blue[], yellow[], black[], purple[], red[], green[], white[];

float randomInRange(float min, float max)
{
    float random = (float) rand() / (float) RAND_MAX;
    float range = max - min;
    return (random*range) + min;
}

void drawUnitVectors(void){
    int lineLength = 3;
    glLineWidth(5);
    //  X line
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroMaterial);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(lineLength, 0, 0);
    glEnd();
    //  Z line
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroMaterial);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, lineLength);
    glEnd();
    //  Y line
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroMaterial);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, lineLength, 0);
    glEnd();
    // Sphere
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroMaterial);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 0);
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glutSolidSphere(0.75, 20, 20);
    glPopMatrix();
}

void drawGrid(int gridSize, bool wireFrameGrid){
    float quadSize = 3;
    glLineWidth(1);
    if(wireFrameGrid)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    GLfloat blue[] = {0.34, 0.32, 0.49, 1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroMaterial);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 0);
    glBegin(GL_QUADS);
    for(GLfloat x = gridSize/2 * -quadSize; x < gridSize/2 * quadSize; x+=quadSize){
        for(GLfloat z = gridSize/2 * -quadSize; z < gridSize/2 * quadSize; z+=quadSize){
            glVertex3f(x, 0, z);
            glVertex3f(x, 0, z+quadSize);
            glVertex3f(x+quadSize, 0, z+quadSize);
            glVertex3f(x+quadSize, 0, z);
        }
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int diskImageTextureWidth, diskImageTextureHeight;
int cylinderImageTextureWidth, cylinderImageTextureHeight;
/************************************************************************
 
 Function:        loadImage
 
 Description:    Loads in the PPM image
 
 *************************************************************************/
GLubyte* loadImage(char* fileName, bool isDisk){
    GLubyte *imageData;
    // the ID of the image file
    FILE *fileID;
    // maxValue
    int  maxValue;
    // total number of pixels in the image
    int  totalPixels;
    // temporary character
    char tempChar;
    // counter variable for the current pixel in the image
    int i;
    // array for reading in header information
    char headerLine[100];
    // if the original values are larger than 255
    float RGBScaling;
    // temporary variables for reading in the red, green and blue data of each pixel
    int red, green, blue;
    // open the image file for reading
    fileID = fopen(fileName, "r");
    // read in the first header line
    //    - "%[^\n]"  matches a string of all characters not equal to the new line character ('\n')
    //    - so we are just reading everything up to the first line break
    fscanf(fileID,"%[^\n] ", headerLine);
    // make sure that the image begins with 'P3', which signifies a PPM file
    if ((headerLine[0] != 'P') || (headerLine[1] != '3'))
    {
        printf("This is not a PPM file!\n");
        exit(0);
    }
    // read in the first character of the next line
    fscanf(fileID, "%c", &tempChar);
    // while we still have comment lines (which begin with #)
    while(tempChar == '#')
    {
        // read in the comment
        fscanf(fileID, "%[^\n] ", headerLine);
        
        // print the comment
        printf("%s\n", headerLine);
        
        // read in the first character of the next line
        fscanf(fileID, "%c",&tempChar);
    }
    // the last one was not a comment character '#', so we need to put it back into the file stream (undo)
    ungetc(tempChar, fileID);
    
    if(isDisk){
        // read in the image hieght, width and the maximum value
        fscanf(fileID, "%d %d %d", &diskImageTextureWidth, &diskImageTextureHeight, &maxValue);
        
        // print out the information about the image file
        printf("%d rows  %d columns  max value= %d\n", diskImageTextureHeight, diskImageTextureWidth, maxValue);
        totalPixels = diskImageTextureWidth * diskImageTextureHeight;
    }
    else{
        // read in the image hieght, width and the maximum value
        fscanf(fileID, "%d %d %d", &cylinderImageTextureWidth, &cylinderImageTextureHeight, &maxValue);
        
        // print out the information about the image file
        printf("%d rows  %d columns  max value= %d\n", cylinderImageTextureHeight, cylinderImageTextureWidth, maxValue);
        // compute the total number of pixels in the image
        totalPixels = cylinderImageTextureWidth * cylinderImageTextureHeight;
    }
    
    // allocate enough memory for the image  (3*) because of the RGB data
    imageData = malloc(3 * sizeof(GLuint) * totalPixels);
    
    
    // determine the scaling for RGB values
    RGBScaling = 255.0 / maxValue;
    
    
    // if the maxValue is 255 then we do not need to scale the
    //    image data values to be in the range or 0 to 255
    if (maxValue == 255)
    {
        for(i = 0; i < totalPixels; i++)
        {
            // read in the current pixel from the file
            fscanf(fileID,"%d %d %d",&red, &green, &blue );
            
            // store the red, green and blue data of the current pixel in the data array
            imageData[3*totalPixels - 3*i - 3] = red;
            imageData[3*totalPixels - 3*i - 2] = green;
            imageData[3*totalPixels - 3*i - 1] = blue;
        }
    }
    else  // need to scale up the data values
    {
        for(i = 0; i < totalPixels; i++)
        {
            // read in the current pixel from the file
            fscanf(fileID,"%d %d %d",&red, &green, &blue );
            
            // store the red, green and blue data of the current pixel in the data array
            imageData[3*totalPixels - 3*i - 3] = red   * RGBScaling;
            imageData[3*totalPixels - 3*i - 2] = green * RGBScaling;
            imageData[3*totalPixels - 3*i - 1] = blue  * RGBScaling;
        }
    }
    
    
    // close the image file
    fclose(fileID);
    return imageData;
}

GLdouble radius = 50;
GLint slices = 50, stacks = 10;
GLubyte* diskImageTexture;
GLubyte* cylinderImageTexture;
GLfloat emission[] = {1, 1, 1, 1};
GLuint diskTextureId = 1, cylinderTextureId = 1;

void drawDisk(void){
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroMaterial);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 0);
    glMaterialfv(GL_FRONT_AND_BACK,  GL_EMISSION, emission);
    
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_FOG);
    GLfloat fogColor[] = {1, 0.58, 0.6, 1};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, 0.0025);
    
    glBindTexture(GL_TEXTURE_2D, diskTextureId);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glScalef(26, 26, 26);
    gluDisk(quad, 0, radius, slices, stacks);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_FOG);
    glMaterialfv(GL_FRONT_AND_BACK,  GL_EMISSION, zeroMaterial);
}

void drawCylinder(void){
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroMaterial);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 0);
    glMaterialfv(GL_FRONT_AND_BACK,  GL_EMISSION, emission);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cylinderTextureId);
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glPushMatrix();
    glTranslatef(0, -15, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(25, 25, 25);
    gluCylinder(quad, radius, radius, 30, slices, stacks);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glMaterialfv(GL_FRONT_AND_BACK,  GL_EMISSION, zeroMaterial);
}

GLfloat island2d[100][2][3];

void generateIsland2d(void){
    int smoothness = 4;
    int levels = 1;
    
//    initial pyramid
    island2d[0][0][0] = -3.0f;
    island2d[0][0][1] = 0.0f;
    island2d[0][0][2] = 0.0f;
    island2d[0][1][0] = 0.0f;
    island2d[0][1][1] = 5.0f;
    island2d[0][1][2] = 0.0f;
    island2d[1][0][0] = 0.0f;
    island2d[1][0][1] = 5.0f;
    island2d[1][0][2] = 0.0f;
    island2d[1][1][0] = 3.0f;
    island2d[1][1][1] = 0.0f;
    island2d[1][1][2] = 0.0f;
        
//    i is equivalent to each level
    for (int i = 0; i < smoothness; i++) {
        levels *= 2;
        GLfloat temp[levels*2][2][3];
        int lineCount = 0;
        printf("1/levels = 1/%d = %f\n", levels, 1/(float)levels);
        for (int j = 0; j < levels; j ++) {
            // Line 1
            temp[lineCount][0][0] = island2d[j][0][0];
            temp[lineCount][0][1] = island2d[j][0][1];
            temp[lineCount][0][2] = island2d[j][0][2];

            GLfloat randomHeight;
            if(island2d[j][0][1] > island2d[j][1][1]){
                GLfloat distanceBetween = island2d[j][0][1] - island2d[j][1][1];
                GLfloat padding = distanceBetween * 1/levels;
                randomHeight = randomInRange(island2d[j][1][1]+padding, island2d[j][0][1] - padding);
            }
            else{
                GLfloat distanceBetween = island2d[j][1][1] - island2d[j][0][1];
                GLfloat padding = distanceBetween * 1/levels;
                randomHeight = randomInRange(island2d[j][0][1]+padding, island2d[j][1][1]-padding);
            }
            GLfloat halfWidth = (island2d[j][1][0] + island2d[j][0][0])/2;
            temp[lineCount][1][0] = halfWidth;
            temp[lineCount][1][1] = randomHeight;
            temp[lineCount][1][2] = island2d[j][0][2];
            printf("temp[%d] = [(%f, %f, %f), (%f, %f, %f)]\n", lineCount, temp[lineCount][0][0], temp[lineCount][0][1], temp[lineCount][0][2], temp[lineCount][1][0], temp[lineCount][1][1], temp[lineCount][1][2]);

            lineCount++;
            // Line 2
            temp[lineCount][0][0] = halfWidth;
            temp[lineCount][0][1] = randomHeight;
            temp[lineCount][0][2] = island2d[j][1][2];

            temp[lineCount][1][0] = island2d[j][1][0];
            temp[lineCount][1][1] = island2d[j][1][1];
            temp[lineCount][1][2] = island2d[j][1][2];
            printf("temp[%d] = [(%f, %f, %f), (%f, %f, %f)]\n", lineCount, temp[lineCount][0][0], temp[lineCount][0][1], temp[lineCount][0][2], temp[lineCount][1][0], temp[lineCount][1][1], temp[lineCount][1][2]);
            lineCount++;
        }
        for(int x = 0; x < levels*2; x++){
            for(int y = 0; y < 2; y++){
                for(int z = 0; z < 3; z++){
                    island2d[x][y][z] = temp[x][y][z];
                }
            }
        }
    }
}

GLfloat getRandomHeightBetweenVertices(GLfloat height1, GLfloat height2, int i, int levels){
    GLfloat randomHeight;
    if(height1 > height2){
        GLfloat distanceBetween = height1 - height2;
        GLfloat padding = distanceBetween * 1/levels;
        randomHeight = randomInRange(height2 + padding, height1 - padding);
    }
    else{
        GLfloat distanceBetween = height2 - height1;
        GLfloat padding = distanceBetween * 1/levels;
        randomHeight = randomInRange(height1 + padding, height2 - padding);
    }
    return randomHeight;
}

GLfloat island1[1000][4][3];
GLfloat island2[1000][4][3];
GLfloat island3[1000][4][3];

void generateIsland3d(int islandNum, int smoothness){
    GLfloat island[1000][4][3];
    //    initial pyramid
    // polygon 1
    island[0][0][0] = -5.0f;
    island[0][0][1] = 0.0f;
    island[0][0][2] = 5.0f;
    
    island[0][1][0] = 0.0f;
    island[0][1][1] = 0.0f;
    island[0][1][2] = 5.0f;
    
    island[0][2][0] = 0.0f;
    island[0][2][1] = 5.0f;
    island[0][2][2] = 0.0f;
    
    island[0][3][0] = -5.0f;
    island[0][3][1] = 0.0f;
    island[0][3][2] = 0.0f;
    
    // polygon 2
    island[1][0][0] = 5.0f;
    island[1][0][1] = 0.0f;
    island[1][0][2] = 5.0f;
    
    island[1][1][0] = 5.0f;
    island[1][1][1] = 0.0f;
    island[1][1][2] = 0.0f;
    
    island[1][2][0] = 0.0f;
    island[1][2][1] = 5.0f;
    island[1][2][2] = 0.0f;
    
    island[1][3][0] = 0.0f;
    island[1][3][1] = 0.0f;
    island[1][3][2] = 5.0f;
    
    // polygon 3
    island[2][0][0] = 5.0f;
    island[2][0][1] = 0.0f;
    island[2][0][2] = -5.0f;
    
    island[2][1][0] = 0.0f;
    island[2][1][1] = 0.0f;
    island[2][1][2] = -5.0f;
    
    island[2][2][0] = 0.0f;
    island[2][2][1] = 5.0f;
    island[2][2][2] = 0.0f;
    
    island[2][3][0] = 5.0f;
    island[2][3][1] = 0.0f;
    island[2][3][2] = 0.0f;
    
    // polygon 4
    island[3][0][0] = -5.0f;
    island[3][0][1] = 0.0f;
    island[3][0][2] = -5.0f;
    
    island[3][1][0] = -5.0f;
    island[3][1][1] = 0.0f;
    island[3][1][2] = 0.0f;
    
    island[3][2][0] = 0.0f;
    island[3][2][1] = 5.0f;
    island[3][2][2] = 0.0f;
    
    island[3][3][0] = 0.0f;
    island[3][3][1] = 0.0f;
    island[3][3][2] = -5.0f;
    
    int levels = 1;
    
    //    i is equivalent to each level
    for (int i = 0; i < smoothness; i++) {
        levels *= 4;
        GLfloat temp[levels*4][4][3];
        int polyCount = 0;
//        printf("1/levels = 1/%d = %f\n", levels, 1/(float)levels);
        
        for (int j = 0; j < levels; j++) {
//            GLfloat randomHeight1 = getRandomHeightBetweenVertices(island2[j][2][1], island2[j][1][1], i, levels);
            
            GLfloat halfX = (island[j][0][0] + island[j][2][0])/2;
            GLfloat halfZ = (island[j][0][2] + island[j][2][2])/2;
            for(int k = 0; k < 4; k++){
                // POLYGON
                // vertex 1
                temp[polyCount][0][0] = island[j][k][0];
                temp[polyCount][0][1] = island[j][k][1];
                temp[polyCount][0][2] = island[j][k][2];
                // vertex 2
                temp[polyCount][1][0] = halfX;
                if(k == 3)
                    temp[polyCount][1][1] = getRandomHeightBetweenVertices(island[j][k][1], island[j][0][1], i, levels);
                else
                    temp[polyCount][1][1] = getRandomHeightBetweenVertices(island[j][k][1], island[j][k+1][1], i, levels);
                temp[polyCount][1][2] = island[j][k][2];
                // vertex 3
                temp[polyCount][2][0] = halfX;
                if(k == 3)
                    temp[polyCount][2][1] = getRandomHeightBetweenVertices(island[j][k][1], island[j][0][1], i, levels);
                else
                    temp[polyCount][2][1] = getRandomHeightBetweenVertices(island[j][k][1], island[j][k+1][1], i, levels);
                temp[polyCount][2][2] = halfZ;
                // vertex 4
                temp[polyCount][3][0] = island[j][k][0];
                if(k == 3)
                    temp[polyCount][3][1] = getRandomHeightBetweenVertices(island[j][k][1], island[j][0][1], i, levels);
                else
                    temp[polyCount][3][1] = getRandomHeightBetweenVertices(island[j][k][1], island[j][k+1][1], i, levels);
                temp[polyCount][3][2] = halfZ;
//                printf("temp[%d] = [(%f, %f, %f), (%f, %f, %f), (%f, %f, %f), (%f, %f, %f)]\n", polyCount, temp[polyCount][0][0], temp[polyCount][0][1], temp[polyCount][0][2], temp[polyCount][1][0], temp[polyCount][1][1], temp[polyCount][1][2], temp[polyCount][2][0], temp[polyCount][2][1], temp[polyCount][2][2], temp[polyCount][3][0], temp[polyCount][3][1], temp[polyCount][3][2]);
                
                polyCount++;
            }
        }
        for(int x = 0; x < levels*4; x++){
            for(int y = 0; y < 4; y++){
                for(int z = 0; z < 3; z++){
                    island[x][y][z] = temp[x][y][z];
                    printf("%f\n", island[x][y][z]);
                }
            }
        }
    }
    if(islandNum == 0){
        printf("HELLO FROM 1\n");
        for(int x = 0; x < levels*4; x++){
            for(int y = 0; y < 4; y++){
                for(int z = 0; z < 3; z++){
                    island1[x][y][z] = island[x][y][z];
                    printf("%f\n", island1[x][y][z]);
                }
            }
        }
    }
    else if (islandNum == 1){
        for(int x = 0; x < levels*4; x++){
            for(int y = 0; y < 4; y++){
                for(int z = 0; z < 3; z++){
                    island2[x][y][z] = island[x][y][z];
                }
            }
        }
    }
    else{
        for(int x = 0; x < levels*4; x++){
            for(int y = 0; y < 4; y++){
                for(int z = 0; z < 3; z++){
                    island3[x][y][z] = island[x][y][z];
                }
            }
        }
    }
}

void drawIsland2d(){
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zeroMaterial);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 0);
    glBegin(GL_LINES);
    for(int i=0; i<100; i++){
        glVertex3fv(island1[i][0]);
        glVertex3fv(island1[i][1]);
    }
    glEnd();
}

void drawIsland1(bool wireFrameGrid){
    if(wireFrameGrid)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zeroMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 5);
    glBegin(GL_POLYGON);
    for(int i=0; i<100; i++){
        glVertex3fv(island1[i][0]);
        glVertex3fv(island1[i][1]);
        glVertex3fv(island1[i][2]);
        glVertex3fv(island1[i][3]);
    }
    glEnd();
}
void drawIsland2(bool wireFrameGrid){
    if(wireFrameGrid)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zeroMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 5);
    glBegin(GL_POLYGON);
    for(int i=0; i<100; i++){
        glVertex3fv(island2[i][0]);
        glVertex3fv(island2[i][1]);
        glVertex3fv(island2[i][2]);
        glVertex3fv(island2[i][3]);
    }
    glEnd();
}
void drawIsland3(bool wireFrameGrid){
    if(wireFrameGrid)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zeroMaterial);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 5);
    glBegin(GL_POLYGON);
    for(int i=0; i<100; i++){
        glVertex3fv(island3[i][0]);
        glVertex3fv(island3[i][1]);
        glVertex3fv(island3[i][2]);
        glVertex3fv(island3[i][3]);
    }
    glEnd();
}

void generateIslands(void){
    generateIsland3d(0, 1);
    generateIsland3d(1, 1);
    generateIsland3d(2, 1);
}

void loadScene(void){
//    island = generateIsland();
    diskImageTexture = loadImage("sea02.ppm", true);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(2, &diskTextureId);
    glBindTexture(GL_TEXTURE_2D, diskTextureId);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, diskImageTextureWidth, diskImageTextureHeight, GL_RGB, GL_UNSIGNED_BYTE, diskImageTexture);
    // tell openGL how to scale the texture image up if needed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // tell openGL how to scale the texture image down if needed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glDisable(GL_TEXTURE_2D);
    cylinderImageTexture = loadImage("sky08.ppm", false);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(2, &cylinderTextureId);
    glBindTexture(GL_TEXTURE_2D, cylinderTextureId);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, cylinderImageTextureWidth, cylinderImageTextureHeight, GL_RGB, GL_UNSIGNED_BYTE, cylinderImageTexture);
    // tell openGL how to scale the texture image up if needed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // tell openGL how to scale the texture image down if needed
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glDisable(GL_TEXTURE_2D);
}
