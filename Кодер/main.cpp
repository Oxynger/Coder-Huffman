#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include "./CountSymbol/CountSymbols.hpp"
#include "./CoderTable/CoderTable.hpp"

using namespace std;

void clear_file(string filePath)
{
    ofstream f;
    f.open(filePath, ios::out | ios::trunc);
    f.close();
}

map<char, int> createASCIITable()
{
    map<char, int> ASCII;
    for (int i = 0; i < 256; ++i)
        ASCII[(char)i] = i;

    return ASCII;
}

int main()
{
    string outFile = "out_tab.txt";
    string openedFile = "inp.txt";

    setlocale(LC_ALL, "Russian");

    cout << "Имя файла из которого берем входные данные: " << openedFile << endl;

    fstream infile;
    infile.open(openedFile, ios::in);

    string fullText;
    if (infile.is_open())
    {
        string line;
        while (getline(infile, line))
        {
            // Проверить что будет закодирован символ переноса строки
            fullText.append(line);
        }
    }

    CountSymbols numberOfSymbols(fullText);

    CoderTable HuffmanTable(numberOfSymbols);

    infile.close();

    clear_file(outFile);

    HuffmanTable.WriteTable(outFile);

    return 0;
}
