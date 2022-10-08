#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A, B, C;

float cubeWidth = 20;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = ' ';
int distanceFromCam = 80;
float horizontalOffset;
float K1 = 40;

float incrementSpeed = 0.6;

float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k)
{
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
           j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k)
{
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
           j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
           i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k)
{
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch)
{
    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1 / z;

    xp = (int)(width / 2 + horizontalOffset + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;
    if (idx >= 0 && idx < width * height)
    {
        if (ooz > zBuffer[idx])
        {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main()
{
    printf("\x1b[2J");
    while (1)
    {
        memset(buffer, backgroundASCIICode, width * height);
        memset(zBuffer, 0, width * height * 4);
        cubeWidth = 15;
        horizontalOffset = -2 * cubeWidth;
        // first cube
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed)
        {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed)
            {
                if (cubeY < 2 * (cubeX) + cubeWidth && cubeY < -2 * (cubeX) + cubeWidth)
                {
                    // calculateForSurface(-cubeWidth, cubeY, -cubeX, '~'); // y -90 from buttom
                    // calculateForSurface(-cubeWidth, cubeX, cubeY, '#');                                                       // 1
                    calculateForSurface((sqrt(3) * (-cubeWidth) + cubeY) / 2 + 5, cubeX, (sqrt(3) * cubeY + cubeWidth) / 2 - 10, '~'); // y -30
                    // calculateForSurface(cubeX, -cubeWidth, -cubeY, '~'); // x 90 from buttom
                    // calculateForSurface(-cubeX, -cubeWidth, cubeY, '~'); // 2
                    calculateForSurface(-cubeX, (sqrt(3) * (-cubeWidth) + cubeY) / 2 + 5, (sqrt(3) * cubeY + cubeWidth) / 2 - 10, '+');
                    // calculateForSurface(cubeX, cubeWidth, cubeY, ';'); // x -90 from buttom // 3
                    calculateForSurface(cubeX, (sqrt(3) * cubeWidth - cubeY) / 2 - 5, (sqrt(3) * cubeY + cubeWidth) / 2 - 10, '$');
                    // calculateForSurface(cubeWidth, cubeY, cubeX, '!'); // y 90 from buttom
                    // calculateForSurface(cubeWidth, cubeX, cubeY, '+'); // 4
                    calculateForSurface((sqrt(3) * cubeWidth - cubeY) / 2 - 5, cubeX, (cubeWidth + sqrt(3) * cubeY) / 2 - 10, '#');
                }
                calculateForSurface(cubeX, cubeY, -cubeWidth, ';');
            }
        }
        printf("\x1b[H");
        for (int k = 0; k < width * height; k++)
        {
            putchar(k % width ? buffer[k] : 10);
        }

        A += 0.05;
        B += 0.05;
        C += 0.01;
        usleep(8000 * 2);
    }
    return 0;
}