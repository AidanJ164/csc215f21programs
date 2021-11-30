/** ***************************************************************************
 * @file
 *
 * @brief contains functions that open and close image files.
 *****************************************************************************/
#include "netPBM.h"

/** ***************************************************************************
 * @author Aidan Justice
 * 
 * @par Description 
 * Open input and output .ppm or .pgm images in binary mode.
 * 
 * @param[in,out] fin - inputted .ppm image file
 * @param[in]     input - input file name
 * 
 * @returns true if files opened, false if files failed to open
 * 
 * @par Example
 * @verbatim
   // openFile( fin, input, fout, output);
   @endverbatim
 *****************************************************************************/
bool openFile( fstream& fin, string input )
{
    // Open input image in binary and check if it opens.
    fin.open( input, ios::in | ios::out | ios::binary | ios::ate );
    if ( !fin.is_open() )
    {
        cout << input << " could not be opened.";
        return false;
    }

    // Seek to the beginning of the file
    fin.seekg( ios::beg, 0 );

    return true;
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Close the open files.
 *
 * @param[in] fin - input image file
 *
 * @par Example
 * @verbatim
   // closeFile( fin, fout );
   @endverbatim
 *****************************************************************************/
void closeFile( fstream& fin )
{
    fin.close();
}