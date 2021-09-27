#include "netPBM.h"

bool openFile(ifstream& fin, string input, ofstream& fout, string output)
{
    fin.open(input);
    if (!fin.is_open())
    {
        cout << input << " could not be opened.";
        return false;
    }

    fout.open(output);
    if (!fout.is_open())
    {
        cout << output << " could not be opened.";
        fin.close();
        return false;
    }

    return true;
}

void closeFile(ifstream& fin, ofstream& fout)
{
    fin.close();
    fout.close();
}