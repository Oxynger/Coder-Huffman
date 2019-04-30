#include <iostream>
#include <locale>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <map>
#include <stack>

using namespace std;

vector<string> alpf, alp_num, alp_pun, alp_all_alpf;

void record_alpf()
{
    fstream in;
    string str;
    alpf.clear();
    alp_num.clear();
    alp_pun.clear();
    in.open("alpf.txt", ios::in);
    while (!in.eof())
    {
        getline(in, str);
        alpf.push_back(str);
    }
    in.close();
    in.open("num.txt", ios::in);
    while (!in.eof())
    {
        getline(in, str);
        alp_num.push_back(str);
    }
    in.close();
    in.open("pun.txt", ios::in);
    while (!in.eof())
    {
        getline(in, str);
        alp_pun.push_back(str);
    }
    in.close();
    in.open("all_alpf.txt", ios::in);
    while (!in.eof())
    {
        getline(in, str);
        alp_all_alpf.push_back(str);
    }
    in.close();
}

void clear_file()
{
    ofstream f;
    f.open("out_tab.txt", ios::out | ios::trunc);
    f.close();
}

map<char, int> CreateASCIITable()
{
    map<char, int> ASCII;
    for (int i = 0; i < 256; ++i)
        ASCII[(char)i] = i;

    return ASCII;
}


// Записываем таблицу хафмана в файл
void WriteTable(vector<vector<int> > tableHuffman, string filePath)
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

vector<vector<int> > CreateTableHuffman(const string &input_str)
{
    auto ASCII = CreateASCIITable();

    vector<vector<int> > tableHuffman(255, vector<int>(255));

    string str = input_str;

    int codeFirstLetter = ASCII[str.at(0)];
    cout << codeFirstLetter << endl;
    for (int i = 1; i < str.length(); i++)
    {
        int codeSecondLetter = ASCII[str.at(i)];
        cout << codeSecondLetter << endl;
        
        tableHuffman[codeFirstLetter][codeSecondLetter]++;
        if (codeFirstLetter == 255 && codeSecondLetter == 2)
            cout << "HI-------------------" << endl;

        codeFirstLetter = codeSecondLetter;
    }
    
    return tableHuffman;
}

void Coding()
{
}

void DeCoding(string str)
{
}

int main()
{
    setlocale(LC_ALL, "Russian");

    char name_file[20] = "inp.txt";

    CreateASCIITable();

    cout << "Имя файла: " << name_file << endl;

    clear_file();

    fstream infile;
    infile.open(name_file, ios::in);

    string fullText;
    if (infile.is_open())
    {
        string line;
        while (getline(infile, line))
        {
            fullText.append(line);
            Coding();
        }
    }
    
    vector<vector<int> > HuffmanTable;
    try
    {    
        HuffmanTable = CreateTableHuffman(fullText);
    }
    catch(const std::exception& e)
    {
        std::cerr << "in CreateTableHuffman functon" << endl;
        std::cerr << e.what() << '\n';
    }
        
    infile.close();
    
    WriteTable(HuffmanTable, "out_tab.txt");

    fstream infdec;
    infdec.open("out.txt", ios::in);

    if (!infdec.is_open())
    {
        while (!infdec.eof())
        {
            string str;
            getline(infdec, str);
            DeCoding(str);
        }
    }

    infdec.close();

    return 0;
}
