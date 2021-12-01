#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#ifndef __NETPBM__H__
#define __NETPBM__H__

typedef unsigned char pixel;

class netPBM
{
public:
    netPBM();
    ~netPBM();

    enum outputType{ ASCII, RAW };

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
    void icon(int row, int col, int height, int width);

protected:
    pixel cropRound( double value );
    pixel** alloc2d( int rows, int cols );
    void free2d( pixel**& ptr, int rows );

private:
    int rows;
    int cols;
    string comments;

    pixel **redGray;
    pixel **green;
    pixel **blue;
};

void outputErrorMessage();

#endif