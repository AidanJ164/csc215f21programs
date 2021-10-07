#include "netPBM.h"

int main(int argc, char** argv)
{
    string option;
    string basename;
    string baseimage;
    string format;
    int i;
    int j;
    int value;
    ifstream fin;
    ofstream fout;
    image img;
    

    //Check for valid number of command line args
    if ((argc < 4) || (argc > 6))
    {
        cout << "Usage: thpExam1.exe [option] -o[ab] basename image.ppm";      
        return 0;
    }
    //Check if 4 args has valid options
    if (argc == 4)
    {
        if (!((strcmp(argv[1], "-oa") == 0) ||
              (strcmp(argv[1], "-ob") == 0)))
        {
            cout << "Usage: thpExam1.exe -o[ab] basename image.ppm";
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
            cout << "Usage: thpExam1.exe [option] -o[ab] basename image.ppm";
            return 0;
        }
        if (strcmp(argv[1], "-b") == 0 )
        {
            cout << "Usage: thpExam1.exe -b # -o[ab] basename image.ppm";
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
            cout << "Usage: thpExam1.exe -b # -o[ab] basename image.ppm";
            return 0;
        }
        option = argv[1];
        value = atoi(argv[2]);
        format = argv[3];
        basename = argv[4];
        baseimage = argv[5];
    }

    basename += ".ppm";
    if (!openFile(fin, baseimage, fout, basename))
    {
        return 0;
    }

    fin >> img.magicNumber;
    if ((img.magicNumber != "P3") && (img.magicNumber != "P6"))
    {
        cout << "Invalid Magic Numbers" << endl
             << "Valid Magic Numbers: P3 and P6";
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
    }
    else
    {
        img.magicNumber = "P6";
    }

    outputHeader(img, fout);

    // Output Ascii Values to file
    if (img.magicNumber == "P3")
    {
        outputAscii( fout, img );
    }
   
    // Output binary values to file
    if ( img.magicNumber == "P6" )
    {
        outputBinary( fout, img );
    }
    

    clearArray(img.redgray, img.rows);
    clearArray(img.green, img.rows);
    clearArray(img.blue, img.rows);
    closeFile(fin, fout);

    return 0;
}

void readHeader(image& img, ifstream& fin)
{
    string garbage;
    string comment;

    while (fin.peek() == '#')
    {
        getline(fin, comment);
        img.comment += comment + '\n';
    }
    fin >> img.cols; //fin >> cols
    fin >> img.rows;

    fin >> garbage;
    fin.ignore();
}

void outputHeader(image img, ofstream& fout)
{
    fout << img.magicNumber << '\n';
    fout << img.comment;
    fout << img.cols << " " << img.rows << '\n' << "255" << '\n';
}

void readAscii(ifstream& fin, image img)
{
    int i; 
    int j;
    int temp_value;

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

void readBinary(ifstream& fin, image& img)
{
    int i;
    int j;

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

void outputAscii(ofstream& fout, image img)
{
    int i;
    int j;

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

void outputBinary(ofstream& fout, image img)
{
    int i;
    int j;

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