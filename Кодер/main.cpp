#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

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

// Записываем таблицу хафмана в файл
void WriteTable(vector<vector<int>> tableHuffman, string filePath)
{
    ofstream out(filePath, ios_base::app);
    if (out.is_open())
    {
        for (auto Line : tableHuffman)
        {
            for (auto Value : Line)
            {
                out << Value << " ";
            }
            out << endl;
        }
    }

    out.close();
}

vector<vector<int>> CreateTableHuffman(const string &input_str)
{
    // Количество бит в кодеровки (ASCII - 255)
    size_t encryptBitCount = 255;
    auto ASCII = createASCIITable();

    vector<vector<int>> tableHuffman(encryptBitCount, vector<int>(encryptBitCount));

    string str = input_str;

    int codeFirstLetter = ASCII[str.at(0)];
    cout << codeFirstLetter << endl;
    for (int i = 1; i < str.length(); i++)
    {
        int codeSecondLetter = ASCII[str.at(i)];
        cout << codeSecondLetter << endl;

        tableHuffman[codeFirstLetter][codeSecondLetter]++;

        codeFirstLetter = codeSecondLetter;
    }

    return tableHuffman;
}

int main()
{
    string outFile = "out_tab.txt";

    setlocale(LC_ALL, "Russian");

    string openedFile = "inp.txt";
    cout << "Имя файла из которого берем входные данные: " << openedFile << endl;

    fstream infile;
    infile.open(openedFile, ios::in);

    string fullText;
    if (infile.is_open())
    {
        string line;
        while (getline(infile, line))
        {
            fullText.append(line);
        }
    }

    vector<vector<int>> HuffmanTable;

    HuffmanTable = CreateTableHuffman(fullText);

    infile.close();

    clear_file(outFile);
    WriteTable(HuffmanTable, outFile);

    return 0;
}
