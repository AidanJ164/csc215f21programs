#include "netPBM.h"

int main(int argc, char** argv)
{
    string option;
    string basename;
    string baseimage;
    string format;
    string comment;
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
    fin.ignore();
    fin >> comment;
    while (comment == "#")
    {
        getline(fin, comment);
        img.comment += "#" + comment;
        fin >> comment;
    }
    img.cols = stoi(comment);
    fin >> img.rows;

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
    
    

    clearArray(img.redgray, img.rows);
    clearArray(img.green, img.rows);
    clearArray(img.blue, img.rows);
    closeFile(fin, fout);

    return 0;
}