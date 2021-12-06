#include "netPBM.h"

int main( int argc, char** argv )
{
    string color;
    string option;
    string basename;
    string baseimage;
    string format;
    int col;
    int row;
    int height;
    int width;
    int value;
    ifstream fin;
    ofstream fout;
    netPBM img;
    netPBM img2;


    //Check for valid number of command line args
    if ( ( argc < 4 ) || ( ( argc > 6 ) && ( argc < 9 ) ) || ( argc > 9 ) )
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
            || ( strcmp( argv[1], "-c" ) == 0 ) || ( strcmp( argv[1], "-b" ) == 0 )
            || ( strcmp( argv[1], "-x" ) == 0 ) || ( strcmp( argv[1], "-y" ) == 0 ) 
            || ( strcmp( argv[1], "-==" ) == 0 ) || ( strcmp(argv[1], "-!=") == 0 ) 
            || ( strcmp( argv[1], "-CW" ) == 0 ) || ( strcmp(argv[1], "-CCW" )== 0 ) ) &&
            !( ( strcmp( argv[2], "-oa" ) == 0 ) || ( strcmp( argv[2], "-ob" ) == 0 ) ) ) 
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
    else if ( argc == 6 )
    {
        if ( !( ( strcmp( argv[3], "-oa" ) == 0 ) || ( strcmp( argv[3], "-ob" ) == 0 )
            || ( ( strcmp( argv[1], "-b" ) != 0 )  && ( strcmp( argv[1], "-r" ) != 0 ) ) ) )
        {
            outputErrorMessage();
            return 0;
        }
        option = argv[1];
        if (option == "-b")
        {
            value = atoi( argv[2] );
        }
        else 
        {
            color = argv[2];
        }
        format = argv[3];
        basename = argv[4];
        baseimage = argv[5];
    }
    else
    {
        if ( !( ( strcmp( argv[6], "-oa" ) == 0 ) || ( strcmp( argv[6], "-ob" ) == 0 ) )
            || ( strcmp( argv[1], "-i" ) != 0 ) )
        {
            outputErrorMessage();
                return 0;
        }
        option = argv[1];
        height = atoi( argv[2] );
        width = atoi( argv[3] );
        row = atoi( argv[4] );
        col = atoi( argv[5] );
        format = argv[6];
        basename = argv[7];
        baseimage = argv[8];
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
            img.contrast();
        }
    }
    else if ( option == "-p" )
    {
        img.sharpen();
    }
    else if ( option == "-s" )
    {
        img.smooth();
    }
    else if ( option == "-x" )
    {
        img.flipx();
    }
    else if (option == "-y" )
    {
        img.flipy();
    }
    else if (option == "-==" )
    {
        // Used to show off == operator.
        img2 = img;

        if ( img == img2 )
        {
            cout << "Images are the same" << endl;
        }

        img.writeOutImage( "Balloons1.ppm", netPBM::RAW );
        img2.writeOutImage( "Balloons2.ppm", netPBM::RAW );

        return 0;
    }
    else if ( option == "-CW" )
    {
        img.rotateCW();
    }
    else if ( option == "-CCW" )
    {
        img.rotateCCW();
    }
    else if ( option == "-i" )
    {
        img.icon( row, col, height, width );
    }
    else if (option == "-!=" )
    {
        // Used to show off != operator.
        img2 = img;

        cout << "Flipping image and comparing it to the original." << endl;
        img2.flipx();

        if ( img != img2 )
        {
            cout << "Images are not the same" << endl;
        }

        img.writeOutImage( "Balloons1.ppm", netPBM::RAW );
        img2.writeOutImage( "Balloons2.ppm", netPBM::RAW );

        return 0;
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
    cout << "-c   Contrast" << endl << "-x   Flip X" << endl;
    cout << "-y   Flip Y" << endl;
}