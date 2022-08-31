/*---------------------------------------------------------------------------------
 * This file contains a program, drawing & animating a wind-mill.
 * This program is used to teach LCS and scene graph concepts.
 *
 * by S. K. Ueng, Nov.  2006
 */
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
using namespace std;
#define  PI   3.141592653
#define  Step  0.5
/*-----Define a unit box--------*/
/* Vertices of the box */
#define TSIZE 64 /* define texture dimension */
float hx[13] = { 0,-1.0135,-2.4335,-3.4015,-3.1595,-2.06,0,1.94,2.78,3.2,2.34,0.91,0 };
float hy[13] = { 2,3.1336,3.3016,2.1136,1.0136,0,-1.6044,0,0.9696,2.0476,3.2796,3.1036,2 };
float ds = 0.0, dt = 0.0;
float  points[][3] = { {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
                      {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5} };
float mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
float hpoints[13][3];
float fog_des = 0.1;
float black_plastic[3][4] = { {0,0,0,1},{0.01,0.01,0.01,1},{0.5,0.5,0.5,1} };
float light_color[4][4] = { {1,1,1,1},{1,0,0,1},{0,1,0,1},{0,0,0,1} };
unsigned char checkboard[TSIZE][TSIZE][4]; /* checkboard textures */
unsigned char checkboard2[TSIZE][TSIZE][4]; /* checkboard textures */
unsigned char brick[TSIZE][TSIZE][4];      /* brick wall textures */
unsigned char* skydome;
unsigned int widt = 612, heigt = 408;
int ch_color;
float light_str = 1;
float   eyeDx, eyeDy, eyeDz;
float   eyeAngx, eyeAngy, eyeAngz;
double  Eye[3] = { 0.0, 0.0, 30.0 }, Focus[3] = { 0.0, 0.0, 0.0 }, Vup[3] = { 0.0, 1.0, 0.0 };
float   u[3][3] = { {1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0} };
float   eye[3];
float   cv, sv, sh; /* cos(5.0) and sin(5.0) */
/* face of box, each face composing of 4 vertices */
int    face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };
int light_enable = 1, light_range = 0, light_spot = 0, light_pos = 0, light_exp = 0;
/* indices of the box faces */
int    cube[6] = { 0, 1, 2, 3, 4, 5 };
float   lt_pos[] = { 20.0, 20.0, 5.0, 0.0 };
float ltp_s = 20.0;
float   lt_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
float   lt_specular[] = { 0.9, 0.9, 0.9, 1.0 };
float   global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
float cube_angle = 0.0;
float skyAng = 0.0;
int cb, dc, cr;
int style, fog_state;
/*----Define material properties for cube -----*/
float lit_angle, shinny = 32.0;
/*----Define light source properties -------*/
float  lit_position[] = { 10.0, 14.0, 0.0, 1.0 };
float  lit_direction[] = { -1.0, -1.0, 1.0, 0.0 };
/*----Define normals of faces ----*/
float  normal[][4] = { {0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0},
              {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0} };
float a[3], b[3];
float mtx[16]; /* the modelview matrix */
/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj* sphere = NULL, * cylind = NULL;

/*-----Define window size----*/
int width = 600, height = 600;
//Global variables recording rotation angles of H-arm and blades.
float  arm_ang = 45.0, bld_ang = 10.0;
float   auto_rotate, autox, autoy, autoz, Rotate, fx, fy, fz;
int stp;

int select = 0;
unsigned int TextureID[10];
unsigned char circle[64][64][4];
unsigned char waterwave[64][64][4];
unsigned char star[64][64][4];
unsigned char land[64][64][4];
/*----- Parameters of cube -----*/
float vertex[][3] = { { -0.5, -0.5, -0.5 }, { 0.5, -0.5, -0.5 }, { 0.5, 0.5, -0.5 }, { -0.5, 0.5, -0.5 },
                      { -0.5, -0.5, 0.5 },  { 0.5, -0.5, 0.5 },  { 0.5, 0.5, 0.5 },  { -0.5, 0.5, 0.5 } };
double color[6][3] = { { 0.55, 0.55, 0.25 }, { 0.65, 0.55, 1.0 }, { 0.75, 0.55, 0.55 },
                       { 0.55, 0.55, 0.25 }, { 0.7, 0.6, 0.7 },   { 0.55, 0.75, 0.75 } };
/*----- Parameters of light -----*/
float material_shininess = 32.0;
float light_position[] = { 0.0, 15.0, 5.0, 1.0 };
float light_direction[] = { 0.0, -1.0, 0.0, 0.0 };
float light_diffuse[] = { 1.0, 1.0, 0.5, 1.0 };
float light_specular[] = { 0.5, 0.5, 0.5, 1.0 };
float light_cutoff = 50.0;
double light_attenuation = 0.8;
/*---Define fog color----*/
float fog_color[] = { 0.15, 0.20, 0.20, 0.50 };
int fc = 0;

/*----------------------------------------------------------
 * Procedure to make a checkboard texture.
 */
void make_check() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            checkboard[i][j][0] = checkboard[i][j][1] = checkboard[i][j][2] = c;
            checkboard[i][j][3] = 255;
        }
}


/*----------------------------------------------------------
 * Procedure to make a checkboard texture.
 */
void make_check2() {
    int i, j, c;

    for (i = 0; i < TSIZE; i++)
        for (j = 0; j < TSIZE; j++) {
            if (j > i / 2 && j <= 64 - i / 2)
                c = 255;
            else
                c = 0;
            checkboard2[i][j][0] = c / 8;
            checkboard2[i][j][1] = c / 2;
            checkboard2[i][j][2] = c / 4;
            if (c == 255)
                checkboard2[i][j][3] = 255;
            else
                checkboard2[i][j][3] = 0;
        }
    // Generate trunk
    for (i = 0; i < TSIZE / 3; i++) {
        for (j = 0; j < TSIZE / 2 - 4; j++)
            checkboard2[i][j][3] = 0;
        for (j = TSIZE / 2 + 4; j < TSIZE; j++)
            checkboard2[i][j][3] = 0;
    }
}
/*----------------------------------------------------------
 * Procedure to maKe a brick wall texture map
 */
void brick_pattern() {
    int y, x, i;

    for (y = 0; y < 12; y++) { /*----Create the first row of bricks----*/
        for (x = 0; x < 2; x++) {
            brick[y][x][0] = brick[y][x][1] = brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 2; x < 30; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 30; x < 34; x++) {
            brick[y][x][0] = brick[y][x][1] = brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 34; x < 62; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 62; x < 64; x++) {
            brick[y][x][0] = brick[y][x][1] = brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }
    }

    for (y = 12; y < 16; y++) /*--Create the cement between two breick layers */
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = brick[y][x][1] = brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 16; y < 28; y++) { /*--Create the 2nd layer of bricks---*/
        for (x = 0; x < 14; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 14; x < 18; x++) {
            brick[y][x][0] = brick[y][x][1] = brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 18; x < 46; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }

        for (x = 46; x < 50; x++) {
            brick[y][x][0] = brick[y][x][1] = brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

        for (x = 50; x < 64; x++) {
            brick[y][x][0] = 200;
            brick[y][x][1] = brick[y][x][2] = 0;
            brick[y][x][3] = 255;
        }
    }

    for (y = 28; y < 32; y++) /*---Add another layer of cement----*/
        for (x = 0; x < 64; x++) {
            brick[y][x][0] = brick[y][x][1] = brick[y][x][2] = 140;
            brick[y][x][3] = 255;
        }

    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                brick[y][x][i] = brick[y - 32][x][i];
}

void Create_Texture_Circle() {
    int y, x, i, temp;
    for (y = 0; y < 64; y++) {
        for (x = 0; x < 64; x++) {
            circle[y][x][0] = circle[y][x][1] = circle[y][x][2] = 100;
            circle[y][x][3] = 255;
        }
    }
    for (y = 16, x = 0; y < 48; y++) {
        if (y <= 20 && y >= 17)
            x = x + 2;
        else if (y == 22 || y == 25 || y == 31)
            x = x + 1;
        else if (y == 42 || y == 39 || y == 33)
            x = x - 1;
        else if (y <= 47 && y >= 44)
            x = x - 2;
        temp = x;
        for (x = 0; x < temp; x++) {
            circle[y][x][0] = circle[y][x][1] = circle[y][x][2] = 150;
            circle[y][x][3] = 255;
        }
        for (x = 64 - temp; x < 64; x++) {
            circle[y][x][0] = circle[y][x][1] = circle[y][x][2] = 150;
            circle[y][x][3] = 255;
        }
        x = temp;
    }

    for (x = 16, y = 0; x < 48; x++) {
        if (x <= 20 && x >= 17)
            y = y + 2;
        else if (x == 22 || x == 25 || x == 31)
            y = y + 1;
        else if (x == 42 || x == 39 || x == 33)
            y = y - 1;
        else if (x <= 47 && x >= 44)
            y = y - 2;
        temp = y;
        for (y = 0; y < temp; y++) {
            circle[y][x][0] = circle[y][x][1] = circle[y][x][2] = 200;
            circle[y][x][3] = 255;
        }
        for (y = 64 - temp; y < 64; y++) {
            circle[y][x][0] = circle[y][x][1] = circle[y][x][2] = 200;
            circle[y][x][3] = 255;
        }
        y = temp;
    }
    for (i = 0; i < 61; i++)
        for (y = i; y < 4 + i; y++)
            for (x = i; x < 4 + i; x++)
                for (int k = 0; k < 4; ++k) {
                    circle[y][x][k] = 255;
                }
    for (i = 0; i < 61; i++)
        for (y = i; y < 4 + i; y++)
            for (x = 60 - i; x < 64 - i; x++)
                for (int k = 0; k < 4; ++k) {
                    circle[y][x][k] = 255;
                }
}

void Create_Texture_Land() {
    int x, y, i;

    for (y = 0; y < 32; y++)
        for (x = 0; x < 32; x++) {
            land[x][y][0] = land[x][y][1] = land[x][y][2] = 240;
            land[x][y][3] = 255;
        }

    for (x = 24; x < 32; x++) {
        land[x][0][1] = land[x][0][2] = land[0][x][1] = land[0][x][2] = 200;
        land[x][0][0] = land[x][0][3] = land[0][x][0] = land[0][x][3] = 255;
    }
    for (x = 26; x < 32; x++) {
        land[x][1][0] = land[x][1][3] = land[1][x][0] = land[1][x][3] = 255;
        land[x][1][1] = land[x][1][2] = land[1][x][1] = land[1][x][2] = 200;
    }
    for (x = 27; x < 32; x++) {
        for (y = 2; y < 4; y++) {
            land[x][y][0] = land[x][y][3] = land[y][x][0] = land[y][x][3] = 255;
            land[x][y][1] = land[x][y][2] = land[y][x][1] = land[y][x][2] = 200;
        }
    }
    for (x = 28; x < 32; x++) {
        for (y = 4; y < 7; y++) {
            land[x][y][0] = land[x][y][3] = land[y][x][0] = land[y][x][3] = 255;
            land[x][y][1] = land[x][y][2] = land[y][x][1] = land[y][x][2] = 200;
        }
    }
    for (x = 29; x < 32; x++) {
        for (y = 7; y < 9; y++) {
            land[x][y][0] = land[x][y][3] = land[y][x][0] = land[y][x][3] = 255;
            land[x][y][1] = land[x][y][2] = land[y][x][1] = land[y][x][2] = 200;
        }
    }
    for (x = 30; x < 32; x++) {
        for (y = 9; y < 12; y++) {
            land[x][y][0] = land[x][y][3] = land[y][x][0] = land[y][x][3] = 255;
            land[x][y][1] = land[x][y][2] = land[y][x][1] = land[y][x][2] = 200;
        }
    }
    for (x = 31; x < 32; x++) {
        for (y = 12; y < 16; y++) {
            land[x][y][0] = land[x][y][3] = land[y][x][0] = land[y][x][3] = 255;
            land[x][y][1] = land[x][y][2] = land[y][x][1] = land[y][x][2] = 200;
        }
    }

    for (y = 16; y < 32; y++)
        for (x = 16; x < 32; x++) {
            land[x][y][0] = 200;
            land[x][y][1] = 160;
            land[x][y][2] = land[x][y][3] = 255;

        }
    for (y = 20; y < 32; y++)
        for (x = 20; x < 32; x++) {
            land[x][y][0] = land[x][y][2] = land[x][y][3] = 255;
            land[x][y][1] = 200;
        }

    for (x = 63; x > 31; x--) {
        for (y = 0; y < 32; y++)
            for (i = 0; i < 4; i++)
                land[x][y][i] = land[63 - x][y][i];
    }

    /* Create the rest half part */
    for (y = 32; y < 64; y++)
        for (x = 0; x < 64; x++)
            for (i = 0; i < 4; i++)
                land[x][y][i] = land[x][63 - y][i];
}

/*--------------------------------------------------------
 * Procedure to draw a billboard, center=[x,z], width=w,
 * height = h;
 */
void draw_billboard(float x, float z, float w, float h) {
    float v0[3], v1[3], v2[3], v3[3];

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    /*----Compute the 4 vertices of the billboard ----*/
    v0[0] = x - (w / 2) * a[0];
    v0[1] = 0.0;
    v0[2] = z - (w / 2) * a[2];
    v1[0] = x + (w / 2) * a[0];
    v1[1] = 0.0;
    v1[2] = z + (w / 2) * a[2];
    v2[0] = x + (w / 2) * a[0];
    v2[1] = h;
    v2[2] = z + (w / 2) * a[2];
    v3[0] = x - (w / 2) * a[0];
    v3[1] = h;
    v3[2] = z - (w / 2) * a[2];

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(v0);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(v1);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(v2);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(v3);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void draw_cylinder() {
    static GLUquadricObj* cylind = NULL;

    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }

    gluQuadricTexture(cylind, GL_TRUE);
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(cylind, 1.0, 1.0, 8.0, 12, 3);
    glPopMatrix();
}

void Create_Texture_Star() {
    int x, y, i;

    for (x = 0; x < 64; x++)
        for (y = 0; y < 64; y++)
            for (i = 0; i < 4; ++i) {
                star[x][y][i] = 255;
            }

    x = 31;
    y = 60;
    while (x >= 12) {
        for (i = y; i > 4; i--) {
            star[x][i][0] = star[x][i][1] = 100;
            star[x][i][2] = star[x][i][3] = 255;
        }
        y -= 3;
        x--;
    }
    x = 31;
    y = 20;
    while (x >= 12) {
        for (i = y; i > 4; i--) {
            for (int j = 0; j < 4; ++j) {
                star[x][i][j] = 255;
            }
        }
        y -= 1;
        x--;
    }
    x = 4;
    y = 40;
    while (y >= 18) {
        for (i = x; i < 32; i++) {
            star[i][y][0] = star[i][y][1] = 100;
            star[i][y][2] = star[i][y][3] = 255;
        }
        y -= 1;
        if ((y % 2) == 1)
            x += 2;
        else
            x++;
    }

    for (x = 63; x > 31; x--)
        for (y = 0; y < 64; y++)
            for (i = 0; i < 4; i++)
                star[x][y][i] = star[63 - x][y][i];
}

/*--------------------------------------------------------
 * Procedure to draw the cube, whose vertices and faces
 * are defined above.
 */
void draw_cut_cube() {
    int i;
    float range;

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glTranslatef(ds, dt, 0.0);
    glMatrixMode(GL_MODELVIEW);

    for (i = 0; i < 6; i++) { /* draw the six faces one by one */
        range = 1.0;
        glNormal3fv(normal[i]);
        glBegin(GL_POLYGON); /* Draw the face */
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(points[face[i][0]]);
        glTexCoord2f(0.0, range);
        glVertex3fv(points[face[i][1]]);
        glTexCoord2f(range, range);
        glVertex3fv(points[face[i][2]]);
        glTexCoord2f(range, 0.0);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}

unsigned char* loadBMPRaw(const char* imagepath, unsigned int& outWidth, unsigned int& outHeight, bool flipY) {
    printf("Reading image %s\n", imagepath);
    outWidth = -1;
    outHeight = -1;
    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    // Actual RGB data
    unsigned char* data;
    // Open the file
    FILE* file = fopen(imagepath, "rb");
    if (!file) { printf("Image could not be opened\n"); return NULL; }
    // Read the header, i.e. the 54 first bytes
    // If less than 54 byes are read, problem
    if (fread(header, 1, 54, file) != 54) {
        printf("Not a correct BMP file\n");
        return NULL;
    }
    // A BMP files always begins with "BM"
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file\n");
        return NULL;
    }
    // Make sure this is a 24bpp file
    if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return NULL; }
    if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return NULL; }

    printf("Read successfully!\n");

    // Read the information about the image
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    outWidth = *(int*)&(header[0x12]);
    outHeight = *(int*)&(header[0x16]);
    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0)    imageSize = outWidth * outHeight * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos == 0)      dataPos = 54; // The BMP header is done that way
    // Create a buffer
    data = new unsigned char[imageSize];
    // Read the actual data from the file into the buffer
    fread(data, 1, imageSize, file);
    // Everything is in memory now, the file wan be closed
    fclose(file);
    if (flipY) {
        // swap y-axis
        unsigned char* tmpBuffer = new unsigned char[outWidth * 3];
        int size = outWidth * 3;
        for (int i = 0; i < outHeight / 2; i++) {
            // copy row i to tmp
            memcpy_s(tmpBuffer, size, data + outWidth * 3 * i, size);
            // copy row h-i-1 to i
            memcpy_s(data + outWidth * 3 * i, size, data + outWidth * 3 * (outHeight - i - 1), size);
            // copy tmp to row h-i-1
            memcpy_s(data + outWidth * 3 * (outHeight - i - 1), size, tmpBuffer, size);
        }
        delete[] tmpBuffer;
    }

    return data;
}

void Draw_UnitCube(int index) {
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[index]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    for (int i = 0; i < 6; i++) {
        glNormal3fv(normal[i]);
        glColor3dv(color[i]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 0.0);
        glVertex3fv(vertex[face[i][0]]);
        glTexCoord2f(0.0, 8.0);
        glVertex3fv(vertex[face[i][1]]);
        glTexCoord2f(8.0, 8.0);
        glVertex3fv(vertex[face[i][2]]);
        glTexCoord2f(8.0, 0.0);
        glVertex3fv(vertex[face[i][3]]);
        glEnd();
    }
}

void setup_light()
{
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lt_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[ch_color]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lt_specular);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}
/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(10, TextureID);

    Create_Texture_Circle();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, circle);

    Create_Texture_Star();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, star);

    Create_Texture_Land();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, land);

    /*-----Create Texture -----*/
    make_check();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[3]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkboard);

    /*-----Create another texture ----*/
    brick_pattern();
    glBindTexture(GL_TEXTURE_2D, TextureID[4]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, brick);

    /*-----Create Texture -----*/
    make_check2();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glBindTexture(GL_TEXTURE_2D, TextureID[5]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkboard2);

    skydome = loadBMPRaw("sky.bmp", widt, heigt, true);
    glBindTexture(GL_TEXTURE_2D, TextureID[6]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widt, heigt, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, skydome);
    delete[] skydome;



    /*----Set up fog conditions ---*/
    glEnable(GL_FOG);                /*enable fog fade */
    glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
    glFogf(GL_FOG_DENSITY, fog_des);    /*fog opacity(density)= 0.25*/
    glFogf(GL_FOG_START, 1.0);       /*Setup two ends for GL_LINEAR*/
    glFogf(GL_FOG_END, 36.0);
    glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */


    glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
    /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 499, 499);

    /*-----Set a parallel projection mode-----*/
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-15.0, 15.0, -15.0, 15.0, -10.5, 30.0);
    cv = cos(5.0 * PI / 180.0);
    sv = sin(5.0 * PI / 180.0);
    glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
    glEnable(GL_LIGHTING);
    setup_light();
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    /*---Enable normalization of normals----*/
    glEnable(GL_NORMALIZE);
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];
    glFlush();/*Enforce window system display the results*/
}

void draw_axes()
{
    /*----Draw a white sphere to represent the original-----*/
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0.9, 0.9, 0.9);
    gluSphere(sphere, 2.0,   /* radius=2.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 8 stacks */

    /*----Draw three axes in colors, yellow, meginta, and cyan--*/
    /* Draw Z axis  */
    glColor3f(0.0, 0.95, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */

    /* Draw Y axis */
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
    glColor3f(0.95, 0.0, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    glColor3f(0.95, 0.95, 0.0);
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
    gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();
    /*-- Restore the original modelview matrix --*/
    glPopMatrix();
}

/*-------------------------------------------------------
 * Make viewing matrix
 */
void make_view(int x)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    switch (x) {
    case 4:       /* Perspective */
        /* In this sample program, eye position and Xe, Ye, Ze are computed
           by ourselves. Therefore, use them directly; no transform is
           applied upon eye coordinate system
           */
           //the place of camera; the place of watching_point; the proportion of head degree 
        gluLookAt(eye[0], eye[1], eye[2], eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2], u[1][0], u[1][1], u[1][2]);
        break;

    case 1:       /* X direction parallel viewing */
        gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    case 2:       /* Y direction parallel viewing */
        gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
        break;
    case 3:
        gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
        break;
    }
}
/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int x)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (x == 4) {
        gluPerspective(90.0, (double)width / (double)height, 5, 100.0);
    }
    else {
        if (width > height)
            glOrtho(-40.0, 40.0, -40.0 * (float)height / (float)width, 40.0 * (float)height / (float)width, -0.0, 100.0);
        else
            glOrtho(-40.0 * (float)width / (float)height, 40.0 * (float)width / (float)height, -40.0, 40.0, -0.0, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void draw_view_volume() {
    float w[4][2] = { {1,1},{-1,1},{-1,-1},{1,-1} }, view_volume[8][3] = { {5 * width / height,5,-5} };
    view_volume[4][0] = 50 * width / height;
    view_volume[4][1] = 50;
    view_volume[4][2] = -50;
    for (int i = 0; i < 4; ++i) {
        view_volume[i][0] = view_volume[0][0] * w[i][0];
        view_volume[i][1] = view_volume[0][1] * w[i][1];
        view_volume[i][2] = view_volume[0][2];
        view_volume[i + 4][0] = view_volume[4][0] * w[i][0];
        view_volume[i + 4][1] = view_volume[4][1] * w[i][1];
        view_volume[i + 4][2] = view_volume[4][2];
    }
    if (cb == 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glColor3f(0.5, 0.5, 0.5);  /* Set the current color */
        for (int i = 0; i < 6; i++) {
            glBegin(GL_POLYGON);  /* Draw the face */
            glVertex3fv(view_volume[face[i][0]]);
            glVertex3fv(view_volume[face[i][1]]);
            glVertex3fv(view_volume[face[i][2]]);
            glVertex3fv(view_volume[face[i][3]]);
            glEnd();
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
/*---------------------------------------------------------
 * Procedure to draw view volume, eye position, focus ,...
 * for perspective projection
 */
void draw_view()
{
    int    i;
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMatrixMode(GL_MODELVIEW);

    /*----Draw Eye position-----*/
    glPushMatrix();
    glTranslatef(eye[0], eye[1], eye[2]);
    glColor3f(0.0, 1.0, 0.0);
    glutWireSphere(1.0, 10, 10);
    glRotated(eyeAngx, -1, 0, 0);
    glRotated(eyeAngy, 0, 1, 0);
    glRotated(eyeAngz, 0, 0, -1);
    draw_view_volume();
    glPopMatrix();

    /*----Draw eye coord. axes -----*/
    glColor3f(1.0, 1.0, 0.0);           /* Draw Xe */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[0][0], eye[1] + 20.0 * u[0][1], eye[2] + 20.0 * u[0][2]);
    glEnd();

    glColor3f(1.0, 0.0, 1.0);          /* Draw Ye */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[1][0], eye[1] + 20.0 * u[1][1], eye[2] + 20.0 * u[1][2]);
    glEnd();

    glColor3f(0.0, 1.0, 1.0);          /* Draw Ze */
    glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0] + 20.0 * u[2][0], eye[1] + 20.0 * u[2][1], eye[2] + 20.0 * u[2][2]);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
}
/*---------------------------------------------------------
 * Procedure to draw the floor.
 */
void draw_floor()
{
    int  i, j;
    glEnable(GL_COLOR_MATERIAL);
    glMaterialf(GL_FRONT, GL_SHININESS, 6.0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    for (i = 0; i < 40; i++)
        for (j = 0; j < 40; j++) {
            if ((i + j) % 2 == 0) glColor3f(0.8, 0.8, 0.8);
            else glColor3f(0.7, 0.1, 0.1);
            glBegin(GL_POLYGON);
            glVertex3f((i - 5.0) * 5.0, -2.5, (j - 5.0) * 5.0);
            glVertex3f((i - 5.0) * 5.0, -2.5, (j - 4.0) * 5.0);
            glVertex3f((i - 4.0) * 5.0, -2.5, (j - 4.0) * 5.0);
            glVertex3f((i - 4.0) * 5.0, -2.5, (j - 5.0) * 5.0);
            glEnd();
        }
    glDisable(GL_COLOR_MATERIAL);
}

/*--------------------------------------------------------------------------------
 * Procedure to copy frambauffer image and load into a texture.
 */
void copy_framebuffer_texture(int texID, int xsize, int ysize) {
    glReadBuffer(GL_FRONT);
    glBindTexture(GL_TEXTURE_2D, texID);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 50, 50, TSIZE, TSIZE, 0);
}

/*-------------------------------------------------------
 * Procedure to draw a polygon as a blade of the windmill
 */
void draw_blade()
{
    glEnable(GL_COLOR_MATERIAL);
    glMaterialf(GL_FRONT, GL_SHININESS, 6.0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 4.0, 0.0);
    glVertex3f(1.0, 8.0, 0.0);
    glVertex3f(-1.0, 8.0, 0.0);
    glVertex3f(-1.0, 4.0, 0.0);
    glEnd();
    glDisable(GL_COLOR_MATERIAL);
}

/*--------------------------------------------------------
 * Procedure to draw a cube. The geometrical data of the cube
 * are defined above.
 */
void draw_cube(double x, double y, double z) {
    int    i;
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(x, y, z);  /* Set the current color */
    for (int i = 0; i < 6; i++) {
        glBegin(GL_POLYGON);  /* Draw the face */
        glNormal3fv(normal[i]);
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
    glDisable(GL_COLOR_MATERIAL);
}

void draw_heart(double x, double y, double z) {
    //plastic heart
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, black_plastic[0]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black_plastic[1]);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black_plastic[2]);
    glMaterialf(GL_FRONT, GL_SHININESS, shinny);
    glColor3f(x, y, z);  /* Set the current color */
    for (int i = 0; i < 13; i++) {
        glBegin(GL_POLYGON);  /* Draw the face */
        glNormal3fv(hpoints[i]);
        glVertex3fv(hpoints[i]);
        hpoints[i][2] += 2;
        glVertex3fv(hpoints[i]);
        hpoints[i][2] -= 2;
        glVertex3fv(hpoints[(i + 1) % 12]);
        hpoints[(i + 1) % 12][2] += 2;
        glVertex3fv(hpoints[(i + 1) % 12]);
        hpoints[(i + 1) % 12][2] -= 2;
        glEnd();
    }
    glBegin(GL_POLYGON);
    for (int i = 0; i < 13; i++) {
        /* Draw the face */
        glVertex3fv(hpoints[i]);
    }
    glEnd();
    glBegin(GL_POLYGON);
    for (int i = 0; i < 13; i++) {
        /* Draw the face */
        hpoints[i][2] += 2;
        glVertex3fv(hpoints[i]);
        hpoints[i][2] -= 2;
    }
    glEnd();
    glMaterialf(GL_FRONT, GL_SHININESS, 6.0);
    glDisable(GL_COLOR_MATERIAL);
}

void compute_ab_axes(void) {
    float w0, w2;
    double len;

    /*----Get w0 and w2 from the modelview matrix mtx[] ---*/
    w0 = mtx[2];
    w2 = mtx[10];

    len = sqrt(w0 * w0 + w2 * w2);
    /*---- Define the a and b axes for billboards ----*/
    b[0] = 0.0;
    b[1] = 1.0;
    b[2] = 0.0;
    a[0] = w2 / len;
    a[1] = 0.0;
    a[2] = -w0 / len;
}

/*-------------------------------------------------------
 * Display and Idle callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void draw_cone() {
    float colxyz[4] = { 1,1,1,1 }, colb[4] = { 0,0,0,1 };
    if (cr % 4 == 1) {
        colxyz[1] = colxyz[2] = 0;
    }
    else if (cr % 4 == 2) {
        colxyz[1] = colxyz[3] = 0;
    }
    else if (cr % 4 == 3) {
        colxyz[2] = colxyz[3] = 0;
    }
    glMaterialfv(GL_FRONT, GL_EMISSION, colxyz);
    autoy += 5;
    glRotated(30 * light_pos, 1, 0, 0);
    glTranslatef(0.0, -3, 0.0);
    gluSphere(sphere, 0.5,   /* radius=1.5 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 12 stacks */
    double deg = (30.0 + light_range * 30.0) / 180.0 * PI;
    glTranslatef(0.0, -(autoy * 3 / cos(deg)), 0.0);
    glRotatef(-90, 1, 0, 0);
    glutWireCone(tan(deg) * (autoy * 3 / cos(deg)), autoy * 3 / cos(deg), 32, 32);
    autoy -= 5;
    glMaterialfv(GL_FRONT, GL_EMISSION, colb);
}

void draw_scene() {

    glMaterialfv(GL_FRONT, GL_DIFFUSE, lt_diffuse);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[6]);
    gluQuadricTexture(sphere, GL_TRUE);
    glPushMatrix();
    glTranslatef(0, 20, 0);
    glScalef(-1, -1, -1);
    glRotatef(90, -1, 0, 0);
    glRotatef(skyAng, 0, 0, 1);
    gluSphere(sphere, 60, 20, 20);
    glPopMatrix();
    gluQuadricTexture(sphere, GL_FALSE);
    skyAng = skyAng + 0.01;
    if (skyAng > 360)skyAng -= 360;
    glMatrixMode(GL_MODELVIEW);
    draw_axes();
    //draw light-spot
    glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
    compute_ab_axes();
    if (light_spot == 1) {
        glPushMatrix();
        glTranslatef(20.0, 0.0, 20.0);
        glRotatef(lit_angle, 0.0, 1.0, 0.0);

        glPushMatrix();
        glTranslatef(lit_position[0], lit_position[1], lit_position[2]);
        glScalef(0.5, 0.5, 0.5);
        glDisable(GL_LIGHTING);
        glColor3f(0.9, 0.9, 0.0);
        glutWireSphere(1.0, 4, 4);
        glPopMatrix();

        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
        glPopMatrix();
    }
    else if (light_spot == 0) {
        glPushMatrix();
        if (light_enable)glEnable(GL_LIGHT0);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
        lt_pos[0] = ltp_s;
        glLightfv(GL_LIGHT0, GL_POSITION, lt_pos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[ch_color]);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lt_specular);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
        glPopMatrix();
    }
    else if (light_spot == 3) {
        glPushMatrix();
        float down_light[3] = { autox,autoy,autoz }, pos[4] = { 0,0,0,1 };
        glEnable(GL_LIGHTING);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30 + light_range * 30);
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, light_exp);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, down_light);
        glPopMatrix();
    }
    /*-------Draw the cube ----*/
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[3]);

    glPushMatrix();
    glTranslatef(4.0, 0.0, 4.0);
    glRotatef(cube_angle, 0.0, 1.0, 0.0);
    glScalef(3.0, 6.0, 3.0); /*set the metrics=6x6x6*/
    draw_cut_cube();
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-15, 0, 15);
    /*-----Draw a cylinder with brick texture ---*/

    glBindTexture(GL_TEXTURE_2D, TextureID[4]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glPushMatrix();
    glTranslatef(10.0, 0.0, 8.0);
    glRotatef(cube_angle, 0.0, 1.0, 0.0);
    draw_cylinder();
    glPopMatrix();
    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    /*-------Draw the billboard ----*/
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, TextureID[5]);

    draw_billboard(5.0 * 4.0, 3.0 * 4.0, 5.0, 8.0);
    glBindTexture(GL_TEXTURE_2D, TextureID[4]);
    draw_billboard(6.0 * 4.0, 5.0 * 4.0, 5.0, 8.0);
    glBindTexture(GL_TEXTURE_2D, TextureID[3]);
    draw_billboard(3.0 * 4.0, 6.0 * 4.0, 5.0, 8.0);
    glBindTexture(GL_TEXTURE_2D, TextureID[2]);
    draw_billboard(2.0 * 4.0, 7.0 * 4.0, 5.0, 8.0);
    glBindTexture(GL_TEXTURE_2D, TextureID[1]);
    draw_billboard(7.0 * 4.0, 2.0 * 4.0, 5.0, 8.0);


    glPushMatrix();
    glTranslatef(50, 0, 15);
    glRotatef(bld_ang, 0.0, 0.0, 1.0);
    glScalef(2, 2, 2);
    draw_heart(bld_ang / 360, bld_ang / 360, bld_ang / 360);
    glScalef(1 / 2, 1 / 2, 1 / 2);
    glPopMatrix();

    glPushMatrix();
    glRotatef(45.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    draw_floor();
    glScalef(5, 20, 5);
    for (int i = 0; i < 2; ++i) {
        glTranslatef(i * 5, 0, 0);
        Draw_UnitCube((select + i) % 3);
        glTranslatef(-i * 5, 0, 0);
    }

    glTranslatef(5, 0, 8);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1, 0, 0);
    glRotatef(270.0, 1.0, 0.0, 0.0);
    glScalef(1, 1 / 4, 1);
    gluCylinder(cylind, 1, 1, /* radius of top and bottom circle */
        3.0,              /* height of the cylinder */
        100,               /* use 12-side polygon approximating circle*/
        1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30, 30, 10);
    glScalef(12, 12, 12);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(0, 1, 1);
    gluSphere(sphere, 0.5,   /* radius=1.5 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 12 stacks */
    glScalef(1, 0.01, 1 / 5);
    glColor3f(1, 1, 1);
    float colxyz[4] = { 1,1,1,1 }, colb[4] = { 0,0,0,1 };
    if (cr % 4 == 1) {
        colxyz[1] = colxyz[2] = 0;
    }
    else if (cr % 4 == 2) {
        colxyz[1] = colxyz[3] = 0;
    }
    else if (cr % 4 == 3) {
        colxyz[2] = colxyz[3] = 0;
    }
    glMaterialfv(GL_FRONT, GL_EMISSION, colxyz);
    draw_blade();
    glRotatef(180.0, 1.0, 0.0, 0.0);
    draw_blade();
    glMaterialfv(GL_FRONT, GL_EMISSION, colb);
    glPopMatrix();
    glTranslatef(autox, autoy, autoz);

    /*-------Draw the base of the windmill which is a cube----*/
    glTranslatef(10.0, 0.0, 6.0);
    glRotatef(auto_rotate, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glRotatef(Rotate, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glTranslatef(0.0, 5.0, 0.0);

    glRotatef(-45, 0.0, 1.0, 0.0);
    glRotatef(fx, 1.0, 0.0, 0.0);
    glRotatef(fz, 0.0, 0.0, 1.0);
    glRotatef(fy, 0.0, 1.0, 0.0);
    glRotatef(45, 0.0, 1.0, 0.0);
    if (fx > 0)
        fx -= 0.01;
    if (fx < 0)
        fx += 0.01;
    if (fz > 0)
        fz -= 0.01;
    if (fz < 0)
        fz += 0.01;
    if (autoy < 0)
        autoy = 0;
    if (autoy == 0)
        fx = fz = 0;
    //draw head light
    if (light_spot == 2) {
        glPushMatrix();
        glRotatef(-45, 0.0, 1.0, 0.0);
        float down_light[3] = { 0,-1,-light_pos * 0.5 }, pos[4] = { 0,0,0,1 };
        glEnable(GL_LIGHTING);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30 + light_range * 30);
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, light_exp);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, down_light);
        if (dc == 1) {
            draw_cone();
        }
        glPopMatrix();
    }

    //BODY
    glPushMatrix();              /* Save M1 coord. sys */
    //draw_h_light();
    glRotatef(45.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glScalef(1.0, 1.0, 2.0);
    draw_heart(1, 0, 0);
    glScalef(1.0, 1.0, 1 / 2.0);
    glScalef(6.0, 6.0, 6.0);     /* Scale up the axes by 6 times */
    draw_cube(0.2, 0.5, 0.2);                 /* Draw a unit cube in the new coord. sys. */
    glPopMatrix();               /* Get M1 back */

    //tail
    glPushMatrix();              /* Save M1 coord. sys */
    glRotatef(45.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glRotatef(90.0, 0.0, 0.0, 1.0);
    glTranslatef(0.0, 3.0, 0);
    glScalef(1.5, 1.5, 1.5);
    draw_cube(0, 0, 1);
    glTranslatef(0.0, 3.0, 0);
    glScalef(1 / 1.5, 10.0 / 1.5, 1 / 1.5);
    draw_cube(0.1, 0.5, 0.1);    /* Draw a unit cube in the new coord. sys. */
    glPopMatrix();

    //legs
    glPushMatrix();              /* Save M1 coord. sys */
    glRotatef(45.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glRotatef(90.0, 0.0, 0.0, 1.0);
    glTranslatef(-5.0, 0.0, 5);
    glScalef(1, 10.0, 1);
    draw_cube(0.1, 0.5, 0.1);    /* Draw a unit cube in the new coord. sys. */
    glTranslatef(0.0, 0, -10);
    draw_cube(0.1, 0.5, 0.1);    /* Draw a unit cube in the new coord. sys. */
    glPopMatrix();

    //legs arm
    glPushMatrix();
    glRotatef(45.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glRotatef(40.0, 1.0, 0, 0.0);
    glTranslatef(-2, -0.5, 4);
    glColor3f(0, 0, 1);  /* Set the current color */
    gluCylinder(cylind, 0.25, 0.25, /* radius of top and bottom circle */
        3.0,              /* height of the cylinder */
        100,               /* use 12-side polygon approximating circle*/
        1);               /* Divide it into 3 sections */
    glTranslatef(4, 0, 0);
    gluCylinder(cylind, 0.25, 0.25, /* radius of top and bottom circle */
        3.0,              /* height of the cylinder */
        100,               /* use 12-side polygon approximating circle*/
        1);
    glPopMatrix();

    //legs arm
    glPushMatrix();
    glRotatef(45.0, 0.0, 1.0, 0.0);  /* rotate about y-axis by 30' */
    glRotatef(140.0, 1.0, 0, 0.0);
    glTranslatef(-2, 0.5, 4);
    glColor3f(0, 0, 1);  /* Set the current color */
    gluCylinder(cylind, 0.25, 0.25, /* radius of top and bottom circle */
        3.0,              /* height of the cylinder */
        100,               /* use 12-side polygon approximating circle*/
        1);               /* Divide it into 3 sections */
    glTranslatef(4, 0, 0);
    gluCylinder(cylind, 0.25, 0.25, /* radius of top and bottom circle */
        3.0,              /* height of the cylinder */
        100,               /* use 12-side polygon approximating circle*/
        1);
    glPopMatrix();

    //head arm
    glPushMatrix();
    glRotatef(45.0, 0.0, 1.0, 0.0);
    glTranslatef(0, 3, 0);
    draw_cube(0, 0, 1);
    glTranslatef(0, 2, 0);
    gluSphere(sphere, 0.5,   /* radius=1.5 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 12 stacks */
    glTranslatef(0, -1, 0);
    //brade
    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(0, 1, 0);
    glRotatef(90.0, 1.0, 0.0, 0.0);

    gluCylinder(cylind, 0.25, 0.25, /* radius of top and bottom circle */
        3.0,              /* height of the cylinder */
        100,               /* use 12-side polygon approximating circle*/
        1);
    glRotatef(bld_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
    draw_blade();
    glRotatef(180.0, 1.0, 0.0, 0.0);
    draw_blade();
    glRotatef(90.0, 0.0, 0.0, 1.0);
    draw_blade();
    glRotatef(180.0, 0.0, 0.0, 1.0);
    draw_blade();
    glPopMatrix();

    //tail arm
    glPushMatrix();
    glRotatef(45.0, 0.0, 1.0, 0.0);
    glTranslatef(-13, 0, 0);
    draw_cube(0, 0, 1);
    glScalef(0.3, 0.3, 0.3);
    glTranslatef(0, 3, 0);
    gluSphere(sphere, 0.5,   /* radius=1.5 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 12 stacks */
    glTranslatef(0, -1, 0);
    //brade
    glColor3f(0.5, 0.5, 0.5);
    glTranslatef(0, 1, 0);
    glRotatef(90.0, 1.0, 0.0, 0.0);

    gluCylinder(cylind, 0.25, 0.25, /* radius of top and bottom circle */
        3.0,              /* height of the cylinder */
        100,               /* use 12-side polygon approximating circle*/
        1);
    glRotatef(bld_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
    draw_blade();
    glRotatef(180.0, 1.0, 0.0, 0.0);
    draw_blade();
    glRotatef(90.0, 0.0, 0.0, 1.0);
    draw_blade();
    glRotatef(180.0, 0.0, 0.0, 1.0);
    draw_blade();
    glPopMatrix();
}
void display()
{
    /*Clear previous frame and the depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (cylind == NULL) {               /* allocate a quadric object, if necessary */
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }

    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
    }

    switch (style) {
    case 0:
        make_view(4);
        make_projection(4);
        glViewport(width / 2, 0, width / 2, height / 2);
        draw_scene();

        make_view(1);
        make_projection(1);
        glViewport(0, height / 2, width / 2, height / 2);
        draw_scene();
        make_view(1);
        draw_view();

        make_view(2);
        glViewport(width / 2, height / 2, width / 2, height / 2);
        draw_scene();
        make_view(2);
        draw_view();

        make_view(3);
        glViewport(0, 0, width / 2, height / 2);
        draw_scene();
        make_view(3);
        draw_view();
        break;

    case 1:
        glViewport(0, 0, width, height);
        make_view(1);
        make_projection(1);
        draw_scene();
        make_view(1);
        draw_view();
        break;

    case 2:
        glViewport(0, 0, width, height);
        make_view(2);
        draw_scene();
        make_view(2);
        draw_view();
        break;

    case 3:
        glViewport(0, 0, width, height);
        make_view(3);
        draw_scene();
        make_view(3);
        draw_view();
        break;

    case 4:
        glViewport(0, 0, width, height);
        make_view(4);
        make_projection(4);
        draw_scene();
        break;
    }

    glutSwapBuffers();
    glFlush(); /*--Display the results----*/
}
/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{
    width = w;
    height = h;

    glViewport(0, 0, w / 1.2, h / 1.2);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-15.0, 15.0, -15.0 * (float)h / (float)w, 15.0 * (float)h / (float)w,
            -10.5, 30.0);
    else
        glOrtho(-15.0 * (float)w / (float)h, 15.0 * (float)w / (float)h, -15.0, 15.0,
            -10.5, 30.0);
}

/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void my_keyboard(unsigned char key, int ix, int iy)
{
    int i;
    float  x[3], y[3], z[3];

    if (key == 'Q' || key == 'q') exit(0);
    else if (key == 'Z') {
        light_exp += 2;
        if (light_exp > 20)light_exp = 0;
    }
    else if (key == 'y') {
        ltp_s += 5;
        if (ltp_s >= 180) ltp_s = -180;
    }
    else if (key == '?') {
        ds += 0.1;
        if (ds > 1.0) ds = ds - 1.0;
    }
    else if (key == '<') {
        dt += 0.1;
        if (dt > 1.0) dt = dt - 1.0;
    }
    else if (key == '.') {
        fc = (fc + 1) % 4;
        if (fc == 0) {
            fog_color[0] = 0.15;
            fog_color[1] = 0.2;
            fog_color[2] = 0.2;
            fog_color[3] = 0.5;
        }
        else {
            fog_color[0] = fog_color[1] = fog_color[2] = fog_color[3] = 0;
            fog_color[fc] = 0.2;
        }
        glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
    }
    else if (key == 'F') {
        fog_des += 0.02;
        if (fog_des >= 0.25)fog_des = 0.01;
        glFogf(GL_FOG_DENSITY, fog_des);    /*fog opacity(density)= 0.25*/
    }
    else if (key == 'f') {
        fog_state = (fog_state + 1) % 4;
        if (fog_state != 0) glEnable(GL_FOG);
        if (fog_state == 1)glFogi(GL_FOG_MODE, GL_LINEAR);
        else if (fog_state == 2)glFogi(GL_FOG_MODE, GL_EXP);
        else if (fog_state == 3)glFogi(GL_FOG_MODE, GL_EXP2);
        else glDisable(GL_FOG);
    }
    else if (key == 'C') {
        copy_framebuffer_texture(TextureID[3], TSIZE, TSIZE);
    }
    else if (key == '*') {
        shinny *= 2;
    }
    else if (key == '/') {
        shinny /= 2;
    }
    else if (key == 'c') {
        ++cr;
    }
    else if (key == 'x') {
        glEnable(GL_LIGHTING);
        if (sh == 0) {
            glShadeModel(GL_FLAT);
            sh = 1;
        }
        else {
            glShadeModel(GL_SMOOTH);
            sh = 0;
        }
        setup_light();
    }
    else if (key == '$') {
        light_pos = (light_pos + 1) % 3;
    }
    else if (key == '>') {
        light_range = (light_range + 1) % 3;
    }
    else if (key == '-') {
        if (light_str < 3) {
            light_str += 0.1;
        }
        for (int i = 0; i < 3; ++i) {
            light_color[ch_color][i] *= 0.9;
        }
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light_str);
    }
    else if (key == '+') {
        if (light_str > 0) {
            light_str -= 0.1;
        }
        for (int i = 0; i < 3; ++i) {
            light_color[ch_color][i] *= 1.1;
        }
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light_str);
    }
    else if (key == '@') { /*turn on/off the light*/
        if (light_enable) {
            light_enable = 0;
            glDisable(GL_LIGHT0);
        }
        else {
            light_enable = 1;
            glEnable(GL_LIGHT0);
        }
    }
    else if (key == '#') {
        ch_color = (ch_color + 1) % 4;
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color[ch_color]);
    }
    else if (key == '!') {
        light_spot = (light_spot + 1) % 4;
    }
    else if (key == ' ') lit_angle += 10.0;
    else if (key == '[') {
        if (dc)dc = 0;
        else dc = 1;
    }
    else if (key == ']') {
        if (cb)cb = 0;
        else cb = 1;
    }
    else if (key >= '0' && key <= '4') {
        style = key - '0';
    }
    else if (key == 'p') {
        if (stp == 1) {
            stp = 0;
        }
        else {
            stp = 1;
        }
    }
    else if (key == 'D') {
        eyeDy += 0.5;       /* move up */
        for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[1][i];
    }
    else if (key == 'U') {
        eyeDy += -0.5;       /* move down */
        for (i = 0; i < 3; i++) eye[i] += 0.5 * u[1][i];
    }
    else if (key == 'R') {
        eyeDx += -0.5;       /* move left */
        for (i = 0; i < 3; i++) eye[i] += 0.5 * u[0][i];
    }
    else if (key == 'L') {
        eyeDx += 0.5;        /* move right */
        for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[0][i];
    }
    else if (key == 'I') {
        eyeDz += 0.5;        /* zoom in */
        for (i = 0; i < 3; i++) eye[i] -= 0.5 * u[2][i];
    }
    else if (key == 'O') {
        eyeDz += -0.5;       /* zoom out */
        for (i = 0; i < 3; i++) eye[i] += 0.5 * u[2][i];
    }
    else if (key == 'P') {//pitching
        eyeAngx += -5.0;
        if (eyeAngx < 0.0) eyeAngx += 360.0;
        y[0] = u[1][0] * cv + u[2][0] * sv;
        y[1] = u[1][1] * cv + u[2][1] * sv;
        y[2] = u[1][2] * cv + u[2][2] * sv;

        z[0] = u[2][0] * cv - u[1][0] * sv;
        z[1] = u[2][1] * cv - u[1][1] * sv;
        z[2] = u[2][2] * cv - u[1][2] * sv;

        for (i = 0; i < 3; i++) {
            u[1][i] = y[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'H') {/* heading */
        eyeAngy += -5.0;
        if (eyeAngy < 0.0) eyeAngy += 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] + sv * u[2][i];
            z[i] = -sv * u[0][i] + cv * u[2][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[2][i] = z[i];
        }
    }
    else if (key == 'r') {/* rolling */
        eyeAngz += -5.0;
        if (eyeAngz < 0.0) eyeAngz += 360.0;
        for (i = 0; i < 3; i++) {
            x[i] = cv * u[0][i] + sv * u[1][i];
            y[i] = -sv * u[0][i] + cv * u[1][i];
        }
        for (i = 0; i < 3; i++) {
            u[0][i] = x[i];
            u[1][i] = y[i];
        }
    }
    if (stp == 0) {
        if (autoy != 0) {
            if (key == 'd') {
                autox += Step * cos((Rotate - 45) * PI / 180.0);
                autoz -= Step * sin((Rotate - 45) * PI / 180.0);
                if (fz > -30) {
                    fz -= 1;
                }
            }
            else if (key == 'a') {
                autox += Step * cos((Rotate + 135) * PI / 180.0);
                autoz -= Step * sin((Rotate + 135) * PI / 180.0);
                if (fz < 30) {
                    fz += 1;
                }
            }
            else if (key == 'w') {
                autox += Step * cos((Rotate + 45) * PI / 180.0);
                autoz -= Step * sin((Rotate + 45) * PI / 180.0);
                if (fx > -30) {
                    fx -= 1;
                }
            }
            else if (key == 's') {
                autox -= Step * cos((Rotate + 45) * PI / 180.0);
                autoz += Step * sin((Rotate + 45) * PI / 180.0);
                if (fx < 30) {
                    fx += 1;
                }
            }
            else if (key == 'u') {
                ++Rotate;
            }
            else if (key == 'i') {
                --Rotate;
            }
            else if (key == 'o') {
                my_keyboard('u', 0, 0);
                my_keyboard('w', 0, 0);
            }
            else if (key == 'l') {
                my_keyboard('i', 0, 0);
                my_keyboard('w', 0, 0);
            }
        }
        if (key == 'j') {
            ++autoy;
        }
        else if (key == 'k') {
            --autoy;
        }
    }
    display();
}

/*---------------------------------------------
 * Idle event callback function, increase blade and arm
 * rotation angles.
 */
void idle_func()
{
    //auto_rotate += 0.01;
    if (stp == 0 && autoy != 0)bld_ang += 1.0;
    if (auto_rotate > 360)auto_rotate -= 360;
    if (bld_ang > 360.0)bld_ang -= 360.0;
    display(); /* show the scene again */
}

/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char** argv)
{
    /*-----Initialize the GLUT environment-------*/
    for (int i = 0; i < 13; ++i) {
        hpoints[i][0] = hx[i];
        hpoints[i][1] = hy[i];
    }
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("直升機");

    myinit();      /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);
    glutIdleFunc(idle_func);    /* Add idle event */
    glutReshapeFunc(my_reshape);
    glutKeyboardFunc(my_keyboard);

    glutMainLoop();
}
