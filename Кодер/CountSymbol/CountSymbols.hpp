#ifndef COUNTSYMBOL_HPP
#define COUNTSYMBOL_HPP

#include <map>
#include <fstream>

class CountSymbols : public std::map<char, std::map<char, int>>
{
private:
    const int encodingBites = 255;

    // Функция которая заменяет служебные символы на их текст для вывода
    std::string converServiceSymbols(char Symbol)
    {
        switch ((int)Symbol)
        {
        case 13:
            return "Enter";

        case 32:
            return "Space";
        default:
            return std::string(1, Symbol);
        }
        return "";
    }

    // Запись в файл таблицы с количиством последовательностей символов
    void WriteInFile(std::ofstream &out)
    {
        for (auto LineCursor = this->begin(); LineCursor != this->end(); LineCursor++)
        {
            auto Line = LineCursor->second;

            for (auto Number = Line.begin(); Number != Line.end(); Number++)
            {
                out << Number->second << " ";
            }

            out << std::endl;
        }
    }

    // Подсчет количество последовательностей символов из текста
    void CountingForText(const std::string &input_str)
    {
        std::string str = input_str;

        char FirstLetter = str.at(0);

        for (int i = 1; i < str.length(); i++)
        {
            char SecondLetter = str.at(i);

            (*this)[FirstLetter][SecondLetter]++;

            FirstLetter = SecondLetter;
        }
    }

public:
    // Конструктор заполняющий таблицу нулямиы
    CountSymbols();

    // Конструктор который по строке строит таблицу с количествами последовательностей символов в ней
    CountSymbols(std::string);

    // Формирует удобо читаемый вывод таблицы
    std::string FormatOutput();

    // Записываем количесвто символов в файл название которого переданно
    void WriteTable(std::string);
};

#endif