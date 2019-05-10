#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
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

string getTextFromFile(std::fstream &in)
{
    std::string text;
    if (in.is_open())
    {
        string line;
        while (getline(in, line))
        {
            // Проверить что будет закодирован символ переноса строки
            text.append(line);
        }
    }
    in.close();

    return text;
}

int main()
{
    const string outTable = "bin/data/out_tab.txt";
    const string codes = "bin/data/out_code.txt";
    const string openedFile = "inp.txt";
    const string outDecrypt = "bin/data/out_msg.txt";
    const string benchFile = "bin/data/coder_bench.txt";

    setlocale(LC_ALL, "Russian");

    cout << "Имя файла из которого берем входные данные: " << openedFile << endl;

    fstream infile;
    infile.open(openedFile, ios::in);
    std::ofstream benchOut(benchFile, ios::out);
    
    auto fullText = getTextFromFile(infile);

    clear_file(outTable);
    clear_file(codes);
    clear_file(outDecrypt);
    clear_file(benchFile);

    // Начало отчета времени.
    auto start = std::chrono::high_resolution_clock::now();

    CountSymbols numberOfSymbols(fullText);

    CoderTable HuffmanTable(numberOfSymbols, fullText);

    HuffmanTable.WriteTable(outTable);
    HuffmanTable.Encrypt(codes);
    HuffmanTable.Decrypt(outDecrypt, codes);

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    benchOut << "milliseconds: " << elapsedMilliseconds << endl;

    return 0;
}
