#include "netPBM.h"

void brighten(image& img, int value)
{
    int i;
    int j;
    int temp_value;

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

void negate(image& img)
{
    int i;
    int j;

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

void grayscale(image& img, double& min, double& scale)
{
    int i;
    int j;
    double max;

    min = img.redgray[0][0] = (.3 * img.redgray[0][0])
        + (.6 * img.green[0][0]) + (.1 * img.blue[0][0]);
    max = img.redgray[0][0] = (.3 * img.redgray[0][0])
        + (.6 * img.green[0][0]) + (.1 * img.blue[0][0]);

    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            img.redgray[i][j] = (.3 * img.redgray[i][j])
                + (.6 * img.green[i][j]) + (.1 * img.blue[i][j]);

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

void contrast(image& img, double min, double scale)
{
    int i;
    int j;

    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            img.redgray[i][j] = scale * ( img.redgray[i][j] - min );
        }
    }
}

void sharpen(image& img)
{
    int i;
    int j;
    int temp_value;
    pixel** tempred;
    pixel** tempgreen;
    pixel** tempblue;

    createArray(tempred, img.rows, img.cols);
    createArray(tempgreen, img.rows, img.cols);
    createArray(tempblue, img.rows, img.cols);

    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            if ((i == 0) || (i == img.rows - 1) || (j == 0) || (j == img.cols - 1))
            {
                tempred[i][j] = 0;
                tempgreen[i][j] = 0;
                tempblue[i][j] = 0;
            }
            else
            {
                temp_value = (5 * img.redgray[i][j]) - img.redgray[i - 1][j]
                    - img.redgray[i + 1][j] - img.redgray[i][j - 1]
                    - img.redgray[i][j + 1];
                checkNum(temp_value);
                tempred[i][j] = temp_value;

                temp_value = (5 * img.green[i][j]) - img.green[i - 1][j]
                    - img.green[i + 1][j] - img.green[i][j - 1]
                    - img.green[i][j + 1];
                checkNum(temp_value);
                tempgreen[i][j] = temp_value;

                temp_value = (5 * img.blue[i][j]) - img.blue[i - 1][j]
                    - img.blue[i + 1][j] - img.blue[i][j - 1]
                    - img.blue[i][j + 1];
                checkNum(temp_value);
                tempblue[i][j] = temp_value;
            }
        }
    }
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

void smooth(image& img)
{
    int i;
    int j;
    int temp_value;
    pixel** tempred;
    pixel** tempgreen;
    pixel** tempblue;

    createArray(tempred, img.rows, img.cols);
    createArray(tempgreen, img.rows, img.cols);
    createArray(tempblue, img.rows, img.cols);

    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            if ((i == 0) || (i == img.rows - 1) || (j == 0) || (j == img.cols - 1))
            {
                tempred[i][j] = 0;
                tempgreen[i][j] = 0;
                tempblue[i][j] = 0;
            }
            else
            {
                smoothCompute(img.redgray, tempred, i, j);
                smoothCompute(img.green, tempgreen, i, j);
                smoothCompute(img.blue, tempblue, i, j);
            }
        }
    }
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