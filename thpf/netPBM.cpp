#include "netPBM.h"

netPBM::netPBM()
{

}
netPBM::~netPBM()
{
    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );
}

pixel** netPBM::alloc2d(int rows, int cols)
{
    int i;
    pixel** arr = nullptr;

    // Create array and check for success.
    arr = new (nothrow) pixel* [rows];
    if (arr == nullptr)
    {
        return nullptr;
    }

    // Create each column for the array and check for success.
    for ( i = 0; i < rows; i++ )
    {
        arr[i] = new ( nothrow ) pixel[cols];
        if (arr[i] == nullptr)
        {
            free2d( arr, i );
            return nullptr;
        }
    }

    return arr;
}

void netPBM::free2d( pixel** &arr, int rows )
{
    int i;

    // Delete columns
    for ( i = 0; i < rows; i++ )
    {
        delete[] arr[i];
    }

    // Delete the array
    delete[] arr;
}
    
    

