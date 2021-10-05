#include "netPBM.h"

bool openinFile(ifstream& fin, string input)
{
    fin.open(input);
    if (!fin.is_open())
    {
        cout << input << " could not be opened.";
        return false;
    }

    return true;
}

bool openoutFile(ofstream& fout, string output, string format)
{
    if (format == "-oa")
    {
        fout.open(output);
        if (!fout.is_open())
        {
            return false;
        }
    }
    else
    {
        fout.open(output, ios::out | ios::trunc | ios::binary);
        if (!fout.is_open())
        {
            return false;
        }
    }

    return true;
}

void closeFile(ifstream& fin, ofstream& fout)
{
    fin.close();
    fout.close();
}