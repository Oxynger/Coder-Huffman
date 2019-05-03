#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <map>
#include <vector>
#include <list>
#include <string>

struct Node
{
    int BinaryCode;     // Число
    char Character;     //Символ
    Node *left, *right; //Указатель  узел

    Node();

    Node(Node *, Node *); //Конструктор на создание узла
};

struct MyCompare
{
    bool operator()(const Node *, const Node *) const;
};

// Вывод количиства количества символов
void PrintSymbolsCount(std::map<char, int>);

std::map<char, std::vector<bool>> CreateEncyptedTable(Node *); //Заполнение таллицы

////// считаем частоты символов
std::map<char, int> CountOfSymbolsFromFile(std::ifstream &);

////// записываем начальные узлы в список list
std::list<Node *> InitHuffmanTree(std::map<char, int>);

////// Cоздаем дерево
void FillHuffmanTree(std::list<Node *> &);

// Кодируем файл, записываем коды символов в выходной файл с результатом
void EncriptFile(std::ifstream &, std::ofstream &, std::map<char, std::vector<bool>>, std::string);

// Декодирует файл и выводит результат на консоль
void Decrypt(std::ifstream &, std::map<char, std::vector<bool>>, std::string);

// Вывод таблицы кодов символов
void PrintTable(std::map<char, std::vector<bool>>);

// Вывод количиства количества символов
void PrintSymbolsCount(std::map<char, int>);

#endif