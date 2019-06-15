
#include <iostream>
#include <sstream>
#include "./ExpendedCountSymbols.hpp"

ExpendedCountSymbols::ExpendedCountSymbols() : map()
{
    // Заполняем количество символов нулями.
    for (int line = 0; line < encodingBites; line++)
    {
        for (int column = 0; column < encodingBites; column++)
        {
            for (int page = 0; page < encodingBites; page++)
            {
                (*this)[line][column][page] = 0;
            }
        }
    }
}

ExpendedCountSymbols::ExpendedCountSymbols(std::string text)
{
    this->CountingForText(text);
}

std::string ExpendedCountSymbols::FormatOutput()
{
    std::stringstream OutputString;
    for (auto LineCursor : (*this))
    {
        auto Line = LineCursor.second;
        for (auto ColumnCursor : Line)
        {
            auto Column = ColumnCursor.second;

            for (auto PageCursor : Column)
            {
                auto number = PageCursor.second;
                auto FirstSymbol = converServiceSymbols(LineCursor.first);
                auto SecondSymdol = converServiceSymbols(ColumnCursor.first);
                auto ThirdSymbol = converServiceSymbols(PageCursor.first);
                
                if (number != 0)
                {
                    OutputString << "Symbol: " << FirstSymbol << " -> " << SecondSymdol << " -> "<< ThirdSymbol << " - " <<  number << std::endl;
                }
            }
        }
    }

    // Из потока получем строку
    return OutputString.str();
}

void ExpendedCountSymbols::WriteTable(std::string filePath)
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