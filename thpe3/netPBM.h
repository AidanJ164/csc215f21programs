/** ***************************************************************************
 * @file
 *
 * @brief contains prototypes, typedefs, and structures
 *****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#ifndef __NETPBM__H__
#define __NETPBM__H__


/**
 * @brief A pixel value inside an image
*/
typedef unsigned char pixel;


/**
* @brief Provides access to newColor and oldColor look better.
*/
enum color { RED, /**<Access red color value.*/
             GREEN, /**<Access green color value.*/
             BLUE /**<Access blue color value.*/
            };


/**
 * @brief Holds the data stored in the original image file. 
*/
struct image
{
    string magicNumber; /**< Determines if image is in ascii or binary format.*/
    string comment; /**< Stores the comments in the original image. */
    int rows; /**< The amount of rows in the image. */
    int cols; /**< The amount of columns in the image. */
    pixel **redgray; /**< Holds the red or gray pixel values for the image. */
    pixel **green; /**< Holds the green pixel values for the image. */
    pixel **blue; /**< Holds the blue pixel values for the image. */
};


/*******************************************************************************
 *                         Function Prototypes
 ******************************************************************************/
bool openFile( fstream& fin, string input );
void closeFile( fstream& fin );
bool createArray( pixel** &array, int rows, int cols );
void clearArray( pixel** &array, int rows );
void readHeader( image& img, fstream& fin );
void outputHeader( image img, fstream& fout );
void readAscii( fstream& fin, image& img );
void readBinary( fstream& fin, image& img );
void outputAscii( fstream& fout, image img );
void outputBinary( fstream& fout, image img );
void fill( image& img, int row, int col, int newColor[], int oldColor[] );
bool getStartColor( image img, int oldColor[], int row, int col );
bool operator>>( fstream& file, image& img );

#endif
