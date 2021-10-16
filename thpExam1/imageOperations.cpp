/** ***************************************************************************
 * @file
 * 
 * @brief contains functions that manipulate the image's pixel values
 *****************************************************************************/
#include "netPBM.h"

/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Takes each pixel in image and adds a given value to it while making sure 
 * that pixel is not less than 0 or greater than 255.
 *
 * @param[in,out] img - an image structure that holds the pixel arrays.
 * @param[in]     value - the user's value that is added to each of the pixels.
 *
 * @par Example
 * @verbatim
   // brighten(img, value);
   @endverbatim
 *****************************************************************************/
void brighten(image& img, int value)
{
    int i;
    int j;
    int temp_value;

    // Calculate brightened value for each pixel.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            temp_value = (unsigned long) img.redgray[i][j] + value;
            checkNum(temp_value);
            img.redgray[i][j] = temp_value;

            temp_value = (unsigned long) img.green[i][j] + value;
            checkNum(temp_value);
            img.green[i][j] = temp_value;

            temp_value = (unsigned long) img.blue[i][j] + value;
            checkNum(temp_value);
            img.blue[i][j] = temp_value;
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Negates each pixel stored in the image. It takes each pixel and subtracts it
 * from the max color value, 255.
 *
 * @param[in,out] img - an image structure that holds the images contents.
 *
 * @par Example
 * @verbatim
   // negate(img);
   @endverbatim
 *****************************************************************************/
void negate(image& img)
{
    int i;
    int j;

    // Negate each pixel.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            img.redgray[i][j] = 255 - img.redgray[i][j];
            img.green[i][j] = 255 - img.green[i][j];
            img.blue[i][j] = 255 - img.blue[i][j];
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Converts the image to grayscale. It multiplies the red pixel by .3, the 
 * green by .6, and blue by .1, then add them all together to get the gray
 * pixel value.
 * This function also finds the min and scale of the gray values to use for 
 * the contrast function.
 *
 * @param[in,out] img - an image structure that holds the images data
 * @param[in,out] min - smallest gray pixel value
 * @param[in,out] scale - 255 - (max gray value - min gray value)
 *
 * @par Example
 * @verbatim
   // grayscale(img, min, scale);
   @endverbatim
 *****************************************************************************/
void grayscale(image& img, double& min, double& scale)
{
    int i;
    int j;
    double max;

    min = (.3 * img.redgray[0][0]) + (.6 * img.green[0][0]) 
        + (.1 * img.blue[0][0]);
    max = (.3 * img.redgray[0][0]) + (.6 * img.green[0][0]) 
        + (.1 * img.blue[0][0]);

    // Calculate gray values for each pixel and find min and max value.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            img.redgray[i][j] = (pixel)((.3 * img.redgray[i][j])
                + (.6 * img.green[i][j]) + (.1 * img.blue[i][j]));

            if (img.redgray[i][j] < min)
            {
                min = img.redgray[i][j];
            }
            if (img.redgray[i][j] > max)
            {
                max = img.redgray[i][j];
            }
        }
    }
    scale = 255.0 / (max - min);
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Find the contrast of the input image. It subtracts the min value from the 
 * gray pixel and the multiplies it by the scale.
 *
 * @param[in,out] img - an image structure that holds the images data
 * @param[in]     min - smallest gray pixel value
 * @param[in]     scale - 255 - (max gray value - min gray value)
 *
 * @par Example
 * @verbatim
   // contrast(img, min, scale);
   @endverbatim
 *****************************************************************************/
void contrast(image& img, double min, double scale)
{
    int i;
    int j;

    // Calculate contrast value of each pixel
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            img.redgray[i][j] = (pixel)(scale * ( img.redgray[i][j] - min ));
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Sharpens the inputted image. First it creates temporary arrays for the new
 * color values. It uses sharpenCompute to compute each new color value.
 * Finally, it puts the values of the new arrays in the old arrays.
 *
 * @param[in,out] img - an image structure that holds the images data    
 *
 * @par Example
 * @verbatim
   // sharpen(img);
   @endverbatim
 *****************************************************************************/
void sharpen(image& img)
{
    int i;
    int j;
    pixel** tempred;
    pixel** tempgreen;
    pixel** tempblue;
    
    // Create temporary arrays.
    createArray(tempred, img.rows, img.cols);
    createArray(tempgreen, img.rows, img.cols);
    createArray(tempblue, img.rows, img.cols);

    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            // Set outer pixels to 0.
            if ((i == 0) || (i == img.rows - 1) || (j == 0) || (j == img.cols - 1))
            {
                tempred[i][j] = 0;
                tempgreen[i][j] = 0;
                tempblue[i][j] = 0;
            }
            // Compute the sharpened values of each pixel
            else
            {
                sharpenCompute(img.redgray, tempred, i, j);
                sharpenCompute(img.green, tempgreen, i, j);
                sharpenCompute(img.blue, tempblue, i, j);
            }
        }
    }
    // Set original arrays to temporary arrays.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            img.redgray[i][j] = tempred[i][j];
            img.green[i][j] = tempgreen[i][j];
            img.blue[i][j] = tempblue[i][j];
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Compute the given pixel's sharpened value and store it into a temporary
 * array.
 *
 * @param[in,out] img - an image structure that holds the images data      
 * @param[in,out] arr - temporary array that holds color values
 * @param[in]     i - index for the row of the arrays
 * @param[in]     j - index for the column of the arrays
 *
 * @par Example
 * @verbatim
   // sharpenCompute(img, arr, i, j);
   @endverbatim
 *****************************************************************************/
void sharpenCompute(pixel** img, pixel** arr, int i, int j)
{
    int temp_value;

    temp_value = (5 * img[i][j]) - img[i - 1][j] - img[i + 1][j] 
        - img[i][j - 1] - img[i][j + 1];
    checkNum(temp_value);
    arr[i][j] = temp_value;
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Smooths the inputted image. First it creates temporary arrays for each 
 * color value. Uses the smoothCompute function to find new values for each 
 * pixel and stores them in the temporary arrays. Finally, it stores the new
 * values in the original arrays.
 *
 * @param[in,out] img - an image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // smooth(img);
   @endverbatim
 *****************************************************************************/
void smooth(image& img)
{
    int i;
    int j;
    pixel** tempred;
    pixel** tempgreen;
    pixel** tempblue;

    // Create temporary arrays
    createArray(tempred, img.rows, img.cols);
    createArray(tempgreen, img.rows, img.cols);
    createArray(tempblue, img.rows, img.cols);

    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            // Set outer pixels to 0.
            if ((i == 0) || (i == img.rows - 1) || (j == 0) || (j == img.cols - 1))
            {
                tempred[i][j] = 0;
                tempgreen[i][j] = 0;
                tempblue[i][j] = 0;
            }
            // Call compute function for all pixels.
            else
            {
                smoothCompute(img.redgray, tempred, i, j);
                smoothCompute(img.green, tempgreen, i, j);
                smoothCompute(img.blue, tempblue, i, j);
            }
        }
    }
    // Set original arrays to temporary arrays.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            img.redgray[i][j] = tempred[i][j];
            img.green[i][j] = tempgreen[i][j];
            img.blue[i][j] = tempblue[i][j];
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Takes in two arrays and an index to the row and column of a pixel to
 * compute. It takes the average of the pixel and all of the neighboring 
 * pixels and stores it into the temporary array.
 *
 * @param[in,out] img - an image structure that holds the images data      
 * @param[in,out] arr - temporary array that holds color values
 * @param[in]     i - index for the row of the arrays
 * @param[in]     j - index for the column of the arrays
 * 
 * @par Example
 * @verbatim
   // smoothCompute(img, arr, i, j);
   @endverbatim
 *****************************************************************************/
void smoothCompute(pixel** img, pixel** arr, int i, int j)
{
    int temp_value;

    temp_value = (img[i][j] + img[i - 1][j - 1]
        + img[i - 1][j] + img[i - 1][j + 1]
        + img[i][j - 1] + img[i][j + 1]
        + img[i + 1][j] + img[i + 1][j - 1]
        + img[i + 1][j + 1]) / 9;
    checkNum(temp_value);
    arr[i][j] = temp_value;
}