#include <iostream>
#include <fstream>
#include <string>

using namespace std;
#ifndef __NETPBM__H__
#define __NETPBM__H__
typedef unsigned char pixel;

struct image
{
    string magicNumber;
    string comment;
    int rows; 
    int cols;
    pixel **redgray;
    pixel **green;
    pixel **blue;
};

bool openFile(ifstream& fin, string input, ofstream& fout, string output);
void closeFile(ifstream& fin, ofstream& fout);
bool createArray(pixel** &array, int rows, int cols);
void clearArray(pixel** &array, int rows);
void readHeader(image& img, ifstream& fin);
void outputHeader(image img, ofstream& fout);


#endif
