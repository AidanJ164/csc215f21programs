/** ***************************************************************************
 * @file
 *
 * @brief contains functions for allocating and clearing arrays
 *****************************************************************************/
#include "netPBM.h"

/** ***************************************************************************
 * @author Aidan Justice
 * 
 * @par Description 
 * Dynamically allocates a 2d array.
 * 
 * @param[in,out] array - the 2d array being allocated.
 * @param[in]     rows - amount of rows in the array.
 * @param[in]     cols - amount of columns in the array.
 * 
 * @returns true if array was created, false if it failed
 * 
 * @par Example
 * @verbatim
   // createArray( array, rows, cols );
   @endverbatim
 *****************************************************************************/
bool createArray(pixel** &array, int rows, int cols)
{
    int i;
    int j;
    array = nullptr;

    // Create array and check if it allocates.
    array = new (nothrow) pixel* [rows];
    if (array == nullptr)
    {
        cout << "Memory Allocation Error";
        return false;
    }

    // Create column arrays and check if they are valid.
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



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Deletes the inputted 2d array.
 *
 * @param[in,out] array - 2d array to be deleted.
 * @param[in]     rows - amount of rows in the array.
 *
 * @par Example
 * @verbatim
   // clearArray( array, rows );
   @endverbatim
 *****************************************************************************/
void clearArray(pixel** &array, int rows)
{
    int i;
    
    // Clear column arrays.
    for (i = 0; i < rows; i++)
    {
        delete[] array[i];
    }

    // Clear main array.
    delete[] array;
}