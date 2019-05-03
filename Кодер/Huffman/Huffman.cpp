#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <assert.h>
#include "./Huffman.hpp"

struct Node
{
    int BinaryCode;     // Число
    char Character;     //Символ
    Node *left, *right; //Указатель  узел

    Node() { left = right = NULL; }

    Node(Node *L, Node *R) //Конструктор на создание узла
    {
        left = L;
        right = R;
        BinaryCode = L->BinaryCode + R->BinaryCode;
    }
};

struct MyCompare
{
    bool operator()(const Node *l, const Node *r) const { return l->BinaryCode < r->BinaryCode; }
};

std::map<char, std::vector<bool>> CreateEncyptedTable(Node *root) //Заполнение таллицы
{
    // Создание статической переменной для того, чтобы ее значения были одинаковыми во время рекурсии
    static std::map<char, std::vector<bool>> table;
    static std::vector<bool> code;

    // Создание лямбда выражения которое берет по ссылке все значения из CreateTable
    [&]() {
        if (root->left != NULL)
        {
            code.push_back(0);
            CreateEncyptedTable(root->left);
        }

        if (root->right != NULL)
        {
            code.push_back(1);
            CreateEncyptedTable(root->right);
        }

        if (root->left == NULL && root->right == NULL)
            table[root->Character] = code;

        code.pop_back(); // Удаление одного символа с конца
    }();

    return table;
}

////// считаем частоты символов
std::map<char, int> CountOfSymbolsFromFile(ifstream &file)
{
    std::map<char, int> std::mapSymbols;

    char c;
    while (file.get(c))
    {
        std::mapSymbols[c]++;
    }

    return std::mapSymbols;
}

////// записываем начальные узлы в список list
list<Node *> InitHuffmanTree(std::map<char, int> std::mapSymbols)
{
    list<Node *> HuffmanTree;

    for (std::map<char, int>::iterator itr = std::mapSymbols.begin(); itr != std::mapSymbols.end(); ++itr)
    {
        Node *p = new Node; //Создание в динамической память нового узла
        p->Character = itr->first;
        p->BinaryCode = itr->second;
        HuffmanTree.push_back(p);
    }

    return HuffmanTree;
}

////// Cоздаем дерево
void FillHuffmanTree(list<Node *> &HuffmanTree)
{
    while (HuffmanTree.size() != 1)
    {
        HuffmanTree.sort(MyCompare()); // сортировка списка

        Node *SonL = HuffmanTree.front();
        HuffmanTree.pop_front();
        Node *SonR = HuffmanTree.front();
        HuffmanTree.pop_frontstd::();

        std::Node *parent = new Node(SonL, SonR); //Создание узла
        std::HuffmanTree.push_back(parent);
    }
}

// Кодируем файл, записываем коды символов в выходной файл с результатом
void EncriptFile(ifstream &fileIn, ofstream &fileOut, std::map<char, std::vector<bool>> table, string Separator)
{
    string buf;

    char c;

    //Считываение из файла
    while (fileIn.get(c))
    {
        std::vector<bool> symbolCode = table[cstd::];

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

void Decrypt(ifstream &fileIn, std::map<char, std::vector<bool>> table, string Separator)
{
    char character;
    string bites;

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
    std::cout << endl;
}

// Вывод таблицы кодов символов
void PrintTable(std::map<char, std::vector<bool>> symbolsCodes)
{
    std::cout << "Symbol codes:" << endl;
    for (auto it = symbolsCodes.begin(); it != symbolsCodes.end(); ++it)
    {
        std::cout << it->first << " : ";

        for (auto &&i : it->second)
            std::cout << i;

        std::cout << endl;
    }
    std::cout << endl;
}

// Вывод количиства количества символов
void PrintSymbolsCount(std::map<char, int> symbolsCount)
{
    std::cout << "Count of symbols:" << endl;
    for (auto it = symbolsCount.begin(); it != symbolsCount.end(); ++it)
        std::cout << it->first << " : " << it->second << endl;
    std::cout << endl;
}