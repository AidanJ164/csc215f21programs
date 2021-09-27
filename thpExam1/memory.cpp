#include "netPBM.h"

bool createArray(pixel** &array, int rows, int cols)
{
    int i;
    int j;
    array = nullptr;

    array = new (nothrow) pixel* [rows];
    if (array == nullptr)
    {
        cout << "Memory Allocation Error";
        return false;
    }

    for (i = 0; i < rows; i++)
    {
        array[i] = new (nothrow) pixel[cols];
        if (array[i] == nullptr)
        {
            for (j = 0; j < i; j++)
            {
                delete[] array[j];
            }
            delete[] array;
            cout << "Memory Allocation Error";
            return false;
        }
    }
    return true;
}

void clearArray(pixel** &array, int rows)
{
    int i;

    for (i = 0; i < rows; i++)
    {
        delete[] array[i];
    }

    delete[] array;
}