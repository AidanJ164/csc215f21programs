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