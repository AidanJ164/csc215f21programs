#include "netPBM.h"

int main( int argc, char** argv )
{
    string option;
    string basename;
    string baseimage;
    string format;
    int value;
    ifstream fin;
    ofstream fout;
    netPBM img;

    //Check for valid number of command line args
    if ( ( argc < 4 ) || ( argc > 6 ) )
    {
        outputErrorMessage();
        return 0;
    }
    //Check if 4 args has valid options
    if ( argc == 4 )
    {
        if ( !( ( strcmp( argv[1], "-oa" ) == 0 ) ||
            ( strcmp( argv[1], "-ob" ) == 0 ) ) )
        {
            outputErrorMessage();
            return 0;
        }
        format = argv[1];
        basename = argv[2];
        baseimage = argv[3];
    }
    //Check if 5 args has valid options
    else if ( ( argc == 5 ) )
    {
        if ( !( ( strcmp( argv[1], "-n" ) == 0 ) || ( strcmp( argv[1], "-p" ) == 0 )
            || ( strcmp( argv[1], "-s" ) == 0 ) || ( strcmp( argv[1], "-g" ) == 0 )
            || ( strcmp( argv[1], "-c" ) == 0 ) || ( strcmp( argv[1], "-b" ) == 0 ) &&
            !( ( strcmp( argv[2], "-oa" ) == 0 ) || ( strcmp( argv[2], "-ob" ) == 0 ) ) ) )
        {
            outputErrorMessage();
            return 0;
        }
        if ( strcmp( argv[1], "-b" ) == 0 )
        {
            outputErrorMessage();
            return 0;
        }
        option = argv[1];
        format = argv[2];
        basename = argv[3];
        baseimage = argv[4];
    }
    //Check if 6 args has valid options
    else
    {
        if ( !( ( strcmp( argv[3], "-oa" ) == 0 ) || ( strcmp( argv[3], "-ob" ) == 0 )
            || ( strcmp( argv[1], "-b" ) != 0 ) ) )
        {
            outputErrorMessage();
            return 0;
        }
        option = argv[1];
        value = atoi( argv[2] );
        format = argv[3];
        basename = argv[4];
        baseimage = argv[5];
    }

    // Add the file extension to the basename.
    if ( ( option == "-g" ) || ( option == "-c" ) )
    {
        basename += ".pgm";
    }
    else
    {
        basename += ".ppm";
    }

    // Read in the image to a class.
    if (!img.readInImage( baseimage ))
    {
        cout << "Could not read in " << baseimage;
        return 0;
    }

    // Apply option to image
    if ( option == "-b" )
    {
        img.brighten( value );
    }
    else if ( option == "-n" )
    {
        img.negate();
    }
    else if ( ( option == "-g" ) || (option == "-c") )
    {
        img.grayscale();
        if (option == "-c")
        {
            
        }
    }

    if (format == "-oa")
    {
        if ( (option == "-g") || (option == "-c") )
        {
            img.writeOutGrayImage( basename, netPBM::ASCII );
        }
        else
        {
            if(!img.writeOutImage( basename, netPBM::ASCII ))
            {
                cout << "Could not open " << basename;
                return 0;
            }
        }
    }
    else
    {
        if ( ( option == "-g" ) || ( option == "-c" ) )
        {
            img.writeOutGrayImage( basename, netPBM::RAW);
        }
        else
        {
            if ( !img.writeOutImage( basename, netPBM::RAW ) )
            {
                cout << "Could not open " << basename;
                return 0;
            }
        }
    }

    return 0;
}

/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Outputs the error message when incorrect command line arguments are given.
 *
 * @par Example
 * @verbatim
   // outputErrorMessage();
   @endverbatim
 *****************************************************************************/
void outputErrorMessage()
{
    cout << "Usage: thpf.exe [option] -o[ab] basename image.ppm" << endl;
    cout << "Option" << endl << "-n   Negate" << endl;
    cout << "-b # Brighten" << endl << "-p   Sharpen" << endl;
    cout << "-s   Smooth" << endl << "-g   Grayscale" << endl;
    cout << "-c   Contrast" << endl;
}