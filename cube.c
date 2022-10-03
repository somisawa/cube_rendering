#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int width = 160, height = 44;
char buffer[160 * 44];
float zBuffer[160 * 44];
float focalX = 20.;
float focalY = 10.;
float cubeWidth = 40.;
float stepsize = 0.6;
float distantFromCam = 100.;
float inversedZ;
float A, B, C;
int X, Y;
int ct;

typedef struct {
    float x, y, z;
} Cord3D;

Cord3D cube;

float calculateForX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateForY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

float calculateForZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char c) {
    cube.x = calculateForX(cubeX, cubeY, cubeZ);
    cube.y = calculateForY(cubeX, cubeY, cubeZ);
    cube.z = calculateForZ(cubeX, cubeY, cubeZ) + distantFromCam;

    inversedZ = 1 / (cube.z);

    X = (int)(focalX * cube.x * inversedZ) + width / 2;
    Y = (int)(focalY * cube.y * inversedZ) + height / 2;

    ct = X + width * Y;
    if (ct >= 0 && ct < width * height) {
        if (inversedZ > zBuffer[ct]) {
            zBuffer[ct] = inversedZ;
            buffer[ct] = c;
        }
    }
}

int main() {

    A, B, C = 0, 0, 0;

    printf("\x1b[2J");
    while (1) {
        memset(buffer, ' ', width * height);
        memset(zBuffer, 0, width * height * 4);
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += stepsize) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth;
                 cubeY += stepsize) {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
                calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
                calculateForSurface(cubeWidth, cubeY, cubeX, '$');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '+');
                calculateForSurface(cubeX, cubeWidth, cubeY, '~');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
            }
        }
        printf("\x1b[H");
        for (int i = 0; i < width * height; i++) {
            putchar(i % width ? buffer[i] : 10);
        }
        A += 0.05;
        B += 0.05;
        C += 0.01;

        usleep(8000);
    }
    return 0;
}
