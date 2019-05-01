#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class CountOfSymbols : public map<char, map<char, int>>
{
private:
    uint encodingBites = 255;

    string converServiceSymbols(char Symbol)
    {
        switch ((int)Symbol)
        {
        case 13:
            return "Enter";
        
        case 32:
            return "Space";
        default:
            return string(1,Symbol);
        }
        return "";
    }

public:
    CountOfSymbols() : map()
    {
        // Заполняем количество символов нулями.
        for (char line = 0; line < encodingBites + 1; line++)
        {
            for (char column = 0; column < encodingBites + 1; column++)
            {
                (*this)[line][column] = 0;
            }
        }
    }
    string FormatOutput()
    {
        stringstream OutputString;
        for (auto LineCursor = this->begin(); LineCursor != this->end(); LineCursor++)
        {
            auto Line = LineCursor->second;
            for (auto ColumnCursor = Line.begin(); ColumnCursor != Line.end(); ColumnCursor++)
            {
                auto Number = ColumnCursor->second;
                auto FirstSymbol = converServiceSymbols(LineCursor->first);
                auto SecondSymdol = converServiceSymbols(ColumnCursor->first);

                if (ColumnCursor->second != 0)
                {
                    OutputString << "Symbol: " << FirstSymbol << " -> " << SecondSymdol << " - " << Number << endl;
                }
            }
        }

        return OutputString.str();
    }

    void WriteInFile(ofstream &out)
    {
        for (auto LineCursor = this->begin(); LineCursor != this->end(); LineCursor++)
        {
            auto Line = LineCursor->second;

            for (auto Number = Line.begin(); Number != Line.end(); Number++)
            {
                out << Number->second << "  ";
            }

            out << endl;
        }
    }
};

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

string ForamtCountOfSymbols(CountOfSymbols symbols)
{
}

// Записываем таблицу хафмана в файл
void WriteTable(CountOfSymbols symbolsCount, string filePath)
{
    ofstream out(filePath, ios_base::app);

    if (out.is_open())
    {
        cout << symbolsCount.FormatOutput() << endl;
        symbolsCount.WriteInFile(out);
    }

    out.close();
}

CountOfSymbols CountingSymbols(const string &input_str)
{
    auto ASCII = createASCIITable();

    CountOfSymbols countOfSymbols;

    string str = input_str;

    char FirstLetter = str.at(0);

    for (int i = 1; i < str.length(); i++)
    {
        char SecondLetter = str.at(i);

        countOfSymbols[FirstLetter][SecondLetter]++;

        FirstLetter = SecondLetter;
    }

    return countOfSymbols;
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

    auto numberOfSymbols = CountingSymbols(fullText);

    infile.close();

    clear_file(outFile);
    WriteTable(numberOfSymbols, outFile);

    return 0;
}
