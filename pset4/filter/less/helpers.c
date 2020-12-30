#include "helpers.h"
#include "math.h"
#include <stdlib.h>

// Function to swap pixel a for pixel b
void swapRGBTRIPLE(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE tmp = *a;
    *a = *b;
    *b = tmp;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i ++) // loop all rows of pixels
    {
        for (int j = 0; j < width; j ++) // loop all pixels in a row
        {
            // *red (*green/*blue), red (green/blue) are the value, pointer (address) of each color of the pixel[i][j]
            BYTE *red = &image[i][j].rgbtRed;
            BYTE *green = &image[i][j].rgbtGreen;
            BYTE *blue = &image[i][j].rgbtBlue;

            /* calculate the average of pixel's rgbtRed (*red), rgbtGreen (*green) and rgbtBlue (*blue)
            store it as a float
            and change the float type to nearest integer */
            float tmp_grey = ((float) *red + *green + *blue) / 3;
            int grey = round(tmp_grey);

            // set the value of each colour in each pixel to grey
            *red = grey;
            *green = grey;
            *blue = grey;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i ++)  // loop all rows of pixels
    {
        for (int j = 0; j < width; j ++)  // loop all pixels in a row
        {
            // *red (*green/*blue), red (green/blue) are the value, pointer (address) of each color of the pixel[i][j]
            BYTE *red = &image[i][j].rgbtRed;
            BYTE *green = &image[i][j].rgbtGreen;
            BYTE *blue = &image[i][j].rgbtBlue;

            /* calculate the new value of pixel's rgbtRed, rgbtGreen, and rgbtBlue
            and store it as floats sepiaRed, sepiaGreen, sepiaBlue */
            float sepiaRed = 0.393 * (float) *red + 0.769 * *green + 0.189 * *blue;
            float sepiaGreen = 0.349 * (float) *red + 0.686 * *green + 0.168 * *blue;
            float sepiaBlue = 0.272 * (float) *red + 0.534 * *green + 0.131 * *blue;

            /* convert the type of sepiaRed, sepiaGreen and sepiaBlue from float to integer
            take the min value between that and 255 (the max possible hexadecimal value for RGB)
            and set the value of each colour in each pixel to the sepia value */
            *red = fmin(255, round(sepiaRed));
            *green = fmin(255, round(sepiaGreen));
            *blue = fmin(255, round(sepiaBlue));
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i ++) // loop all rows of pixels
    {
        if (width % 2 == 0)  // if the no. of pixels in a row is an even number
        {
            for (int j = 0; j < width / 2; j ++) // loop through the 1st half of all pixels in that row
            {
                // swap the pixel in 1st half to the symetrical one in the 2nd half
                swapRGBTRIPLE(&image[i][j], &image[i][width - 1 - j]);
            }
        }

        else // if the no. of pixels in a row is an odd number
        {
            // loop through the 1st half of all pixels in that row, except the middle one
            for (int j = 0; j < (width - 1) / 2; j ++)
            {
                 // swap the pixel in 1st half to the symetrical one in the 2nd half
                swapRGBTRIPLE(&image[i][j], &image[i][width - 1 - j]);
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // declare a new bitmap to store the blur pixels
    RGBTRIPLE blur [height][width];

    for (int i = 0; i < height; i ++) // loop all rows of pixels
    {
        for (int j = 0; j < width; j ++) // loop all pixels in a row
        {
            /* initialise the sum of color value in both the current pixel
            and the adjacent pixels */
            int sum_red = 0;
            int sum_green = 0;
            int sum_blue = 0;

            /* initialise the total number of the adjacent pixels plus the current pixel */
            int count = 0;

            // calculate the sum and count of color value
            for (int i1 = i - 1; i1 <= i + 1; i1 ++) // loop 3 rows of pixels
            {
                for (int j1 = j - 1; j1 <= j + 1; j1 ++) //// loop 3 pixels in a row
                {
                    // allocate 3 BYTE size of memory for 3 color value in each pixel
                    BYTE *red = malloc(sizeof(BYTE));
                    BYTE *green = malloc(sizeof(BYTE));
                    BYTE *blue = malloc(sizeof(BYTE));

                    // if the current pixel is not existed
                    if (i1 > height - 1 || i1 < 0 || j1 > width - 1 || j1 < 0)
                    {
                        // set the color value to 0
                        *red = 0;
                        *green = 0;
                        *blue = 0;
                    }

                    else // if the current pixel is existed
                    {
                        // put the color value into the memory assigned
                        *red = image[i1][j1].rgbtRed;
                        *green = image[i1][j1].rgbtGreen;
                        *blue = image[i1][j1].rgbtBlue;

                        // increment the count by 1
                        count ++;
                    }

                    // increment sum by the color value of the current pixel
                    sum_red += *red;
                    sum_green += *green;
                    sum_blue += *blue;

                    // free the dynamic memory allocated
                    free(red);
                    free(green);
                    free(blue);
                }
            }

            // *blur_red (*blur_green/*blur_blue) is the Red (Green/Blue) value in each pixel
            BYTE *blur_red = &blur[i][j].rgbtRed;
            BYTE *blur_green = &blur[i][j].rgbtGreen;
            BYTE *blur_blue = &blur[i][j].rgbtBlue;

            // set the blur value to the rounded average color value of adjacent pixel and the current one
            *blur_red = round((float) sum_red / count);
            *blur_green = round((float) sum_green / count);
            *blur_blue = round((float) sum_blue / count);
        }
    }

    for (int i = 0; i < height; i ++) // loop all rows of pixels
    {
        for (int j = 0; j < width; j ++) // loop all pixels in a row
        {
            // copy the value from the blur image to the current image
            image[i][j].rgbtRed = blur[i][j].rgbtRed;
            image[i][j].rgbtGreen = blur[i][j].rgbtGreen;
            image[i][j].rgbtBlue = blur[i][j].rgbtBlue;
        }
    }
    return;
}