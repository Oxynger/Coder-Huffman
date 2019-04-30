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

	char c;
	while (file.get(c))
	{
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

////// Cоздаем дерево
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

// Кодируем файл, записываем коды символов в выходной файл с результатом
void EncriptFile(ifstream &fileIn, ofstream &fileOut, map<char, vector<bool>> table, string Separator)
{
	string buf;

	char c;

	//Считываение из файла
	while (fileIn.get(c))
	{
		vector<bool> symbolCode = table[c];

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

void Decrypt(ifstream &fileIn, map<char, vector<bool>> table, string Separator)
{
	char character;
	string bites;

	vector<bool> characterMask;

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
				cout << it->first;
			}
		}
	};

	// Перевод нулей и единиц обратно в символы и вывод результата на консоль
	cout << "Resault of decoding: ";
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
	cout << endl;
}

// Вывод таблицы кодов символов
void PrintTable(map<char, vector<bool>> symbolsCodes)
{
	cout << "Symbol codes:" << endl;
	for (auto it = symbolsCodes.begin(); it != symbolsCodes.end(); ++it)
	{
		cout << it->first << " : ";

		for (auto &&i : it->second)
			cout << i;

		cout << endl;
	}
	cout << endl;
}

// Вывод количиства количества символов
void PrintSymbolsCount(map<char, int> symbolsCount)
{
	cout << "Count of symbols:" << endl;
	for (auto it = symbolsCount.begin(); it != symbolsCount.end(); ++it)
		cout << it->first << " : " << it->second << endl;
	cout << endl;
}

int main(int argc, char *argv[])
{
	ifstream fileIn("1.txt", ios::out | ios::binary);

	auto mapSymbols = CountOfSymbolsFromFile(fileIn);

	PrintSymbolsCount(mapSymbols);

	auto HuffmanTree = InitHuffmanTree(mapSymbols);

	FillHuffmanTree(HuffmanTree);

	//root - указатель на вершину дерева
	Node *root = HuffmanTree.front();

	////// создаем пары 'символ-код':

	auto table = CreateTable(root);

	PrintTable(table);
	////// Выводим коды в файл output.txt
	fileIn.clear();
	fileIn.seekg(0); // перемещаем указатель снова в начало файла

	ofstream fileOut("output.txt", ios::out | ios::binary);

	// Разделитель битов для записи в файл и декодирования
	string Separator = " ";
	EncriptFile(fileIn, fileOut, table, Separator);

	fileIn.close();
	fileOut.close();

	///// считывание из файла output.txt и преобразование обратно

	ifstream DecryptedFile("output.txt", ios::in | ios::binary);

	setlocale(LC_ALL, "Russian"); // чтоб русские символы отображались в командной строке

	Decrypt(DecryptedFile, table, Separator);

	DecryptedFile.close();

	return 0;
}
