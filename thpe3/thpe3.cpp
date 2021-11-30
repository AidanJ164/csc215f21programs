/** **************************************************************************
 * @file
 * 
 * @brief File that holds main and any extra functions
 ****************************************************************************/
 /** **************************************************************************
  * @mainpage thpe3
  *
  * @section course_section Course Information
  *
  * @author Aidan Justice
  *
  * @par Professor:
  *         Prof. Roger Schrader
  *
  * @par Course:
  *         CSC215 - M001 - Programming Techniques
  *
  * @par Location:
  *         McLaury - 207
  *
  * @date Due December 3, 2021
  *
  * @section program_section Program Information
  *
  * @details
  * This program is designed to flood fill a region of an ppm image. 
  * Using the user's given starting point and color values, the program 
  * recursively fills the region that it started in. It fills a pixel and 
  * moves to an adjacent one until it finds either the same color, a different
  * color, or the edge. After it finds that base case, it will try the next 
  * adjacent pixel and will keep going until it returns back to the starting
  * pixel.
  *
  * @section compile_section Compiling and Usage
  *
  * @par Compiling Instructions:
  *      Must change the Stack Reserve Size and Stack Commit Size to 
  * 4 billion (4000000000). This can be found by right clicking the project
  * name, selecting Properties, expanding the Linker options, and going to 
  * System. 
  *
  * @par Usage:
    @verbatim
    c:\> thpe3.exe imageFile row col redValue greenValue blueValue
             imageFile - image to be edited
             row, col - starting pixel value to fill
             redValue - red value to fill area with
             greenValue - green value to fill area with
             blueValue - blue value to fill area with
    @endverbatim
  *
  * @section todo_bugs_modification_section Todo, Bugs, and Modifications
  *
  * @par Modifications and Development Timeline:
  * This is a link to gitlab's commit as an example. To view <a target="_blank"
  * href="https://gitlab.cse.sdsmt.edu/101066736/csc215f21programs/-/commits/master">click here.</a>
  *
  *****************************************************************************/

#include "netPBM.h"

/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Reads in a .ppm image and using user inputted values, fills in a region
 * of the image.
 *
 * @param[in] argc - amount of arguments given
 * @param[in] argv - array that holds the user given arguments
 *
 * @par Example
 * @verbatim
   // thpe3.exe imageFile row col redValue greenValue blueValue
   @endverbatim
 *****************************************************************************/
int main( int argc, char** argv )
{
    fstream file;
    image img;
    int row;
    int col;
    int newColor[3];
    int oldColor[3];

    if ( argc != 7 )
    {
        cout << "Usage: thpe3.exe imageFile row col redValue greenValue blueValue"
            << endl;
        cout << "imageFile  - image to be edited" << endl;
        cout << "row, col   - starting pixel value to fill" << endl;
        cout << "redValue   - red value to fill area with" << endl;
        cout << "greenValue - green value to fill area with" << endl;
        cout << "blueValue  - blue value to fill area with" << endl;
        return 0;
    }

    if ( !openFile( file, argv[1] ) )
    {
        return 0;
    }

    // Assign arguments to variables.
    row = atoi( argv[2] );
    col = atoi( argv[3] );
    newColor[RED] = atoi( argv[4] );
    newColor[GREEN] = atoi( argv[5] );
    newColor[BLUE] = atoi( argv[6] );

    // Read in image data
    if ( !( file >> img ) )
    {
        return 0;
    }

    // Fill in Area
    if ( !getStartColor( img, oldColor, row, col ) )
    {
        return 0;
    }

    fill( img, row, col, newColor, oldColor );

    file.seekp( ios::beg, 0 );
    outputHeader( img, file );
    
    // Write out image data
    if ( img.magicNumber == "P3" )
    {
        outputAscii( file, img );
    }
    else
    {
        outputBinary( file, img );
    }

    // Clean up the arrays and close the image
    clearArray( img.redgray, img.rows );
    clearArray( img.green, img.rows );
    clearArray( img.blue, img.rows );
    closeFile( file );

    return 0;
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Reads the header of the inputted image. Reads the magic number, image
 * comments, and the dimensions of the image.
 *
 * @param[in,out] img - an image structure that holds the images data.
 * @param[in,out] fin - the input .ppm image.
 *
 * @par Example
 * @verbatim
   // readHeader(img, fin);
   @endverbatim
 *****************************************************************************/
void readHeader( image& img, fstream& fin )
{
    string garbage;
    string comment;

    // Look for comments
    while ( fin.peek() == '#' )
    {
        getline(fin, comment);
        img.comment += comment + '\n';
    }

    // Read in columns and rows
    fin >> img.cols;
    fin >> img.rows;

    // Disregard max color value
    fin >> garbage;
    fin.ignore();
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Output the header to the image file.
 *
 * @param[in]     img - image structure that holds the images data
 * @param[in,out] fout - output image file
 *
 * @par Example
 * @verbatim
   // outputHeader(img, fout);
   @endverbatim
 *****************************************************************************/
void outputHeader( image img, fstream& fout )
{
    fout << img.magicNumber << '\n';
    fout << img.comment;
    fout << img.cols << " " << img.rows << '\n' << "255" << '\n';
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Read in the data from a P3 .ppm image.
 *
 * @param[in,out] fin - inputted image file
 * @param[in,out] img - image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // readAscii(fin, img);
   @endverbatim
 *****************************************************************************/
void readAscii( fstream& fin, image& img )
{
    int i;
    int j;
    int temp_value;

    // Read ascii values into the image arrays.
    for ( i = 0; i < img.rows; i++ )
    {
        for ( j = 0; j < img.cols; j++ )
        {
            fin >> temp_value;
            img.redgray[i][j] = temp_value;

            fin >> temp_value;
            img.green[i][j] = temp_value;

            fin >> temp_value;
            img.blue[i][j] = temp_value;
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Read in the data from a P6 .ppm image.
 *
 * @param[in,out] fin - inputted image file
 * @param[in,out] img - image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // readBinary(fin, img);
   @endverbatim
 *****************************************************************************/
void readBinary( fstream& fin, image& img )
{
    int i;
    int j;

    // Read binary values into the image arrays.
    for ( i = 0; i < img.rows; i++ )
    {
        for ( j = 0; j < img.cols; j++ )
        {
            fin.read( ( char* ) &img.redgray[i][j], sizeof( pixel ) );
            fin.read( ( char* ) &img.green[i][j], sizeof( pixel ) );
            fin.read( ( char* ) &img.blue[i][j], sizeof( pixel ) );
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Output the ascii data to the new image file.
 *
 * @param[in,out] fout - output image file
 * @param[in]     img - image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // outputAscii(fout, img);
   @endverbatim
 *****************************************************************************/
void outputAscii( fstream& fout, image img )
{
    int i;
    int j;

    // Write each color value to file in ascii.
    for ( i = 0; i < img.rows; i++ )
    {
        for ( j = 0; j < img.cols; j++ )
        {
            fout << ( int )img.redgray[i][j] << " ";
            fout << ( int )img.green[i][j] << " ";
            fout << ( int )img.blue[i][j] << endl;
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Output the binary data to the new image file.
 *
 * @param[in,out] fout - output image file
 * @param[in]     img - image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // outputBinary(fout, img);
   @endverbatim
 *****************************************************************************/
void outputBinary( fstream& fout, image img )
{
    int i;
    int j;

    // Write each color value to file in binary.
    for ( i = 0; i < img.rows; i++ ) 
    {
        for ( j = 0; j < img.cols; j++ )
        {
            fout.write( ( char* ) &img.redgray[i][j], sizeof( pixel ) );
            fout.write( ( char* ) &img.green[i][j], sizeof( pixel ) );
            fout.write( ( char* ) &img.blue[i][j], sizeof( pixel ) );
        }
    }
}

/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * If the given pixel is not within the range of the image, not the original
 * color of the starting pixel, or is the new color, it changes the given
 * pixel value to the user's new color. After it changes the color, it calls
 * this function on the adjacent pixels.
 *
 * @param[in,out] img - image structure that holds all the image's data
 * @param[in] row - row of the pixel to change
 * @param[in] col - column of the pixel to change
 * @param[in] newColor - array that holds the 3 new color values
 * @param[in] oldColor - array that holds the 3 old color values
 *
 * @par Example
 * @verbatim
   // fill( img, row, col, newColor, oldColor );
   @endverbatim
 *****************************************************************************/
void fill( image& img, int row, int col, int newColor[], int oldColor[] )
{
    // Check the base case.
    if (( row < 0 ) || ( row >= img.rows ) || 
        ( col < 0 ) || ( col >= img.cols ) ||
        ( img.redgray[row][col] != oldColor[RED] ) ||
        ( img.green[row][col] != oldColor[GREEN] ) ||
        ( img.blue[row][col] != oldColor[BLUE] ) ||
        (( img.redgray[row][col] == newColor[RED] ) &&
        ( img.green[row][col] == newColor[GREEN] ) &&
        ( img.blue[row][col] == newColor[BLUE] ) ) )
    {
        return;
    }

    // Assign new pixel values
    img.redgray[row][col] = newColor[RED];
    img.green[row][col] = newColor[GREEN];
    img.blue[row][col] = newColor[BLUE];

    // Fill adjacent pixels
    fill( img, row - 1, col, newColor, oldColor );
    fill( img, row, col + 1, newColor, oldColor );
    fill( img, row + 1, col, newColor, oldColor );
    fill( img, row, col - 1, newColor, oldColor );
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Get the color of the user's inputted starting pixel.
 *
 * @param[in] img - image structure that holds the images data
 * @param[in,out] oldColor - array that stores the starting color values
 * @param[in] row - user inputted starting row
 * @param[in] col - user inputted starting col
 *
 * @par Example
 * @verbatim
   // getStartColor( img, oldColor, row, col );
   @endverbatim
 *****************************************************************************/
bool getStartColor( image img, int oldColor[], int row, int col )
{
    // If pixel is out of scope, return false
    if ( (row < 0 ) || ( row > img.rows ) || ( col < 0 ) || ( col > img.cols ) )
    {
        return false;
    }

    // Get starting color values
    oldColor[RED] = img.redgray[row][col];
    oldColor[GREEN] = img.green[row][col];
    oldColor[BLUE] = img.blue[row][col];

    return true;
}

/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Read in inputted file data and store it into an image structure.
 *
 * @param[in,out] file - .ppm image
 * @param[in,out] img - image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // file >> img;
   @endverbatim
 *****************************************************************************/
bool operator>>( fstream& file, image& img )
{
    // Check for valid magic number.
    file >> img.magicNumber;
    if ( ( img.magicNumber != "P3" ) && ( img.magicNumber != "P6" ) )
    {
        cout << "Invalid Magic Numbers" << endl
            << "Valid Magic Numbers: P3 and P6";
        closeFile( file );
        return false;
    }

    file.ignore();
    readHeader( img, file );

    // Create pixel arrays
    if ( !createArray( img.redgray, img.rows, img.cols ) )
    {
        return false;
    }
    if ( !createArray( img.green, img.rows, img.cols ) )
    {
        return false;
    }
    if ( !createArray( img.blue, img.rows, img.cols ) )
    {
        return false;
    }

    // Read in Ascii Values
    if ( img.magicNumber == "P3" )
    {
        readAscii( file, img );
    }

    //Read in Binary Values from image
    else
    {
        readBinary( file, img );
    }

    return true;
}
