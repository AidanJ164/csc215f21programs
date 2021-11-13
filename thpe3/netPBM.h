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

void outputErrorMessage();
bool openFile(ifstream& fin, string input, ofstream& fout, string output);
void closeFile(ifstream& fin, ofstream& fout);
bool createArray(pixel** &array, int rows, int cols);
void clearArray(pixel** &array, int rows);
void readHeader(image& img, ifstream& fin);
void outputHeader(image img, ofstream& fout);
void readAscii(ifstream& fin, image& img);
void readBinary(ifstream& fin, image& img);
void outputAscii(ofstream& fout, image img);
void outputBinary(ofstream& fout, image img);


#endif
