
#include <iostream>
#include <sstream>
#include "./CountSymbols.hpp"

CountSymbols::CountSymbols() : map()
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

CountSymbols::CountSymbols(std::string text)
{
    this->CountingForText(text);
}

std::string CountSymbols::FormatOutput()
{
    std::stringstream OutputString;
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
                OutputString << "Symbol: " << FirstSymbol << " -> " << SecondSymdol << " - " << Number << std::endl;
            }
        }
    }

    // Из потока получем строку
    return OutputString.str();
}

void CountSymbols::WriteTable(std::string filePath)
{
    std::ofstream out(filePath, std::ios_base::app);

    if (!out.is_open())
        throw "error: " + filePath + "not open";

    // Выводим сформинрованную строку в консоль
    std::cout << this->FormatOutput() << std::endl;
    // Выводим таблицу с количеством сиволов в файл
    this->WriteInFile(out);

    out.close();
}