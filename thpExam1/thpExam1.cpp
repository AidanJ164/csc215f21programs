/** **************************************************************************
 * @file
 * 
 * @brief contains main and any extra functions
 ****************************************************************************/
 /** **************************************************************************
  * @mainpage thpExam1
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
  * There are 6 options that can be applied to the image. You can negate the
  * image which takes each pixel and subtracts its value from the max color
  * value. You can brighten an image a certain amount which takes each color
  * pixel and adds the given value. You can sharpen an image which will
  * subtract the neighboring pixels from the center pixel multiplied by 5.
  * You can smooth an image which takes the average of each pixel and it's
  * neighbors. You can grayscale an image which takes each color band and
  * combines them to a single grayscale pixel. Finally, you can contrast an
  * image which grayscales the image and subtracts the minimum value from each
  * pixel and then multiplies it by the scale. The last two options convert
  * image to a .pgm file.
  *
  * @section compile_section Compiling and Usage
  *
  * @par Compiling Instructions:
  *      none - a straight compile and link with no external libraries.
  *
  * @par Usage:
    @verbatim
    c:\> thpExam1.exe [option] -o[ab] basename image.ppm
            Option          Option Name
              -n            Negate
              -b #          Brighten
              -p            Sharpen
              -s            Smooth
              -g            Grayscale
              -c            Contrast
            -oa - Convert image to ascii format
            -ob - Convert image to binary format
            basename  - output image name
            image.ppm - input image
    @endverbatim
  *
  * @section todo_bugs_modification_section Todo, Bugs, and Modifications
  * 
  * @bug Struggles writing out all data for larger pictures.
  *
  * @par Modifications and Development Timeline:
  * https://gitlab.cse.sdsmt.edu/101066736/csc215f21programs/-/commits/master/thpExam1
  *
  ****************************************************************************/

#include "netPBM.h"

/** ***************************************************************************
 * @author Aidan Justice
 * 
 * @par Description 
 * Main function of the program that calls all of the other functions. It 
 * takes in a .ppm image, applies an option, converts it to either ascii or 
 * binary format, and then creates a new image with an extension of .ppm or 
 * .pgm.
 * 
 * @param[in]    argc - amount of command line arguments given
 * @param[in]    argv - 2d char array of command line arguments
 * 
 * @returns 0
 * 
 * @par Example
 * @verbatim
   // thpExam1.exe [option] -o[ab] basename image.ppm
   @endverbatim
 *****************************************************************************/
int main(int argc, char** argv)
{
    string option;
    string basename;
    string baseimage;
    string format;
    int value;
    ifstream fin;
    ofstream fout;
    image img;
    double min;
    double scale;
    
    //Check for valid number of command line args
    if ((argc < 4) || (argc > 6))
    {
        outputErrorMessage();
        return 0;
    }
    //Check if 4 args has valid options
    if (argc == 4)
    {
        if (!((strcmp(argv[1], "-oa") == 0) ||
              (strcmp(argv[1], "-ob") == 0)))
        {
            outputErrorMessage();
            return 0;
        }
        format = argv[1];
        basename = argv[2];
        baseimage = argv[3];
    }
    //Check if 5 args has valid options
    else if ((argc == 5))
    {
        if (!( (strcmp(argv[1], "-n") == 0) || (strcmp(argv[1], "-p") == 0)
            || (strcmp(argv[1], "-s") == 0) || (strcmp(argv[1], "-g") == 0)
            || (strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "-b") == 0) &&
            !((strcmp(argv[2], "-oa") == 0) || (strcmp(argv[2], "-ob") == 0))))
        {
            outputErrorMessage();
            return 0;
        }
        if (strcmp(argv[1], "-b") == 0 )
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
    else
    {
        if (!((strcmp(argv[3], "-oa") == 0) || (strcmp(argv[3], "-ob") == 0) 
             || (strcmp(argv[1], "-b") != 0)))
        {
            outputErrorMessage();
            return 0;
        }
        option = argv[1];
        value = atoi(argv[2]);
        format = argv[3];
        basename = argv[4];
        baseimage = argv[5];
    }


    // Add the file extension to the basename.
    if ((option == "-g") || (option == "-c"))
    {
        basename += ".pgm";
    }
    else
    {
        basename += ".ppm";
    }


    //Open input and output file
    if (!openFile(fin, baseimage, fout, basename))
    {
        return 0;
    }


    // Read in magic number and confirm it is valid.
    fin >> img.magicNumber;
    if ((img.magicNumber != "P3") && (img.magicNumber != "P6"))
    {
        cout << "Invalid Magic Numbers" << endl
             << "Valid Magic Numbers: P3 and P6";
        closeFile(fin, fout);
        return 0;
    }


    fin.ignore();
    readHeader(img, fin);
    

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
        readAscii(fin, img);
    }


    //Read in Binary Values from image
    if (img.magicNumber == "P6")
    {
        readBinary(fin, img);
    }


    // Change magic number
    if (format == "-oa")
    {
        img.magicNumber = "P3";
        if ((option == "-g") || (option == "-c"))
        {
            img.magicNumber = "P2";
        }
    }
    else
    {
        img.magicNumber = "P6";
        if ((option == "-g") || (option == "-c"))
        {
            img.magicNumber = "P5";
        }
    }


    // Do selected operation
    if (option == "-b")
    {
        brighten(img, value);
    }
    else if (option == "-n")
    {
        negate(img);
    }
    else if ((option == "-g") || (option == "-c"))
    {
        grayscale(img, min, scale);
        if (option == "-c")
        {
            contrast(img, min, scale);
        }
    }
    else if (option == "-p")
    {
        sharpen(img);
    }
    else if (option == "-s")
    {
        smooth(img);
    }


    outputHeader(img, fout);


    // Output Ascii Values to file
    if (img.magicNumber == "P3")
    {
        outputAscii( fout, img );
    }
   
    // Output binary values to file
    else if ( img.magicNumber == "P6" )
    {
        outputBinary( fout, img );
    }

    // Output the grayscale ascii values
    else if ( img.magicNumber == "P2" )
    {
        outputGrayAscii(img, fout);
    }

    // Output the grayscale binary values
    else if (img.magicNumber == "P5")
    {
        outputGrayBinary(img, fout);
    }
    

    // Clean up arrays and close files.
    clearArray(img.redgray, img.rows);
    clearArray(img.green, img.rows);
    clearArray(img.blue, img.rows);
    closeFile(fin, fout);

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
    cout << "Usage: thpExam1.exe [option] -o[ab] basename image.ppm" << endl;
    cout << "Option" << endl << "-n   Negate" << endl;
    cout << "-b # Brighten" << endl << "-p   Sharpen" << endl;
    cout << "-s   Smooth" << endl << "-g   Grayscale" << endl;
    cout << "-c   Contrast" << endl;
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
void readHeader(image& img, ifstream& fin)
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
void outputHeader(image img, ofstream& fout)
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
void readAscii(ifstream& fin, image& img)
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
void readBinary(ifstream& fin, image& img)
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
void outputAscii(ofstream& fout, image img)
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
void outputBinary(ofstream& fout, image img)
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



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Check to see if the inputted integer is less than 0 or greater than 255.
 *
 * @param[in,out] num - integer
 *
 * @par Example
 * @verbatim
   // checkNum(num);
   @endverbatim
 *****************************************************************************/
void checkNum(int& num)
{
    // If num is greater than 255, set value to 255.
    if (num > 255)
    {
        num = 255;
    }
    // If num is less than 0, set value to 0.
    if (num < 0)
    {
        num = 0;
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Output the ascii grayscale data to a new image file.
 *
 * @param[in]     img - image structure that holds the images data
 * @param[in,out] fout - output image file
 *
 * @par Example
 * @verbatim
   // outputGrayAscii(img, fout);
   @endverbatim
 *****************************************************************************/
void outputGrayAscii(image img, ofstream& fout)
{
    int i;
    int j;

    // Write one gray pixel in ascii.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            fout << (int) img.redgray[i][j] << endl;
        }
    }
}



/** ***************************************************************************
 * @author Aidan Justice
 *
 * @par Description
 * Output the binary grayscale data to a new image file.
 *
 * @param[in]     img - image structure that holds the images data
 * @param[in,out] fout - output image file
 *
 * @par Example
 * @verbatim
   // outputGrayBinary(img, fout);
   @endverbatim
 *****************************************************************************/
void outputGrayBinary(image img, ofstream& fout)
{
    int i;
    int j;

    // Write one gray pixel at a time in binary.
    for (i = 0; i < img.rows; i++)
    {
        for (j = 0; j < img.cols; j++)
        {
            fout.write((char*)&img.redgray[i][j], sizeof(pixel));
        }
    }
}