/** **************************************************************************
 * @file
 * 
 * @brief Holds the functions that are in the netPBM class.
 ****************************************************************************/
#include "netPBM.h"

/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Constructor for the netPBM class.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // netPBM img;
   @endverbatim
 *****************************************************************************/
netPBM::netPBM()
{
    rows = 0;
    cols = 0;
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Copy constuctor for the netPBM class. Copies in the private data from an 
 * existing netPBM image to a new one.
 *
 * @param[in]  img - existing netPBM image to copy
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // netPBM img2( img );
   @endverbatim
 *****************************************************************************/
netPBM::netPBM( netPBM& img )
{
    int i;
    int j;

    rows = img.rows;
    cols = img.cols;
    comments = img.comments;

    // Allocate new arrays
    redGray = alloc2d( rows, cols );
    green = alloc2d( rows, cols );
    blue = alloc2d( rows, cols );

    // Fill arrays
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = img.redGray[i][j];
            green[i][j] = img.green[i][j];
            blue[i][j] = img.blue[i][j];
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * The deconstructor for the netPBM class. It frees up the dynamically
 * allocated arrays that are stored in the private section.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // Don't call the deconstructor, does it automatically.
   @endverbatim
 *****************************************************************************/
netPBM::~netPBM()
{
    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Dynamically allocates a 2d array of the given size.
 *
 * @param[in]     rows - number of rows in the array.
 * @param[in]     cols - number of columns in the array.
 *
 * @returns a pointer to the array.
 *
 * @par Example
 * @verbatim
   // redGray = alloc2d( rows, cols );
   @endverbatim
 *****************************************************************************/
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


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Using the user's given value, it runs through each pixel adding the value
 * to them. Also checks to see if the new value exceeds 255 or is below 0.
 *
 * @param[in]  value - integer to be added to each pixel.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img.brighten( 25 );
   @endverbatim
 *****************************************************************************/
void netPBM::brighten( int value )
{
    int i;
    int j;
    int temp_value;

    // Calculate brightened value for each pixel.
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            temp_value = ( unsigned long ) redGray[i][j] + value;
            redGray[i][j] = cropRound( temp_value );

            temp_value = ( unsigned long ) green[i][j] + value;
            green[i][j] = cropRound( temp_value );

            temp_value = ( unsigned long ) blue[i][j] + value;
            blue[i][j] = cropRound( temp_value );
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Find the contrast of the input image. It subtracts the min value from the 
 * gray pixel and the multiplies it by the scale.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img.contrast();
   @endverbatim
 *****************************************************************************/
void netPBM::contrast()
{
    int i;
    int j;
    double min;
    double scale;

    // Find the scale and the minimum values.
    findScale( scale, min );

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = ( pixel ) ( scale * ( redGray[i][j] - min ) );
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 * 
 * @par Description 
 * Ensures the given value is not larger than 255 or smaller than 0. If the
 * value is larger than 255, it crops it to 255 and if it is smaller than 0,
 * it crops it to 0.
 * 
 * @param[in]  value - double to check.
 * 
 * @returns value typecasted to a pixel.
 * 
 * @par Example
 * @verbatim
   // new_value = cropRound( value );
   @endverbatim
 *****************************************************************************/
pixel netPBM::cropRound( double value )
{
    if ( value > 255 )
    {
        value = 255;
    }
    else if ( value < 0 )
    {
        value = 0;
    }

    return ( pixel ) value;
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Finds the minimum and the scale used in the contrast function. Scale is 
 * found by taking the maximum value minus the minimum value and then divinding
 * that from 255.
 *
 * @param[in,out] scale - 255 / ( max - min )
 * @param[in,out] min - minimum grayscale value in the img.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // findScale( scale, min );
   @endverbatim
 *****************************************************************************/
void netPBM::findScale( double& scale, double& min )
{
    int i;
    int j;
    double max;

    min = redGray[0][0];
    max = redGray[0][0];

    // Find min and max values.
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            if ( redGray[i][j] < min )
            {
                min = redGray[i][j];
            }
            if ( redGray[i][j] > max )
            {
                max = redGray[i][j];
            }
        }
    }

    // Calculate scale.
    scale = 255.0 / ( max - min );
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Flips the images along the x-axis. It goes from column to column, swapping
 * the two opposite rows.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img.flipx();
   @endverbatim
 *****************************************************************************/
void netPBM::flipx()
{
    int i;
    int j;
    int mid;
    
    // Find midpoint of the rows.
    mid = rows / 2;

    // Run through array, swapping opposite rows, making sure to only go half way.
    for ( j = 0; j < cols; j++ )
    {
        for ( i = 0; i < mid; i++ )
        {
            swap( redGray[i][j], redGray[rows - 1 - i][j] );
            swap( green[i][j], green[rows - 1 - i][j] );
            swap( blue[i][j], blue[rows - 1 - i][j] );
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Flips the image along the y-axis. It goes from row to row, swapping the 
 * opposite columns.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img.flipy();
   @endverbatim
 *****************************************************************************/
void netPBM::flipy()
{
    int i;
    int j;
    int mid;

    // Find midpoint of the columns.
    mid = cols / 2;

    // Run through array, swapping the opposite columns.
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < mid; j++ )
        {
            swap( redGray[i][j], redGray[i][cols - 1 - j] );
            swap( green[i][j], green[i][cols - 1 - j] );
            swap( blue[i][j], blue[i][cols - 1 - j] );
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Deletes the given array.
 *
 * @param[in,out] arr - pointer to the array to be deleted.
 * @param[in]     rows - amount of rows in the array.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // free2d( redGray, rows );
   @endverbatim
 *****************************************************************************/
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


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Converts the image to grayscale. It multiplies the red pixel by .3, the 
 * green by .6, and blue by .1, then add them all together to get the gray
 * pixel value.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img.grayscale();
   @endverbatim
 *****************************************************************************/
void netPBM::grayscale()
{
    int i;
    int j;

    // Calculate gray values for each pixel.
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = ( pixel ) ( ( .3 * redGray[i][j] ) + ( .6 * green[i][j] )
                + ( .1 * blue[i][j] ) );
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Using the user's given row, col, height, and width, it makes a smaller
 * image that is a snippit of the original.
 *
 * @param[in]  row - row to start from
 * @param[in]  col - column to start from
 * @param[in]  height - height of the icon
 * @param[in]  width - width of the the icon
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img.icon( 50, 50, 300, 350 );
   @endverbatim
 *****************************************************************************/
void netPBM::icon( int row, int col, int height, int width )
{
    int i;
    int j;
    netPBM temp;

    // Check to see if height and width are too big.
    if ( height > rows )
    {
        height = rows;
        row = 0;
    }
    if ( width > cols )
    {
        width = cols;
        col = 0;
    }

    // Get a temporary image to store old values.
    temp = *this;
    
    // Free the arrays of the original image.
    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );

    // Set new dimensions.
    rows = height;
    cols = width;

    // Allocate new arrays.
    redGray = alloc2d( rows, cols );
    green = alloc2d( rows, cols );
    blue = alloc2d( rows, cols );

    // Get an appropriate starting point.
    if ( ( height + row ) > temp.rows )
    {
        row = temp.rows - height;
    }
    if ( ( width + col ) > temp.cols )
    {
        col = temp.cols - width;
    }

    // Run through the new image, setting the pixels equal to the original.
    for ( i = 0; i < height; i++ )
    {
        for ( j = 0; j < width; j++ )
        {
            redGray[i][j] = temp.redGray[row + i][col + j];
            green[i][j] = temp.green[row + i][col + j];
            blue[i][j] = temp.blue[row + i][col + j];
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Negates each pixel stored in the image. It takes each pixel and subtracts it
 * from the max color value, 255.

 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img.negate();
   @endverbatim
 *****************************************************************************/
void netPBM::negate()
{
    int i;
    int j;

    // Negate each pixel.
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = 255 - redGray[i][j];
            green[i][j] = 255 - green[i][j];
            blue[i][j] = 255 - blue[i][j];
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Overload the = operator to easily set to netPBM images equal.
 *
 * @param[in]  img - existing netPBM image.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img = img2;
   @endverbatim
 *****************************************************************************/
void netPBM::operator=( netPBM img )
{
    int i;
    int j;

    // Free existing arrays
    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );

    // Get private data.
    rows = img.rows;
    cols = img.cols;
    comments = img.comments;

    // Allocate new arrays
    redGray = alloc2d( rows, cols );
    green = alloc2d( rows, cols );
    blue = alloc2d( rows, cols );

    // Fill arrays
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = img.redGray[i][j];
            green[i][j] = img.green[i][j];
            blue[i][j] = img.blue[i][j];
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Overload the == operator to check if two netPBM images are equal.
 *
 * @param[in]  img - netPBM to compare with.
 *
 * @returns true if equal, false otherwise.
 *
 * @par Example
 * @verbatim
   // if ( img == img2 )
   @endverbatim
 *****************************************************************************/
bool netPBM::operator==( netPBM img )
{
    int i;
    int j;

    // Check to see if rows and cols are the same.
    if ( (rows != img.rows) || (cols != img.cols) )
    {
        return false;
    }

    // Run through arrays to see if each pixel is the same.
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            if ( ( redGray[i][j] != img.redGray[i][j] ) ||
                 ( green[i][j] != img.green[i][j] ) ||
                 ( blue [i][j] != img.blue[i][j] ) )
            {
                return false;
            }
        }
    }

    return true;
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Overload the != operator to easily check if two netPBM images are not 
 * equal.
 *
 * @param[in]  img - netPBM image to compare.
 *
 * @returns true if successful and false otherwise.
 *
 * @par Example
 * @verbatim
   // if ( img != img2 )
   @endverbatim
 *****************************************************************************/
bool netPBM::operator!=( netPBM img )
{
    return !( *this == img );
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Output the header of the .ppm or .pgm image to the file.
 *
 * @param[in,out]  fout - output file to write to.    
 * @param[in]      magicNum - magic number to write to file.
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img.outputHeader( fout, magicNum );
   @endverbatim
 *****************************************************************************/
void netPBM::outputHeader( ofstream& fout, string magicNum )
{
    fout << magicNum << '\n';
    fout << comments;
    fout << cols << " " << rows << '\n' << "255" << '\n';
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Read in the data from a P3 .ppm image.
 *
 * @param[in,out]  fin - input file to read from
 *
 * @returns nothing
 *
 * @par Example
 * @verbatim
   // img.readAcii( fin );
   @endverbatim
 *****************************************************************************/
void netPBM::readAscii( ifstream& fin )
{
    int i;
    int j;
    int temp_value;

    // Read ascii values into the image arrays.
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            fin >> temp_value;
            redGray[i][j] = temp_value;

            fin >> temp_value;
            green[i][j] = temp_value;

            fin >> temp_value;
            blue[i][j] = temp_value;
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Read in the data from a P6 .ppm image.
 *
 * @param[in,out]  fin - input file to read from
 *
 * @par Example
 * @verbatim
   // img.readBinary( fin );
   @endverbatim
 *****************************************************************************/
void netPBM::readBinary( ifstream& fin )
{
    int i;
    int j;

    // Read binary values into the image arrays.
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            fin.read( ( char* ) &redGray[i][j], sizeof( pixel ) );
            fin.read( ( char* ) &green[i][j], sizeof( pixel ) );
            fin.read( ( char* ) &blue[i][j], sizeof( pixel ) );
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Read in the header from any .ppm image.
 *
 * @param[in,out]  fin - input file to read from.
 *
 * @par Example
 * @verbatim
   // img.readHeader( fin );
   @endverbatim
 *****************************************************************************/
void netPBM::readHeader( ifstream& fin )
{
    string garbage;
    string comment;

    while ( fin.peek() == '#' )
    {
        getline( fin, comment );
        comments += comment + '\n';
    }

    fin >> cols;
    fin >> rows;

    fin >> garbage;
    fin.ignore();
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Reads in the image data from a .ppm image.
 *
 * @param[in]    filename - name of the file to be opened and read in.
 *
 * @returns true if successful and false otherwise.
 *
 * @par Example
 * @verbatim
   // if ( !img.readInImage( filename ) )
   @endverbatim
 *****************************************************************************/
bool netPBM::readInImage( string filename )
{
    ifstream fin;
    string magicNum;
    string garbage;

    // Open file and check for success.
    fin.open( filename, ios::in | ios::binary );
    if ( !fin.is_open() )
    {
        return false;
    }

    // Read in magic number and validate it is appropriate.
    fin >> magicNum;
    if ((magicNum != "P3") && (magicNum != "P6"))
    {
        return false;
    }
    fin.ignore();

    readHeader( fin );

    // Allocate arrays and check for success.
    redGray = alloc2d( rows, cols );
    if (redGray == nullptr)
    {
        return false;
    }
    green = alloc2d( rows, cols );
    if ( green == nullptr )
    {
        return false;
    }
    blue = alloc2d( rows, cols );
    if ( blue == nullptr )
    {
        return false;
    }

    // Read in image data.
    if ( magicNum == "P3" )
    {
        readAscii( fin );
    }
    else
    {
        readBinary( fin );
    }

    fin.close();
    return true;
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Set all blue values equal to 0.
 *
 * @par Example
 * @verbatim
   // img.removeBlue();
   @endverbatim
 *****************************************************************************/
void netPBM::removeBlue()
{
    int i;
    int j;

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            blue[i][j] = 0;
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Set all green values equal to 0.
 *
 * @par Example
 * @verbatim
   // img.removeGreen();
   @endverbatim
 *****************************************************************************/
void netPBM::removeGreen()
{
    int i;
    int j;

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            green[i][j] = 0;
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Set all red values equal to 0.
 *
 * @par Example
 * @verbatim
   // img.removeRed();
   @endverbatim
 *****************************************************************************/
void netPBM::removeRed()
{
    int i;
    int j;

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = 0;
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Rotate the image clockwise 90 degrees.
 *
 * @par Example
 * @verbatim
   // img.rotateCW();
   @endverbatim
 *****************************************************************************/
void netPBM::rotateCW()
{
    int i;
    int j;
    netPBM img;

    // Create temporary image.
    img = *this;

    // Free old arrays.
    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );

    swap( rows, cols );

    // Allocate new arrays.
    redGray = alloc2d( rows, cols );
    green = alloc2d( rows, cols );
    blue = alloc2d( rows, cols );

    // Fill new arrays.
    for ( i = 0; i < img.rows; i++ )
    {
        for ( j = 0; j < img.cols; j++ )
        {
            redGray[j][cols - 1 - i] = img.redGray[i][j];
            green[j][cols - 1 - i] = img.green[i][j];
            blue[j][cols - 1 - i] = img.blue[i][j];
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Rotate the image counterclockwise 90 degrees.
 *
 * @par Example
 * @verbatim
   // img.rotateCCW();
   @endverbatim
 *****************************************************************************/
void netPBM::rotateCCW()
{
    int i;
    int j;
    netPBM img;

    // Create temporary image.
    img = *this;

    // Free old arrays.
    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );

    swap( rows, cols );

    // Allocate new arrays.
    redGray = alloc2d( rows, cols );
    green = alloc2d( rows, cols );
    blue = alloc2d( rows, cols );

    // Fill new arrays.
    for ( i = 0; i < img.rows; i++ )
    {
        for ( j = 0; j < img.cols; j++ )
        {
            redGray[rows - 1 - j][i] = img.redGray[i][j];
            green[rows - 1 - j][i] = img.green[i][j];
            blue[rows - 1 - j][i] = img.blue[i][j];
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Sharpens the image. Creates a temporary image and sets it equal to the 
 * original. Then computes each pixels sharpened values.
 *
 * @par Example
 * @verbatim
   // img.sharpen();
   @endverbatim
 *****************************************************************************/
void netPBM::sharpen()
{
    int i;
    int j;
    int temp_value;
    netPBM temp;

    temp = *this;

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            // Set border pixels to 0;
            if ( ( i == 0 ) || ( i == rows - 1 ) || ( j == 0 ) || ( j == cols - 1 ) )
            {
                redGray[i][j] = 0;
                green[i][j] = 0;
                blue[i][j] = 0;
            }
            // Compute sharpened values for others.
            else 
            {
                temp_value = ( 5 * temp.redGray[i][j] ) - temp.redGray[i - 1][j]
                    - temp.redGray[i + 1][j] - temp.redGray[i][j - 1] 
                    - temp.redGray[i][j + 1];
                redGray[i][j] = cropRound( temp_value );

                temp_value = ( 5 * temp.green[i][j] ) - temp.green[i - 1][j]
                    - temp.green[i + 1][j] - temp.green[i][j - 1]
                    - temp.green[i][j + 1];
                green[i][j] = cropRound( temp_value );
                
                temp_value = ( 5 * temp.blue[i][j] ) - temp.blue[i - 1][j]
                    - temp.blue[i + 1][j] - temp.blue[i][j - 1]
                    - temp.blue[i][j + 1];
                blue[i][j] = cropRound( temp_value );
            }
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 *
 *
 * @param[in]     var1 - a description of variable one.
 * @param[out]    var2 - a description of variable two.
 * @param[in,out] var3 - a description of variable three.
 *
 * @returns true if successful and false otherwise.
 *
 * @par Example
 * @verbatim
   // provide a small snippet on calling your function and the results
   @endverbatim
 *****************************************************************************/
void netPBM::smooth()
{
    int i;
    int j;
    int temp_value;
    netPBM temp;

    temp = *this;

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            // Set borders to 0.
            if ( ( i == 0 ) || ( i == rows - 1 ) || ( j == 0 ) || ( j == cols - 1 ) )
            {
                redGray[i][j] = 0;
                green[i][j] = 0;
                blue[i][j] = 0;
            }
            // Calculate smoothed values for each pixel.
            else
            {
                temp_value = ( temp.redGray[i][j] + temp.redGray[i - 1][j - 1]
                    + temp.redGray[i - 1][j] + temp.redGray[i - 1][j + 1]
                    + temp.redGray[i][j - 1] + temp.redGray[i][j + 1]
                    + temp.redGray[i + 1][j] + temp.redGray[i + 1][j - 1]
                    + temp.redGray[i + 1][j + 1] ) / 9;
                redGray[i][j] = cropRound( temp_value );

                temp_value = ( temp.green[i][j] + temp.green[i - 1][j - 1]
                    + temp.green[i - 1][j] + temp.green[i - 1][j + 1]
                    + temp.green[i][j - 1] + temp.green[i][j + 1]
                    + temp.green[i + 1][j] + temp.green[i + 1][j - 1]
                    + temp.green[i + 1][j + 1] ) / 9;
                green[i][j] = cropRound( temp_value );

                temp_value = ( temp.blue[i][j] + temp.blue[i - 1][j - 1]
                    + temp.blue[i - 1][j] + temp.blue[i - 1][j + 1]
                    + temp.blue[i][j - 1] + temp.blue[i][j + 1]
                    + temp.blue[i + 1][j] + temp.blue[i + 1][j - 1]
                    + temp.blue[i + 1][j + 1] ) / 9;
                blue[i][j] = cropRound( temp_value );
            }
        }
    }
}

bool netPBM::writeOutGrayImage( string filename, outputType out )
{
    int i;
    int j;
    ofstream fout;

    fout.open( filename, ios::out | ios::trunc | ios::binary );
    if ( !fout.is_open() )
    {
        return false;
    }

    if ( out == ASCII )
    {
        outputHeader( fout, "P2" );

        for ( i = 0; i < rows; i++ )
        {
            for ( j = 0; j < cols; j++ )
            {
                fout << ( int ) redGray[i][j] << endl;
            }
        }
    }
    else
    {
        outputHeader( fout, "P5" );

        for ( i = 0; i < rows; i++ )
        {
            for ( j = 0; j < cols; j++ )
            {
                fout.write( ( char* ) &redGray[i][j], sizeof( pixel ) );
            }
        }
    }

    fout.close();
    return true;
}

bool netPBM::writeOutImage( string filename, outputType out )
{
    int i;
    int j;
    ofstream fout;

    fout.open( filename, ios::out | ios::trunc | ios::binary );
    if ( !fout.is_open() )
    {
        return false;
    }

    if (out == ASCII)
    {
        outputHeader( fout, "P3" );

        // Write each color value to file in ascii.
        for ( i = 0; i < rows; i++ )
        {
            for ( j = 0; j < cols; j++ )
            {
                fout << ( int )redGray[i][j] << " ";
                fout << ( int )green[i][j] << " ";
                fout << ( int )blue[i][j] << endl;
            }
        }
    }
    else
    {
        outputHeader( fout, "P6" );

        // Write each color value to file in binary.
        for ( i = 0; i < rows; i++ )
        {
            for ( j = 0; j < cols; j++ )
            {
                fout.write( ( char* ) &redGray[i][j], sizeof( pixel ) );
                fout.write( ( char* ) &green[i][j], sizeof( pixel ) );
                fout.write( ( char* ) &blue[i][j], sizeof( pixel ) );
            }
        }
    }

    fout.close();
    return true;
}