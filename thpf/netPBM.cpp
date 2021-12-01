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

void netPBM::readHeader( ifstream &fin )
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
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            fin.read( ( char* ) &redGray[i][j], sizeof( pixel ) );
            fin.read( ( char* ) &green[i][j], sizeof( pixel ) );
            fin.read( ( char* ) &blue[i][j], sizeof( pixel ) );
        }
    }
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

void netPBM::outputHeader( ofstream& fout, string magicNum )
{
    fout << magicNum << '\n';
    fout << comments;
    fout << cols << " " << rows << '\n' << "255" << '\n';
}

bool netPBM::writeOutGrayImage( string filename, outputType out )
{
    int i;
    int j;
    ofstream fout;

    return true;
}

void netPBM::brighten( int value )
{
    int i;
    int j;
    int temp_value;

    // Calculate brightened value for each pixel.
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
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

    return ( pixel )value;
}