#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include "./Huffman/Huffman.hpp"

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
            return string(1, Symbol);
        }
        return "";
    }

    void WriteInFile(ofstream &out)
    {
        for (auto LineCursor = this->begin(); LineCursor != this->end(); LineCursor++)
        {
            auto Line = LineCursor->second;

            for (auto Number = Line.begin(); Number != Line.end(); Number++)
            {
                out << Number->second << " ";
            }

            out << endl;
        }
    }

    void CountingSymbols(const string &input_str)
    {
        string str = input_str;

        char FirstLetter = str.at(0);

        for (int i = 1; i < str.length(); i++)
        {
            char SecondLetter = str.at(i);

            (*this)[FirstLetter][SecondLetter]++;

            FirstLetter = SecondLetter;
        }
    }

public:
    CountOfSymbols() : map()
    {
        // Заполняем количество символов нулями.
        for (int line = 0; line < encodingBites; line++)
        {
            for (int column = 0; column < encodingBites; column++)
            {
                (*this)[line][column] = 0;
            }
        }
    }

    CountOfSymbols(string text)
    {
        CountOfSymbols c;
        c.CountingSymbols(text);

        (*this) = c;
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

    // Записываем количесвто символов в файл
    void WriteTable(string filePath)
    {
        ofstream out(filePath, ios_base::app);

        if (out.is_open())
        {
            cout << this->FormatOutput() << endl;
            this->WriteInFile(out);
        }

        out.close();
    }
};

class CoderTable : public map<char, map<char, vector<bool>>>
{
private:
    uint encodingBites = 255;
    char emptyCode = '-';

    void WriteInFile(ofstream &out)
    {
        for (auto LineCursor : (*this))
        {
            for (auto columnsCursor : LineCursor.second)
            {

                // Выводим символ пустого кода
                if (columnsCursor.second.empty())
                {
                    out << emptyCode;
                }

                for (auto binary : columnsCursor.second)
                {
                    out << binary;
                }
                out << " ";
            }

            out << endl;
        }
    }

    string converServiceSymbols(char Symbol)
    {
        switch ((int)Symbol)
        {
        case 13:
            return "Enter";

        case 32:
            return "Space";
        default:
            return string(1, Symbol);
        }
        return "";
    }

    void Encrypt(CountOfSymbols numberOfSymbols)
    {
        for (const auto lineSymbolsCursor : numberOfSymbols)
        {
            auto fullMap = lineSymbolsCursor.second;

            map<char, int> mapSymbols;

            // Собираем новый хэш со значениями != 0
            for (const auto columnSymbolCursor : fullMap)
            {
                if (columnSymbolCursor.second != 0)
                    mapSymbols[columnSymbolCursor.first] = columnSymbolCursor.second;
            }

            auto HuffmanTree = InitHuffmanTree(mapSymbols);

            FillHuffmanTree(HuffmanTree);

            //root - указатель на вершину дерева
            Node *root = HuffmanTree.front();

            ////// создаем пары 'символ-код':
            auto encryptedHuffman = CreateEncyptedTable(root);

            for (auto encrypted : encryptedHuffman)
            {
                (*this)[lineSymbolsCursor.first][encrypted.first] = encrypted.second;
            }
        }
    }

public:
    CoderTable() : map()
    {
        // Заполняем коды символов 0-ми
        for (int line = 0; line < encodingBites; line++)
        {
            for (int column = 0; column < encodingBites; column++)
            {
                (*this)[line][column] = vector<bool>(0);
            }
        }
    }

    CoderTable(CountOfSymbols count)
    {
        CoderTable c;
        c.Encrypt(count);

        (*this) = c;
    }
    // Красивый вывод в коннсоль
    string FormatOutput()
    {
        stringstream OutputString;

        OutputString << "Symbol codes:" << endl;
        for (auto LineCursor : (*this))
        {

            auto Line = LineCursor.second;
            for (auto ColumnCursor : Line)
            {
                auto code = ColumnCursor.second;

                if (!code.empty())
                {
                    OutputString << converServiceSymbols(LineCursor.first) << " -> ";
                    OutputString << converServiceSymbols(ColumnCursor.first) << " : ";
                    for (auto binary : code)
                    {

                        OutputString << binary;
                    }
                    OutputString << endl;
                }
            }
        }

        return OutputString.str();
    }

    // Записываем количесвто символов в файл
    void WriteTable(string filePath)
    {
        ofstream out(filePath, ios_base::app);

        if (out.is_open())
        {
            cout << this->FormatOutput() << endl;
            this->WriteInFile(out);
        }

        out.close();
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

    CountOfSymbols numberOfSymbols(fullText);

    CoderTable HuffmanTable(numberOfSymbols);

    infile.close();

    clear_file(outFile);

    HuffmanTable.WriteTable(outFile);

    return 0;
}
