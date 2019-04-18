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
int tableHuffman[255][255];
map<char, int> ASCII;

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
    //    f.open("out2.txt", ios::out | ios::trunc);
    //    f.close();
}

void FillASCII()
{
    for (int i = 0; i < 256; ++i)
    {
        cout << i << " : " << (char)i << endl;
        ASCII[(char)i] = i;
    }
}

void WriteTable()
{
    ofstream out("out_tab.txt", ios_base::app);
    if (out.is_open())
    {
        for (int it = 0; it < 256; ++it)
        {
            for (int i = 0; i < 256; ++i)
            {

                out << tableHuffman[it][i] << " ";
            }
            out << '\n';
        }
    }
    out.close();
}

void CreateTableHuffman(const string &input_str)
{
    string str = input_str;
    int codeFirstLetter = ASCII[str[0]];
    cout << codeFirstLetter << endl;
    for (int i = 0; i < strlen(str.c_str()); ++i)
    {
        int codeSecondLetter = ASCII[str[i + 1]];
        cout << codeSecondLetter << endl;
        tableHuffman[codeFirstLetter][codeSecondLetter]++;
        if (codeFirstLetter == 255 && codeSecondLetter == 2)
            cout << "HI-------------------" << endl;

        codeFirstLetter = codeSecondLetter;
    }
    //            for (std::map<char, int>::iterator it = m.begin(); it != m.end(); ++it)
    //            {
    //                cout << it->first << " : " << it->second << endl;
    //            }
}

void Coding()
{
}

void DeCoding(string str)
{
}

int main()
{
    setlocale(0, "");
    char name_file[20] = "inp.txt";
    FillASCII();
    cout << "Имя файла: " << name_file << endl;
    //    record_alpf();
    clear_file();
    fstream infile;
    infile.open(name_file, ios::in);
    if (infile.is_open())
        while (!infile.eof())
        {
            string str;
            getline(infile, str);
            if (!str[0])
                continue; // пропуск пустых строк
            CreateTableHuffman(str);
            Coding();
        }
    infile.close();
    WriteTable();
    fstream infdec;
    infdec.open("out.txt", ios::in);
    if (infdec.is_open())
        while (!infdec.eof())
        {
            string str;
            getline(infdec, str);
            DeCoding(str);
        }
    infdec.close();

    return 0;
}
