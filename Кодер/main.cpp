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
    string outTable = "out_tab.txt";
    string codes = "out_code.txt";
    string openedFile = "inp.txt";
    string outDecrypt = "out_msg.txt";

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

    CoderTable HuffmanTable(numberOfSymbols, fullText);

    infile.close();

    clear_file(outTable);
    clear_file(codes);
    clear_file(outDecrypt);

    HuffmanTable.WriteTable(outTable);
    HuffmanTable.Encrypt(codes);
    HuffmanTable.Decrypt(outDecrypt, codes);

    return 0;
}
