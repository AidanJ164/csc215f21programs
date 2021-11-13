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
 * @param[in,out] fout - output .ppm image file
 * @param[in]     output - output file name
 * 
 * @returns true if files opened, false if files failed to open
 * 
 * @par Example
 * @verbatim
   // openFile( fin, input, fout, output);
   @endverbatim
 *****************************************************************************/
bool openFile(ifstream& fin, string input, ofstream& fout, string output)
{
    // Open input image in binary and check if it opens.
    fin.open(input, ios::in | ios::binary);
    if (!fin.is_open())
    {
        cout << input << " could not be opened.";
        return false;
    }

    // Open output image in binary and check if it opens.
    fout.open(output, ios::out | ios::trunc | ios::binary);
    if (!fout.is_open())
    {
        cout << output << " could not be opened.";
        fin.close();
        return false;
    }

    return true;
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Close the open files.
 *
 * @param[in] fin - input image file
 * @param[in] fout - output image file
 *
 * @par Example
 * @verbatim
   // closeFile( fin, fout );
   @endverbatim
 *****************************************************************************/
void closeFile(ifstream& fin, ofstream& fout)
{
    fin.close();
    fout.close();
}