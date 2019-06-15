
#include <sstream>
#include <fstream>
#include <iostream>

#include "./ExpendedCoderTable.hpp"
#include "../ExpendedCountSymbol/ExpendedCountSymbols.hpp"

ExpendedCoderTable::ExpendedCoderTable() : map()
{
    // Заполняем коды символов 0-ми
    for (int line = 0; line < encodingBites; line++)
    {
        for (int column = 0; column < encodingBites; column++)
        {
            for (int page = 0; page < encodingBites; page++)
            {
                (*this)[line][column][page] = std::vector<bool>(0);
            }
        }
    }
}

ExpendedCoderTable::ExpendedCoderTable(ExpendedCountSymbols count)
{
    ExpendedCoderTable c;
    c.encryptTable(count);

    (*this) = c;
}

ExpendedCoderTable::ExpendedCoderTable(ExpendedCountSymbols count, std::string text) : ExpendedCoderTable(count)
{
    this->text = text;
}

std::string ExpendedCoderTable::FormatOutput()
{
    // Поток который используется для форматирования
    std::stringstream OutputString;

    OutputString << "Symbol codes:" << std::endl;
    for (auto LineCursor : (*this))
    {
        auto Line = LineCursor.second;
        for (auto ColumnCursor : Line)
        {
            auto Column = ColumnCursor.second;
            for (auto PageCursor : Column)
            {
                auto code = PageCursor.second;

                if (!code.empty())
                {
                    OutputString << converServiceSymbols(LineCursor.first) << " -> ";
                    OutputString << converServiceSymbols(ColumnCursor.first) << " : ";
                    for (auto binary : code)
                    {
                        // Вывод кода символа
                        OutputString << binary;
                    }
                    OutputString << std::endl;
                }
            }
        }
    }

    // Из потока получем строку
    return OutputString.str();
}

void ExpendedCoderTable::WriteTable(std::string filePath)
{
    std::ofstream out(filePath, std::ios_base::app);
    if (!out.is_open())
        throw "error: " + filePath + "not open";

    // Выводим сформинрованную строку в консоль
    std::cout << this->FormatOutput() << std::endl;
    // Выводим таблицу кодов сиволов в файл
    this->WriteInFile(out);

    out.close();
}

void ExpendedCoderTable::Encrypt(std::string filePath)
{
    std::ofstream out(filePath, std::ios_base::app);
    if (!out.is_open())
        throw "error: " + filePath + "not open";

    this->outCodes(out);
}

void ExpendedCoderTable::Decrypt(std::string EncodedFile, std::string DecodedFile)
{
    std::ofstream out(EncodedFile, std::ios_base::app);
    if (!out.is_open())
        throw "error: " + EncodedFile + "not open";

    std::fstream in(DecodedFile, std::ios::in);
    if (!out.is_open())
        throw "error: " + DecodedFile + "not open";

    this->outDecodedText(out, in);
}