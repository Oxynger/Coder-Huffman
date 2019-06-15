#ifndef EXPENDEDCOUNTSYMBOL_HPP
#define EXPENDEDCOUNTSYMBOL_HPP

#include <map>
#include <fstream>

// Наследуемся от контейнера map и делаем свою реализацию для хранения количества символов в тексте
class ExpendedCountSymbols : public std::map<char, std::map<char, std::map<char, int>>>
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
        for (auto LineCursor : (*this))
        {
            auto Line = LineCursor.second;

            for (auto ColumnCursor : Line)
            {
                auto Column = ColumnCursor.second;
                for (auto Page : Column)
                {
                    out << Page.second << " ";
                }
            }

            out << std::endl;
        }
    }

    // Подсчет количество последовательностей символов из текста
    void CountingForText(const std::string &input_str)
    {
        std::string str = input_str;

        char FirstLetter = str.at(0);
        char SecondLetter = str.at(1);

        for (int i = 2; i < str.length(); i++)
        {
            char ThirdLetter = str.at(i);

            (*this)[FirstLetter][SecondLetter][ThirdLetter]++;

            FirstLetter = SecondLetter;
            SecondLetter = ThirdLetter;
        }
    }

public:
    // Конструктор заполняющий таблицу нулямиы
    ExpendedCountSymbols();

    // Конструктор который по строке строит таблицу с количествами последовательностей символов в ней
    ExpendedCountSymbols(std::string);

    // Формирует удобо читаемый вывод таблицы
    std::string FormatOutput();

    // Записываем количесвто символов в файл название которого переданно
    void WriteTable(std::string);
};

#endif