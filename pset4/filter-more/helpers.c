#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            BYTE average = round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE aux;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width / 2; ++j)
        {
            aux = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = aux;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE aux[height][width];

    int dx[] = {0, 0, 1, 1, 1, -1, -1, -1};
    int dy[] = {-1, 1, -1, 0, 1, -1, 0, 1};

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int r = image[i][j].rgbtRed;
            int b = image[i][j].rgbtBlue;
            int g = image[i][j].rgbtGreen;
            float cnt = 1;
            for (int d = 0; d < 8; ++d)
            {
                int x = dx[d] + i;
                int y = dy[d] + j;
                if (x >= 0 && x < height && y >= 0 && y < width)
                {
                    cnt ++;
                    r += image[x][y].rgbtRed;
                    b += image[x][y].rgbtBlue;
                    g += image[x][y].rgbtGreen;
                }

            }
            aux[i][j].rgbtRed = round(r / cnt);
            aux[i][j].rgbtBlue = round(b / cnt);
            aux[i][j].rgbtGreen = round(g / cnt);

        }
    }
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            image[i][j] = aux[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int GX[][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int GY[][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int dx[] = {0, 0, 1, 1, 1, -1, -1, -1};
    int dy[] = {-1, 1, -1, 0, 1, -1, 0, 1};

    RGBTRIPLE aux[height][width];

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            //in each kernel the middle value equals to 0
            int REDx = 0, REDy = 0, RED;
            int BLUEx = 0, BLUEy = 0, BLUE;
            int GREENx = 0, GREENy = 0, GREEN;
            for (int k = 0; k < 8; ++k)
            {
                int x = dx[k] + i;
                int y = dy[k] + j;
                if (x >= 0 && x < height && y >= 0 && y < width)
                {
                    REDx += image[x][y].rgbtRed * GX[1 + dx[k]][1 + dy[k]];
                    BLUEx += image[x][y].rgbtBlue * GX[1 + dx[k]][1 + dy[k]];
                    GREENx += image[x][y].rgbtGreen * GX[1 + dx[k]][1 + dy[k]];

                    REDy += image[x][y].rgbtRed * GY[1 + dx[k]][1 + dy[k]];
                    BLUEy += image[x][y].rgbtBlue * GY[1 + dx[k]][1 + dy[k]];
                    GREENy += image[x][y].rgbtGreen * GY[1 + dx[k]][1 + dy[k]];
                }
            }
            RED = round(sqrt(REDx * REDx + REDy * REDy));
            BLUE = round(sqrt(BLUEx * BLUEx + BLUEy * BLUEy));
            GREEN = round(sqrt(GREENx * GREENx + GREENy * GREENy));

            if (RED > 255)
            {
                RED = 255;
            }
            if (BLUE > 255)
            {
                BLUE = 255;
            }
            if (GREEN > 255)
            {
                GREEN = 255;
            }
            aux[i][j].rgbtRed = RED;
            aux[i][j].rgbtBlue = BLUE;
            aux[i][j].rgbtGreen = GREEN;
        }
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            image[i][j] = aux[i][j];
        }
    }
    return;
}
