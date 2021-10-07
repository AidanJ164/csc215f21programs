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