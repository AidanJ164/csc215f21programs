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



#endif
