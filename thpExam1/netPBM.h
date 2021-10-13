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
void readAscii(ifstream& fin, image img);
void readBinary(ifstream& fin, image& img);
void outputAscii(ofstream& fout, image img);
void outputBinary(ofstream& fout, image img);
void outputGrayAscii(image img, ofstream& fout);
void outputGrayBinary(image img, ofstream& fout);
void brighten(image& img, int value);
void checkNum(int& num);
void negate(image& img);
void grayscale(image& img, double& min, double& scale);
void contrast(image& img, double min, double scale);
void sharpen(image& img);
void smooth(image& img);
void smoothCompute(pixel** img, pixel** arr, int i, int j);


#endif
