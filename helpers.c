#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Define average variables
    int avg;
    float total;
    //Loop through each pixel in each row
    for  (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Calc average of all 3 rgb values
            avg = image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue;
            total = avg / 3.0;
            //Round average
            avg = round(total);
            //Assign average to each rgb value
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Define swap variable
    RGBTRIPLE swap;
    //Loop through each pixel in each row
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            //Swap the current pixel with it's opposite equivalent
            
            //Put original pixel into variable
            swap.rgbtRed = image[i][j].rgbtRed;
            swap.rgbtGreen = image[i][j].rgbtGreen;
            swap.rgbtBlue = image[i][j].rgbtBlue;
            
            //Move end pixel into original
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            
            //Make end pixel into original with saved variable
            image[i][width - j - 1].rgbtRed = swap.rgbtRed;
            image[i][width - j - 1].rgbtGreen = swap.rgbtGreen;
            image[i][width - j - 1].rgbtBlue = swap.rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Define blur variables
    int counter = 0;
    RGBTRIPLE pixel[9];
    float sum;   
    int ared = 0;
    int agreen = 0;
    int ablue = 0;

    //Loop through each pixel in each row
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Loop through the 9 pixels square
            for (int h = i - 1; h <= i + 1; h++)
            {
                for (int w = j - 1; w <= j + 1; w++)
                {
                    //Check if the pixel exists
                    if (h >= 0 && w >= 0 && h < height && w < width)
                    {
                        //Assign rgb values to pixel variable
                        pixel[counter].rgbtRed = image[h][w].rgbtRed;
                        pixel[counter].rgbtGreen = image[h][w].rgbtGreen;
                        pixel[counter].rgbtBlue = image[h][w].rgbtBlue;
                        //Add to counter
                        counter++;
                    }
                }
            }
            //Loop through counter
            for (int k = 0; k < counter; k++)
            {
                //Add all reds greens & blues together
                ared += pixel[k].rgbtRed;
                agreen += pixel[k].rgbtGreen;
                ablue += pixel[k].rgbtBlue;
            }
            //Average & round red
            sum = ared / counter;
            ared = round(sum);
            //Average & round green
            sum = agreen / counter;
            agreen = round(sum);
            //Average & round blue
            sum = ablue / counter;
            ablue = round(sum);
            
            //Assign new average values to current pixel
            image[i][j].rgbtRed = ared;
            image[i][j].rgbtGreen = agreen;
            image[i][j].rgbtBlue = ablue;
            
            //Reset counter
            counter = 0;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Define edge variables
    int counter;
    int gxt[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gyt[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    int gx[3];
    int gy[3];
    double square;
    int temp;
    //Loop through each pixel in each row
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Loop through the 9 pixels square
            for (int h = i - 1; h <= i + 1; h++)
            {
                for (int w = j - 1; w <= j + 1; w++)
                {
                    //Check if the pixel exists
                    if (h >= 0 && w >= 0 && h < height && w < width)
                    {
                        //Calc gx table for each rgb per pixel
                        gx[0] += (image[h][w].rgbtRed * gxt[counter]);
                        gx[1] += (image[h][w].rgbtGreen * gxt[counter]);
                        gx[2] += (image[h][w].rgbtBlue * gxt[counter]);
                        
                        //Calc gy table for each rgb per pixel
                        gy[0] += (image[h][w].rgbtRed * gyt[counter]);
                        gy[1] += (image[h][w].rgbtGreen * gyt[counter]);
                        gy[2] += (image[h][w].rgbtBlue * gyt[counter]);
                    }
                    //Add to counter
                    counter++;
                    }
                }
            }
            //Calc the new final value for each color by square root of gx^2 + gy^2 & set the pixel to that color
            
            //Red
            square = sqrt(pow(gx[0], 2) + pow(gy[0], 2));
            temp = round(square);
            if (temp > 255)
            {
                temp = 255;
            }
            image[i][j].rgbtRed = temp;
            
            //Green
            square = sqrt(pow(gx[1], 2) + pow(gy[1], 2));
            temp = round(square);
            if (temp > 255)
            {
                temp = 255;
            }
            image[i][j].rgbtGreen = temp;
            
            //Blue
            square = sqrt(pow(gx[2], 2) + pow(gy[2], 2));
            temp = round(square);
            if (temp > 255)
            {
                temp = 255;
            }
            image[i][j].rgbtBlue = temp
            
            //Reset counter
            counter = 0;
            
            //Reset gx & gy arrays
            gx[0] = 0;
            gx[1] = 0;
            gx[2] = 0;
            gy[0] = 0;
            gy[1] = 0;
            gy[2] = 0;
        }
    }
    return;
}
