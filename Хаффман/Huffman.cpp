#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <assert.h>

using namespace std;

struct Node
{
	int BinaryCode;		// Число
	char Character;		//Символ
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

map<char, vector<bool>> CreateTable(Node *root) //Заполнение таллицы
{
	// Создание статической переменной для того, чтобы ее значения были одинаковыми во время рекурсии
	static map<char, vector<bool>> table;
	static vector<bool> code;

	// Создание лямбда выражения которое берет по ссылке все значения из CreateTable
	[&]() {
		if (root->left != NULL)
		{
			code.push_back(0);
			CreateTable(root->left);
		}

		if (root->right != NULL)
		{
			code.push_back(1);
			CreateTable(root->right);
		}

		if (root->left == NULL && root->right == NULL)
			table[root->Character] = code;

		code.pop_back(); // Удаление одного символа с конца
	}();

	return table;
}

////// считаем частоты символов
map<char, int> CountOfSymbolsFromFile(ifstream &file)
{
	map<char, int> mapSymbols;

	while (!file.eof())
	{
		char c = file.get();
		mapSymbols[c]++;
	}

	return mapSymbols;
}

////// записываем начальные узлы в список list
list<Node *> InitHuffmanTree(map<char, int> mapSymbols)
{
	list<Node *> HuffmanTree;

	for (map<char, int>::iterator itr = mapSymbols.begin(); itr != mapSymbols.end(); ++itr)
	{
		Node *p = new Node; //Создание в динамической память нового узла
		p->Character = itr->first;
		p->BinaryCode = itr->second;
		HuffmanTree.push_back(p);
	}

	return HuffmanTree;
}

//////  создаем дерево
void FillHuffmanTree(list<Node *> &HuffmanTree)
{
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

int ComputeCount(ifstream &fileIn, ofstream &fileOut, map<char, vector<bool>> table)
{
	int count = 0;
	char buf = 0;
	while (!fileIn.eof()) //Считываение из файла
	{
		char c = fileIn.get();
		vector<bool> x = table[c];
		for (int n = 0; n < x.size(); n++)
		{
			buf = buf | x[n] << (7 - count);
			count++;
			if (count == 8)
			{
				count = 0;
				fileOut << buf;
				buf = 0;
			}
		}
	}

	return count;
}

void Decrypt(Node *root, ifstream &fileIn)
{
	Node *p = root;
	int count = 0;
	char byte;
	byte = fileIn.get();
	while (!fileIn.eof()) // Перевод нулей и единиц обратно в символы
	{
		bool b = byte & (1 << (7 - count));
		if (b)
			p = p->right;
		else
			p = p->left;
		if (p->left == NULL && p->right == NULL)
		{
			cout << p->Character;
			p = root;
		}
		count++;
		if (count == 8)
		{
			count = 0;
			byte = fileIn.get();
		}
	}
}

int main(int argc, char *argv[])
{
	ifstream fileIn("1.txt", ios::out | ios::binary);

	auto mapSymbols = CountOfSymbolsFromFile(fileIn);

	auto HuffmanTree = InitHuffmanTree(mapSymbols);

	FillHuffmanTree(HuffmanTree);

	//root - указатель на вершину дерева
	Node *root = HuffmanTree.front();

	////// создаем пары 'символ-код':

	auto table = CreateTable(root);

	////// Выводим коды в файл output.txt
	fileIn.clear();
	fileIn.seekg(0); // перемещаем указатель снова в начало файла

	ofstream fileOut("output.txt", ios::out | ios::binary);

	auto count = ComputeCount(fileIn, fileOut, table);

	fileIn.close();
	fileOut.close();

	///// считывание из файла output.txt и преобразование обратно

	ifstream DecryptedFile("output.txt", ios::in | ios::binary);

	setlocale(LC_ALL, "Russian"); // чтоб русские символы отображались в командной строке

	Decrypt(root, DecryptedFile);

	DecryptedFile.close();

	return 0;
}
