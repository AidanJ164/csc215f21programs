/** **************************************************************************
 * @file
 ****************************************************************************/
 /** **************************************************************************
  * @mainpage Final Program
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
  * @date Due October 15, 2021
  *
  * @section program_section Program Information
  *
  * @details
  * This program will take a .ppm image file and convert it to either
  * ascii or binary format. It changes the output type then copies the header
  * from the original image to the new image. You can also apply options to
  * manipulate the image.
  * 
  * @verbatim
    There are 12 options to apply to the image:
    -p Sharpen     - will subtract the neighboring pixels from the center pixel 
                  multiplied by 5.
    -s Smooth      - takes the average of each pixel and its neighbors.
    -n Negate      - takes each pixel value and subtracts it from the max value.
    -b Brighten    - takes each pixel and adds the user given value to it.
    -g Grayscale   - takes each color pixel and combines them into a single 
                  grayscale pixel.
    -c Contrast    - grayscales the image and subtracts the minimum value from 
                  each pixel and then multiplies it by the scale
    -CW Clockwise  - rotates the image clockwise
    -CCW CounterCW - rotates the image counterclockwise
    -x Flip x      - flips the image across the x-axis
    -y Flip y      - flips the image across the y-axis
    -i Icon        - creates an icon
    -r Remove      - sets the given color to 0 for each pixel.
   @endverbatim
  *
  * @section compile_section Compiling and Usage
  *
  * @par Compiling Instructions:
  *      none - a straight compile and link with no external libraries.
  *
  * @par Usage:
    @verbatim
    c:\> thpf.exe [option] -o[ab] basename image.ppm
            Option          Option Name
              -n            Negate
              -b #          Brighten
              -p            Sharpen
              -s            Smooth
              -g            Grayscale
              -c            Contrast
              -CW           Clockwise
              -CCW          Counterclockwise
              -x            Flip x
              -y            Flip y
              -i h w r c    Icon
                   h - Height of icon
                   w - Width of icon
                   r - starting row
                   c - starting column
              -r [r,g,b]    Remove
            -oa - Convert image to ascii format
            -ob - Convert image to binary format
            basename  - output image name
            image.ppm - input image
    @endverbatim
  *
  * @section todo_bugs_modification_section Todo, Bugs, and Modifications
  *
  * @bug None
  *
  * @todo None
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
 * Main function of the program used to call other functions. Takes .ppm 
 * images, reads them into netPBM class, applies an option, and then writes
 * the image back out into either a .ppm image or a .pgm image if it is a
 * grayscaled image.
 * 
 * @param[in]    argc - number of arguments given.
 * @param[in]    argv - 2d char array that stores the arguments.
 * 
 * @returns nothing
 * 
 * @par Example
 * @verbatim
   // thpf.exe [option] -o[ab] basename image.ppm
   @endverbatim
 *****************************************************************************/
int main( int argc, char** argv )
{
    string color;
    string format;
    string baseimage;
    string basename;
    string option;
    int col;
    int height;
    int row;
    int value;
    int width;
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

    // Check 9 arguments
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
    else if (option == "-r" )
    {
        if ( color == "r" )
        {
            img.removeRed();
        }
        else if ( color == "g" )
        {
            img.removeGreen();
        }
        else
        {
            img.removeBlue();
        }
    }
    // Used to show off == operator.
    else if ( option == "-==" )
    {
        img2 = img;

        if ( img == img2 )
        {
            cout << "Images are the same" << endl;
        }

        img.writeOutImage( "Balloons1.ppm", netPBM::RAW );
        img2.writeOutImage( "Balloons2.ppm", netPBM::RAW );

        return 0;
    }
    // Used to show off != operator.
    else if (option == "-!=" )
    {
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
    
    // Write out images in ascii.
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
    // Write out images in binary.
    else
    {
        if ( ( option == "-g" ) || ( option == "-c" ) )
        {
            if ( !img.writeOutGrayImage( basename, netPBM::RAW) )
            {
                cout << "Could not open " << basename;
            }
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
    cout << "Usage: thpf.exe [option] -o[ab] basename image.ppm" << endl <<
        "Option" << endl << 
        " -n            Negate" << endl <<
        " -b #          Brighten" << endl <<
        " -p            Sharpen" << endl <<
        " -s            Smooth" << endl <<
        " -g            Grayscale" << endl <<
        " -c            Contrast" << endl <<
        " -CW           Clockwise" << endl <<
        " -CCW          Counterclockwise" << endl <<
        " -x            Flip x" << endl <<
        " -y            Flip y" << endl <<
        " -r [r,g,b]    Remove" << endl <<
        "      [r,g,b] - red, green, blue" << endl <<
        " -i h w r c    Icon" << endl <<
        "      h - Height of icon" << endl <<
        "      w - Width of icon" << endl <<
        "      r - starting row" << endl <<
        "      c - starting column" << endl <<
        "-oa - Convert image to ascii format" << endl <<
        "-ob - Convert image to binary format" << endl <<
        "basename  - output image name" << endl <<
        "image.ppm - input image" << endl;
}