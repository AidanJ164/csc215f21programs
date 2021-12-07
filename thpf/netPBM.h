/** **************************************************************************
 * @file
 *
 * @brief Header file.
 ****************************************************************************/
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#ifndef __NETPBM__H__
#define __NETPBM__H__


/** 
* @brief A pixel value inside an image
*/
typedef unsigned char pixel;


/**
* @brief Holds the data for .ppm and .pgm images.
*/
class netPBM
{
    public:
        netPBM();
        netPBM( netPBM &img );
        ~netPBM();

        /**
        * @brief Way to output image data
        */
        enum outputType{ ASCII, /**< Output to ascii  */
                         RAW    /**< Output to binary */
                       };

        bool readInImage(string filename);
        bool writeOutImage(string filename, outputType out);
        bool writeOutGrayImage( string filename, outputType out );
        void readHeader( ifstream& fin );
        void readAscii( ifstream& fin );
        void readBinary( ifstream& fin );
        void outputHeader( ofstream& fout, string magicNum );

        void sharpen();
        void smooth();
        void negate();
        void brighten( int value );
        void grayscale();
        void contrast();
        void rotateCW();
        void rotateCCW();
        void flipx();
        void flipy();
        void removeRed();
        void removeGreen();
        void removeBlue();
        void icon(int row, int col, int height, int width);

        void operator=( netPBM img2 );
        bool operator==( netPBM img );
        bool operator!=( netPBM img );

    protected:
        pixel cropRound( double value );
        pixel** alloc2d( int rows, int cols );
        void free2d( pixel**& ptr, int rows );
        void findScale( double& scale, double& min );

    private:
        int rows;           /**< Amount of rows in the image                 */
        int cols;           /**< Amount of columns in the image              */
        string comments;    /**< Comments stored in the image file           */

        pixel **redGray;    /**< 2d array that holds the red or gray pixels  */
        pixel **green;      /**< 2d array that holds the green pixels        */
        pixel **blue;       /**< 2d array that holds the blue pixels         */
};

/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
void outputErrorMessage();

#endif