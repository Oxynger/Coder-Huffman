#ifndef CODERTABLE_HPP
#define CODERTABLE_HPP

#include "../Huffman/Huffman.hpp"
#include "../CountSymbol/CountSymbols.hpp"

// следуемся от контейнера map и делаем свою реализацию для работы с таблицей кодов хафмана
class CoderTable : public std::map<char, std::map<char, std::vector<bool>>>
{
private:
    int encodingBites = 255;
    char emptyCode = '-';
    std::string text = "";
    std::string Separator = " ";

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
    void encryptTable(CountSymbols numberOfSymbols)
    {
        for (const auto lineSymbolsCursor : numberOfSymbols)
        {
            auto fullMap = lineSymbolsCursor.second;

            std::map<char, int> mapSymbols;

            // Собираем новый хэш со значениями != 0 что бы были только коды тех символов с которыми мы работаем
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

    // Записываем закодированный текст
    void outCodes(std::ofstream &out)
    {
        auto firstSymbol = this->text.at(0);

        // Удаляем первый символ, он уже не нужен
        this->text.erase(0,1);

        // Записываем первый символ для декодирования
        out << firstSymbol;
        for (auto c : this->text)
        {
            for (auto binary : this->at(firstSymbol).at(c))
            {
                out << binary;
            }
            out << this->Separator;

            firstSymbol = c;
        }
    }

    // Записываем декодированный текст
    void outDecodedText(std::ofstream &out, std::fstream &in)
    {
        char symbol;
        std::string bites;
        std::vector<bool> charactersMask;

        char firstSymbol = in.get();
        out << firstSymbol;

        // Конвертирует строку с кодом символа в массив бит
        auto stringToMask = [&]() {
            for (auto bite : bites)
                // Если код символа равен символу 1 то записываем true иначе false
                charactersMask.push_back(bite == '1');
        };

        // Поиск символа по битовой маске ( коду ) в таблице с алфавитом
        auto findSymbolByCode = [&]() {
            for (auto it : (*this).at(firstSymbol))
            {
                if (it.second == charactersMask)
                {
                    out << it.first;
                    firstSymbol = it.first;
                }
            }
        };

        while (in.get(symbol))
        {
            // Прверка что мы не дошли до разделителя
            if (symbol != *this->Separator.data())
            {
                bites += symbol;
            }
            else
            {
                stringToMask();
                findSymbolByCode();

                charactersMask.clear();
                bites.clear();
            }
        }
    }

public:
    // Заполнить коды последовательностей букв нулями
    CoderTable();

    // Заполнить коды последовательностей букв по количеству символов
    CoderTable(CountSymbols);

    // Запоминает строку для работы
    CoderTable(CountSymbols, std::string);

    // Формирует удобо читаемый вывод таблицы
    std::string FormatOutput();

    // Записываем коды последователностей букв в файл
    void WriteTable(std::string);

    // Закодирвоать сообщение
    void Encrypt(std::string);

    // Раскодировать сообщение
    void Decrypt(std::string, std::string);
};

#endif