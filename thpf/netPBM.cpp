#include "netPBM.h"

netPBM::netPBM()
{
    rows = 0;
    cols = 0;
}

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

void netPBM::contrast()
{
    int i;
    int j;
    double min;
    double scale;

    findScale( scale, min );

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = ( pixel ) ( scale * ( redGray[i][j] - min ) );
        }
    }
    cout << "pingus" << endl;
}

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

void netPBM::findScale( double& scale, double& min )
{
    int i;
    int j;
    double max;

    min = redGray[0][0];
    max = redGray[0][0];

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

    scale = 255.0 / ( max - min );
}

void netPBM::flipx()
{
    int i;
    int j;
    int mid;

    mid = rows / 2;

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

void netPBM::flipy()
{
    int i;
    int j;
    int mid;

    mid = cols / 2;

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

void netPBM::grayscale()
{
    int i;
    int j;

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = ( pixel ) ( ( .3 * redGray[i][j] ) + ( .6 * green[i][j] )
                + ( .1 * blue[i][j] ) );
        }
    }
}


void netPBM::icon( int row, int col, int height, int width )
{
    int i;
    int j;
    netPBM temp;

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

    temp = *this;
    
    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );

    rows = height;
    cols = width;

    redGray = alloc2d( rows, cols );
    green = alloc2d( rows, cols );
    blue = alloc2d( rows, cols );

    if ( ( height + row ) > temp.rows )
    {
        row = temp.rows - height;
    }
    if ( ( width + col ) > temp.cols )
    {
        col = temp.cols - width;
    }

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

void netPBM::operator=( netPBM img )
{
    int i;
    int j;

    // Free existing arrays
    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );

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

bool netPBM::operator==( netPBM img2 )
{
    int i;
    int j;

    if ( (rows != img2.rows) || (cols != img2.cols) )
    {
        return false;
    }
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            if ( ( redGray[i][j] != img2.redGray[i][j] ) ||
                 ( green[i][j] != img2.green[i][j] ) ||
                 ( blue [i][j] != img2.blue[i][j] ) )
            {
                return false;
            }
        }
    }

    return true;
}

bool netPBM::operator!=( netPBM img )
{
    return !( *this == img );
}

void netPBM::outputHeader( ofstream& fout, string magicNum )
{
    fout << magicNum << '\n';
    fout << comments;
    fout << cols << " " << rows << '\n' << "255" << '\n';
}

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

bool netPBM::readInImage( string filename )
{
    ifstream fin;
    string magicNum;
    string garbage;

    fin.open( filename, ios::in | ios::binary );
    if ( !fin.is_open() )
    {
        return false;
    }

    fin >> magicNum;
    if ((magicNum != "P3") && (magicNum != "P6"))
    {
        return false;
    }
    fin.ignore();

    readHeader( fin );

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

void netPBM::removeColor( string color )
{
}

void netPBM::rotateCW()
{
    int i;
    int j;
    netPBM img;

    img = *this;

    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );

    swap( rows, cols );

    redGray = alloc2d( rows, cols );
    green = alloc2d( rows, cols );
    blue = alloc2d( rows, cols );

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

void netPBM::rotateCCW()
{
    int i;
    int j;
    netPBM img;

    img = *this;

    free2d( redGray, rows );
    free2d( green, rows );
    free2d( blue, rows );

    swap( rows, cols );

    redGray = alloc2d( rows, cols );
    green = alloc2d( rows, cols );
    blue = alloc2d( rows, cols );

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

void netPBM::sharpen()
{
    int i;
    int j;
    pixel** tempred;
    pixel** tempgreen;
    pixel** tempblue;

    tempred = alloc2d( rows, cols );
    tempgreen = alloc2d( rows, cols );
    tempblue = alloc2d( rows, cols );

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            if ( ( i == 0 ) || ( i == rows - 1 ) || ( j == 0 ) || ( j == cols - 1 ) )
            {
                tempred[i][j] = 0;
                tempgreen[i][j] = 0;
                tempblue[i][j] = 0;
            }
            else
            {
                sharpenCompute( redGray, tempred, i, j );
                sharpenCompute( green, tempgreen, i, j );
                sharpenCompute( blue, tempblue, i, j );
            }
        }
    }
    
    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = tempred[i][j];
            green[i][j] = tempgreen[i][j];
            blue[i][j] = tempblue[i][j];
        }
    }

    free2d( tempred, rows );
    free2d( tempgreen, rows );
    free2d( tempblue, rows );
}

void netPBM::sharpenCompute( pixel** img, pixel** temp, int i, int j )
{
    int temp_value;

    temp_value = ( 5 *  img[i][j] ) - img[i - 1][j] - img[i + 1][j]
        - img[i][j - 1] - img[i][j + 1];
    temp_value = cropRound( temp_value );
    temp[i][j] = temp_value;
}

void netPBM::smooth()
{
    int i;
    int j;
    pixel** tempred;
    pixel** tempgreen;
    pixel** tempblue;

    tempred = alloc2d( rows, cols );
    tempgreen = alloc2d( rows, cols );
    tempblue = alloc2d( rows, cols );

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            if ( ( i == 0 ) || ( i == rows - 1 ) || ( j == 0 ) || ( j == cols - 1 ) )
            {
                tempred[i][j] = 0;
                tempgreen[i][j] = 0;
                tempblue[i][j] = 0;
            }

            else 
            {
                smoothCompute( redGray, tempred, i, j );
                smoothCompute( green, tempgreen, i, j );
                smoothCompute( blue, tempblue, i, j );
            }
        }
    }

    for ( i = 0; i < rows; i++ )
    {
        for ( j = 0; j < cols; j++ )
        {
            redGray[i][j] = tempred[i][j];
            green[i][j] = tempgreen[i][j];
            blue[i][j] = tempblue[i][j];
        }
    }

    free2d( tempred, rows );
    free2d( tempgreen, rows );
    free2d( tempblue, rows );
}

void netPBM::smoothCompute( pixel** img, pixel** temp, int i, int j )
{
    int temp_value;

    temp_value = ( img[i][j] + img[i - 1][j - 1]
        + img[i - 1][j] + img[i - 1][j + 1]
        + img[i][j - 1] + img[i][j + 1]
        + img[i + 1][j] + img[i + 1][j - 1]
        + img[i + 1][j + 1] ) / 9;
    temp_value = cropRound( temp_value );
    temp[i][j] = temp_value;
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