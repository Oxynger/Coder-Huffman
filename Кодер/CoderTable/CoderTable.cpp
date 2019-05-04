
#include <sstream>
#include <fstream>
#include <iostream>

#include "./CoderTable.hpp"
#include "../CountSymbol/CountSymbols.hpp"

CoderTable::CoderTable() : map()
{
    // Заполняем коды символов 0-ми
    for (int line = 0; line < encodingBites; line++)
    {
        for (int column = 0; column < encodingBites; column++)
        {
            (*this)[line][column] = std::vector<bool>(0);
        }
    }
}

CoderTable::CoderTable(CountSymbols count)
{
    CoderTable c;
    c.Encrypt(count);

    (*this) = c;
}

std::string CoderTable::FormatOutput()
{
    std::stringstream OutputString;

    OutputString << "Symbol codes:" << std::endl;
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
                OutputString << std::endl;
            }
        }
    }

    return OutputString.str();
}

void CoderTable::WriteTable(std::string filePath)
{
    std::ofstream out(filePath, std::ios_base::app);

    if (out.is_open())
    {
        std::cout << this->FormatOutput() << std::endl;
        this->WriteInFile(out);
    }

    out.close();
}