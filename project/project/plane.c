//
//  plane.c
//  project
//
//  Created by Isaac Dunnett on 2021-12-02.
//

#include "plane.h"

const int numPlaneVertices = 6763;
GLfloat planeVertices[numPlaneVertices][3];
GLfloat planeNormals[numPlaneVertices][3];
GLint planeSubObjects[33][200][18];

const int numPropellerVertices = 6763;
GLfloat propellerVertices[numPropellerVertices][3];
GLfloat propellerNormals[numPropellerVertices][3];
int propellerSubObjects[2][200][18];

extern GLfloat blue[], yellow[], black[], purple[], white[], red[], zeroMaterial[];
GLfloat planeColor1[] = {0, 0, 1, 1}, planeColor2[] = {1, 1, 0, 1}, planeColor3[] = {0.5, 0, 1, 1};

void readInPropeller(){
    FILE *file;
    if((file = fopen("propeller.txt", "r")) == NULL){
        printf("Error opening propeller.txt\n");
        return;
    }
    char line[256];
    int verticesCount = 0;
    int normalsCount = 0;
    int subObjectCount = 0;
    int faceCount = 0;
    while(fgets(line, sizeof(line), file)){
        char indicator = line[0];
        if(indicator == 'v'){
            GLfloat p1, p2, p3;
            sscanf(line, "%*s %f %f %f", &p1, &p2, &p3);
            propellerVertices[verticesCount][0] = p1;
            propellerVertices[verticesCount][1] = p2;
            propellerVertices[verticesCount][2] = p3;
            verticesCount++;
        }
        else if(indicator == 'n') {
            GLfloat p1, p2, p3;
            sscanf(line, "%*s %f %f %f", &p1, &p2, &p3);
            propellerNormals[normalsCount][0] = p1;
            propellerNormals[normalsCount][1] = p2;
            propellerNormals[normalsCount][2] = p3;
            normalsCount++;
        }
        else if(indicator == 'g') {
            subObjectCount++;
            faceCount = 0;
        }
        else if(indicator == 'f') {
            int tokenCount = 0;
            char* token = strtok(line, " ");
            while(token != NULL){
                if(tokenCount > 0){
                    if(strcmp(token, "\n")){
                        propellerSubObjects[subObjectCount-1][faceCount][tokenCount-1] = atoi(token);
                    }
                }
                token = strtok(NULL, " ");
                tokenCount++;
            }
            faceCount++;
        }
    }
    printf("Total propeller vertices: %d\n", verticesCount);
    printf("Total propeller normals: %d\n", normalsCount);
    printf("propeller subObjectCount: %d\n", subObjectCount);
}

void readInPlane(void){
    FILE *file;
    if((file = fopen("cessna.txt", "r")) == NULL){
        printf("Error opening cessna.txt\n");
        return;
    }
    char line[256];
    int verticesCount = 0;
    int normalsCount = 0;
    int subObjectCount = 0;
    int faceCount = 0;
    while(fgets(line, sizeof(line), file)){
        char indicator = line[0];
        if(indicator == 'v'){
            GLfloat p1, p2, p3;
            sscanf(line, "%*s %f %f %f", &p1, &p2, &p3);
            planeVertices[verticesCount][0] = p1;
            planeVertices[verticesCount][1] = p2;
            planeVertices[verticesCount][2] = p3;
            verticesCount++;
        }
        else if(indicator == 'n') {
            GLfloat p1, p2, p3;
            sscanf(line, "%*s %f %f %f", &p1, &p2, &p3);
            planeNormals[normalsCount][0] = p1;
            planeNormals[normalsCount][1] = p2;
            planeNormals[normalsCount][2] = p3;
            normalsCount++;
        }
        else if(indicator == 'g') {
            subObjectCount++;
            faceCount = 0;
        }
        else if(indicator == 'f') {
            int tokenCount = 0;
            char* token = strtok(line, " ");
            while(token != NULL){
                if(tokenCount > 0){
                    if(strcmp(token, "\n")){
                        planeSubObjects[subObjectCount-1][faceCount][tokenCount-1] = atoi(token);
                    }
                }
                token = strtok(NULL, " ");
                tokenCount++;
            }
            faceCount++;
        }
    }
    printf("Total plane vertices: %d\n", verticesCount);
    printf("Total plane normals: %d\n", normalsCount);
    printf("plane subObjectCount: %d\n", subObjectCount);
}

void loadPlane(void){
    readInPlane();
    readInPropeller();
}

void drawPropeller(void){
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zeroMaterial);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 1);
    for(int i = 0; i<2; i++){
        if(i == 0){
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  white);
        } else {
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  red);
        }
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   zeroMaterial);
        for(int j=0; j<120; j++){
            glBegin(GL_POLYGON);
            for(int k=0; k<14; k++){
                if(propellerSubObjects[i][j][k] == 0)
                    break;
                glNormal3fv(propellerNormals[propellerSubObjects[i][j][k]-1]);
                glVertex3fv(propellerVertices[propellerSubObjects[i][j][k]-1]);
            }
            glEnd();
        }
    }
}

void randomPlaneColors(void){
    for(int i =0; i < 3; i++){
        planeColor1[i] = (float)rand()/(float)RAND_MAX;
    }
    for(int i =0; i < 3; i++){
        planeColor2[i] = (float)rand()/(float)RAND_MAX;
    }
    for(int i =0; i < 3; i++){
        planeColor3[i] = (float)rand()/(float)RAND_MAX;
    }
}

void drawPlane(void){
    glMaterialfv(GL_FRONT, GL_DIFFUSE, zeroMaterial);
    glMaterialf(GL_FRONT,  GL_SHININESS, 5);
    for(int i = 0; i<33; i++){
        if(i < 4){
            glMaterialfv(GL_FRONT, GL_DIFFUSE, planeColor2);
            glMaterialfv(GL_FRONT, GL_SPECULAR,  planeColor2);
        }
        else if(i < 6){
            glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
            glMaterialfv(GL_FRONT, GL_SPECULAR,  black);
            glMaterialf(GL_FRONT,  GL_SHININESS, 10);
        }
        else if(i < 7){
            glMaterialfv(GL_FRONT, GL_DIFFUSE, planeColor3);
            glMaterialfv(GL_FRONT, GL_SPECULAR,  planeColor3);
        }
        else if(i < 8){
            glMaterialfv(GL_FRONT, GL_DIFFUSE, planeColor1);
            glMaterialfv(GL_FRONT, GL_SPECULAR,  planeColor1);
        }
        else if(i < 14){
            glMaterialfv(GL_FRONT, GL_DIFFUSE, planeColor2);
            glMaterialfv(GL_FRONT, GL_SPECULAR,  planeColor2);
        }
        else if(i < 26) {
            glMaterialfv(GL_FRONT, GL_DIFFUSE, planeColor1);
            glMaterialfv(GL_FRONT, GL_SPECULAR,  planeColor1);
        }
        else if(i < 33) {
            glMaterialfv(GL_FRONT, GL_DIFFUSE, planeColor2);
            glMaterialfv(GL_FRONT, GL_SPECULAR,  planeColor2);
        }
        glMaterialfv(GL_FRONT, GL_AMBIENT,   zeroMaterial);
        for(int j=0; j<200; j++){
            glBegin(GL_POLYGON);
            for(int k=0; k<18; k++){
                if(planeSubObjects[i][j][k] == 0)
                    break;
//                printf("planeVertices[%d]: (%f, %f, %f)\n", planeSubObjects[i][j][k], planeVertices[planeSubObjects[i][j][k]-1][0], planeVertices[planeSubObjects[i][j][k]-1][1], planeVertices[planeSubObjects[i][j][k]-1][2]);
                glNormal3fv(planeNormals[planeSubObjects[i][j][k]-1]);
                glVertex3fv(planeVertices[planeSubObjects[i][j][k]-1]);
            }
            glEnd();
        }
    }
}

char* intToChars(int n)
{
    int m = n;
    int digit = 0;
    while (m) {
        
        // Increment number of digits
        digit++;
        
        // Truncate the last
        // digit from the number
        m /= 10;
    }
    
    // Declare char array for result
    char* arr;
    
    // Declare duplicate char array
    char arr1[digit];
    
    // Memory allocation of array
    arr = (char*)malloc(digit);
    
    // Separating integer into digits and
    // accommodate it to character array
    int index = 0;
    while (n) {
        
        // Separate last digit from
        // the number and add ASCII
        // value of character '0' is 48
        arr1[++index] = n % 10 + '0';
        
        // Truncate the last
        // digit from the number
        n /= 10;
    }
    
    // Reverse the array for result
    int i;
    for (i = 0; i < index; i++) {
        arr[i] = arr1[index - i];
    }
    
    // Char array truncate by null
    arr[i] = '\0';
    
    // Return char array
    return (char*)arr;
}

void displayPlaneSpeed(GLfloat speed){
    char text[7] = {'S', 'P', 'E', 'E', 'D', ':', ' '};
    for (int i = 0; i < 7; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
    
    char* charSpeed = intToChars((int)(speed * 10));
    unsigned long charSpeedLen = strlen(charSpeed);
    if(charSpeedLen == 0)
        return glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0');
    for(int j = 0; j < charSpeedLen; j++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, charSpeed[j]);
    }
}

void displayPlaneAltitude(GLfloat altitude){
    char text[10] = {'A', 'L', 'T', 'I', 'T', 'U', 'D', 'E', ':', ' '};
    for (int i = 0; i < 10; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
    
    char* charAltitude = intToChars((int)altitude);
    unsigned long charAltitudeLen = strlen(charAltitude);
    if(altitude < 0 || charAltitudeLen == 0)
        return glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0');
    for(int j = 0; j < charAltitudeLen; j++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, charAltitude[j]);
    }
}
