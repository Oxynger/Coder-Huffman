#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <assert.h>
#include <functional>
#include "./Huffman.hpp"

// struct Node
// {
//     int BinaryCode;     // Число
//     char Character;     //Символ
//     Node *left, *right; //Указатель  узел

//     Node() { left = right = NULL; }

//     Node(Node *L, Node *R) //Конструктор на создание узла
//     {
//         left = L;
//         right = R;
//         BinaryCode = L->BinaryCode + R->BinaryCode;
//     }
// };

// Определение логики конструкторов
Node::Node() { left = right = NULL; }

Node::Node(Node *L, Node *R)
{
    left = L;
    right = R;
    BinaryCode = L->BinaryCode + R->BinaryCode;
}

bool MyCompare::operator()(const Node *l, const Node *r) const { return l->BinaryCode < r->BinaryCode; }

std::map<char, std::vector<bool>> CreateEncyptedTable(Node *root) //Заполнение таллицы
{
    if (root == NULL)
        return std::map<char, std::vector<bool>>();

    // Создание статической переменной для того, чтобы ее значения были одинаковыми во время рекурсии
    std::map<char, std::vector<bool>> table;
    std::vector<bool> code;

    // Создание лямбда выражения которое берет по ссылке все значения из CreateTable
    std::function<void(Node *)> encryptTable = [&code, &table, &encryptTable](Node *leaf) {
        if (leaf->left != NULL)
        {
            code.push_back(0);
            encryptTable(leaf->left);
        }

        if (leaf->right != NULL)
        {
            code.push_back(1);
            encryptTable(leaf->right);
        }

        // Если нам приходит текст из 1-ого символа то код пустой, что бы этого избежать мы записываем код для этого символа = 0
        if (code.empty())
        {
            code.push_back(0);
        }

        if (leaf->left == NULL && leaf->right == NULL)
        {
            table[leaf->Character] = code;
        }

        // code.pop_back(); // Удаление одного символа с конца
    };

    encryptTable(root);

    return table;
}

////// считаем частоты символов
std::map<char, int> CountOfSymbolsFromFile(std::ifstream &file)
{
    std::map<char, int> mapSymbols;

    char c;
    while (file.get(c))
    {
        mapSymbols[c]++;
    }

    return mapSymbols;
}

////// записываем начальные узлы в список list
std::list<Node *> InitHuffmanTree(std::map<char, int> mapSymbols)
{
    std::list<Node *> HuffmanTree;

    for (std::map<char, int>::iterator itr = mapSymbols.begin(); itr != mapSymbols.end(); ++itr)
    {
        Node *p = new Node; //Создание в динамической память нового узла
        p->Character = itr->first;
        p->BinaryCode = itr->second;
        HuffmanTree.push_back(p);
    }

    return HuffmanTree;
}

////// Cоздаем дерево
void FillHuffmanTree(std::list<Node *> &HuffmanTree)
{
    if (HuffmanTree.empty())
        return;

    while (HuffmanTree.size() != 1)
    {
        HuffmanTree.sort(MyCompare()); // сортировка списка

        Node *SonL = HuffmanTree.front();
        HuffmanTree.pop_front();
        Node *SonR = HuffmanTree.front();
        HuffmanTree.pop_front();

        Node *parent = new Node(SonL, SonR); //Создание узла
        HuffmanTree.push_back(parent);
    }
}

// Кодируем файл, записываем коды символов в выходной файл с результатом
void EncriptFile(std::ifstream &fileIn, std::ofstream &fileOut, std::map<char, std::vector<bool>> table, std::string Separator)
{
    std::string buf;

    char c;

    //Считываение из файла
    while (fileIn.get(c))
    {
        std::vector<bool> symbolCode = table[c];

        // Коверитрируем из вектора булевых значений в строку
        for (auto i : symbolCode)
        {
            buf.append(to_string(i));
        }

        // Добавляем разделитель к кодам закодированных символов
        buf.append(Separator);

        // Записываем строку с кодами символов в файл
        fileOut << buf;

        // Очищаем файл с кодами после записи
        buf.clear();
    }
}

void Decrypt(std::ifstream &fileIn, std::map<char, std::vector<bool>> table, std::string Separator)
{
    char character;
    std::string bites;

    std::vector<bool> characterMask;

    // Конвертирует строку с кодом символа в
    auto stringToMask = [&]() {
        for (auto bite : bites)
            // Если код символа равен символу 1 то записываем true иначе false
            characterMask.push_back(bite == '1');
    };

    // Поиск символа по битовой маске ( коду ) в таблице с алфавитом
    auto findSymbolByCode = [&]() {
        for (auto it = table.begin(); it != table.end(); ++it)
        {
            if (it->second == characterMask)
            {
                std::cout << it->first;
            }
        }
    };

    // Перевод нулей и единиц обратно в символы и вывод результата на консоль
    std::cout << "Resault of decoding: ";
    while (fileIn.get(character))
    {
        // Прверка что мы не дошли до разделителя
        if (character != *Separator.data())
        {
            bites += character;
        }

        else
        {
            stringToMask();
            findSymbolByCode();

            characterMask.clear();
            bites.clear();
        }
    }
    std::cout << std::endl;
}

// Вывод таблицы кодов символов
void PrintTable(std::map<char, std::vector<bool>> symbolsCodes)
{
    if (symbolsCodes.empty())
        return;

    std::cout << "Symbol codes:" << std::endl;
    for (auto it = symbolsCodes.begin(); it != symbolsCodes.end(); ++it)
    {
        std::cout << it->first << " : ";

        for (auto &&i : it->second)
            std::cout << i;

        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Вывод количиства количества символов
void PrintSymbolsCount(std::map<char, int> symbolsCount)
{
    std::cout << "Count of symbols:" << std::endl;
    for (auto it = symbolsCount.begin(); it != symbolsCount.end(); ++it)
        std::cout << it->first << " : " << it->second << std::endl;
    std::cout << std::endl;
}