#ifndef CODERTABLE_HPP
#define CODERTABLE_HPP

#include "../Huffman/Huffman.hpp"
#include "../CountSymbol/CountSymbols.hpp"

class CoderTable : public std::map<char, std::map<char, std::vector<bool>>>
{
private:
    uint encodingBites = 255;
    char emptyCode = '-';

    // Запись в файл таблицы кодов последовательностей букв
    void WriteInFile(std::ofstream &out)
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

            out << std::endl;
        }
    }

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

    // Полусить таблицу кодов из таблицы количества символов
    void Encrypt(CountSymbols numberOfSymbols)
    {
        for (const auto lineSymbolsCursor : numberOfSymbols)
        {
            auto fullMap = lineSymbolsCursor.second;

            std::map<char, int> mapSymbols;

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
    // Заполнить коды последовательностей букв нулями
    CoderTable();

    // Заполнить коды последовательностей букв по количеству символов
    CoderTable(CountSymbols);

    // Формирует удобо читаемый вывод таблицы
    std::string FormatOutput();

    // Записываем коды последователностей букв в файл
    void WriteTable(std::string);
};

#endif