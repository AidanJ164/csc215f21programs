#include "netPBM.h"

int main(int argc, char** argv)
{
    fstream file;
    image img;
    int row;
    int col;
    int newColor[3];
    int oldColor[3];

    if (argc != 7)
    {
        cout << "Usage: thpe3.exe imageFile row col redValue greenValue blueValue";
        return 0;
    }

    if (!openFile(file, argv[1]))
    {
        return 0;
    }

    row = atoi(argv[2]);
    col = atoi(argv[3]);
    newColor[RED] = atoi(argv[4]);
    newColor[GREEN] = atoi(argv[5]);
    newColor[BLUE] = atoi(argv[6]);

    file >> img.magicNumber;
    if ((img.magicNumber != "P3") && (img.magicNumber != "P6"))
    {
        cout << "Invalid Magic Numbers" << endl
            << "Valid Magic Numbers: P3 and P6";
        closeFile(file);
        return 0;
    }

    file.ignore();
    readHeader(img, file);

    // Create pixel arrays
    if (!createArray(img.redgray, img.rows, img.cols))
    {
        return 0;
    }
    if (!createArray(img.green, img.rows, img.cols))
    {
        return 0;
    }
    if (!createArray(img.blue, img.rows, img.cols))
    {
        return 0;
    }

    // Read in Ascii Values
    if (img.magicNumber == "P3")
    {
        readAscii(file, img);
    }

    //Read in Binary Values from image
    else
    {
        readBinary(file, img);
    }

    // Fill in Area
    if (!getStartColor(img, oldColor, row, col))
    {
        return 0;
    }

    fill(img, row, col, newColor, oldColor);

    file.seekp(ios::beg, 0);
    outputHeader(img, file);
    
    if (img.magicNumber == "P3")
    {
        outputAscii(file, img);
    }
    else
    {
        outputBinary(file, img);
    }

    clearArray(img.redgray, img.rows);
    clearArray(img.green, img.rows);
    clearArray(img.blue, img.rows);
    closeFile(file);

}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Reads the header of the inputted image. Reads the magic number, image
 * comments, and the dimensions of the image.
 *
 * @param[in,out] img - an image structure that holds the images data.
 * @param[in,out] fin - the input .ppm image.
 *
 * @par Example
 * @verbatim
   // readHeader(img, fin);
   @endverbatim
 *****************************************************************************/
void readHeader(image& img, fstream& fin)
{
    string garbage;
    string comment;

    while (fin.peek() == '#')
    {
        getline(fin, comment);
        img.comment += comment + '\n';
    }
    fin >> img.cols;
    fin >> img.rows;

    fin >> garbage;
    fin.ignore();
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Output the header to the image file.
 *
 * @param[in]     img - image structure that holds the images data
 * @param[in,out] fout - output image file
 *
 * @par Example
 * @verbatim
   // outputHeader(img, fout);
   @endverbatim
 *****************************************************************************/
void outputHeader(image img, fstream& fout)
{
    fout << img.magicNumber << '\n';
    fout << img.comment;
    fout << img.cols << " " << img.rows << '\n' << "255" << '\n';
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Read in the data from a P3 .ppm image.
 *
 * @param[in,out] fin - inputted image file
 * @param[in,out] img - image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // readAscii(fin, img);
   @endverbatim
 *****************************************************************************/
void readAscii(fstream& fin, image& img)
{
    int i;
    int j;
    int temp_value;

    // Read ascii values into the image arrays.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            fin >> temp_value;
            img.redgray[i][j] = temp_value;

            fin >> temp_value;
            img.green[i][j] = temp_value;

            fin >> temp_value;
            img.blue[i][j] = temp_value;
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Read in the data from a P6 .ppm image.
 *
 * @param[in,out] fin - inputted image file
 * @param[in,out] img - image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // readBinary(fin, img);
   @endverbatim
 *****************************************************************************/
void readBinary(fstream& fin, image& img)
{
    int i;
    int j;

    // Read binary values into the image arrays.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            fin.read((char*)&img.redgray[i][j], sizeof(pixel));
            fin.read((char*)&img.green[i][j], sizeof(pixel));
            fin.read((char*)&img.blue[i][j], sizeof(pixel));
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Output the ascii data to the new image file.
 *
 * @param[in,out] fout - output image file
 * @param[in]     img - image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // outputAscii(fout, img);
   @endverbatim
 *****************************************************************************/
void outputAscii(fstream& fout, image img)
{
    int i;
    int j;

    // Write each color value to file in ascii.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            fout << (int)img.redgray[i][j] << " ";
            fout << (int)img.green[i][j] << " ";
            fout << (int)img.blue[i][j] << endl;
        }
    }
}


/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Output the binary data to the new image file.
 *
 * @param[in,out] fout - output image file
 * @param[in]     img - image structure that holds the images data
 *
 * @par Example
 * @verbatim
   // outputBinary(fout, img);
   @endverbatim
 *****************************************************************************/
void outputBinary(fstream& fout, image img)
{
    int i;
    int j;

    // Write each color value to file in binary.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            fout.write((char*)&img.redgray[i][j], sizeof(pixel));
            fout.write((char*)&img.green[i][j], sizeof(pixel));
            fout.write((char*)&img.blue[i][j], sizeof(pixel));
        }
    }
}


void fill(image& img, int row, int col, int newColor[], int oldColor[])
{
    if ((row < 0) || (row > img.rows) || (col < 0) || (col > img.cols) ||
        (img.redgray[row][col] != oldColor[RED]) ||
        (img.green[row][col] != oldColor[GREEN]) ||
        (img.blue[row][col] != oldColor[BLUE]) ||
        ((img.redgray[row][col] == newColor[RED]) &&
        (img.green[row][col] == newColor[GREEN]) &&
        (img.blue[row][col] == newColor[BLUE])))
    {
        return;
    }

    img.redgray[row][col] = newColor[RED];
    img.green[row][col] = newColor[GREEN];
    img.blue[row][col] = newColor[BLUE];

    fill(img, row - 1, col, newColor, oldColor);
    fill(img, row, col + 1, newColor, oldColor);
    fill(img, row + 1, col, newColor, oldColor);
    fill(img, row, col - 1, newColor, oldColor);
}


bool getStartColor(image& img, int oldColor[], int row, int col)
{
    if ((row < 0) || (row > img.rows) || (col < 0) || (col > img.cols))
    {
        return false;
    }

    oldColor[RED] = img.redgray[row][col];
    oldColor[GREEN] = img.green[row][col];
    oldColor[BLUE] = img.blue[row][col];

    return true;
}