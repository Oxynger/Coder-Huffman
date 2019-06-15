#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <chrono>
#include "./CountSymbol/CountSymbols.hpp"
#include "./CoderTable/CoderTable.hpp"
#include "./ExpendedCoderTable/ExpendedCoderTable.hpp"
#include "./ExpendedCountSymbol/ExpendedCountSymbols.hpp"

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

void encryptCoder(string outTable, string codes, string openedFile, string outDecrypt, string benchFile)
{
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

    // Конец отчета времени
    auto end = std::chrono::high_resolution_clock::now();

    // Прошедшее время в милисекундах
    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Вывод времени в файл
    benchOut << "milliseconds: " << elapsedMilliseconds << endl;
}

void expendedEncryptCoder(string openedFile, string expendedCodes, string expendedDecrypt, string expendedBench)
{
    fstream infile;
    infile.open(openedFile, ios::in);
    std::ofstream benchOut(expendedBench, ios::out);

    auto fullText = getTextFromFile(infile);

    clear_file(expendedCodes);
    clear_file(expendedDecrypt);

    // Начало отчета времени.
    auto start = std::chrono::high_resolution_clock::now();

    ExpendedCountSymbols numberOfSymbols(fullText);

    ExpendedCoderTable HuffmanTable(numberOfSymbols, fullText);

    HuffmanTable.Encrypt(expendedCodes);
    HuffmanTable.Decrypt(expendedDecrypt, expendedCodes);

    // Конец отчета времени
    auto end = std::chrono::high_resolution_clock::now();

    // Прошедшее время в милисекундах
    auto elapsedMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Вывод времени в файл
    benchOut << "milliseconds: " << elapsedMilliseconds << endl;
}

int main()
{
    const string outTable = "bin/data/out_tab.txt";
    const string codes = "bin/data/out_code.txt";
    const string openedFile = "inp.txt";
    const string outDecrypt = "bin/data/out_msg.txt";
    const string benchFile = "bin/data/coder_bench.txt";
    const string expendedCodes = "bin/data/expended_out_code.txt";
    const string expendedDecrypt = "bin/data/expended_out_msg.txt";
    const string expendedBench = "bin/data/expended_bench.txt";

    setlocale(LC_ALL, "Russian");

    cout << "Имя файла из которого берем входные данные: " << openedFile << endl;
    encryptCoder(outTable, codes, openedFile, outDecrypt, benchFile);

    expendedEncryptCoder(openedFile, expendedCodes, expendedDecrypt, expendedBench);

    return 0;
}
