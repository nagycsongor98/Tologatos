#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <list>

using namespace std;

typedef struct node
{
	int id;
	int** table;
	int g;
	int h;
	int f;
	struct node* parent;
	int back;
}node;

void print(int**);
void printRandomTable();
void readFile();
void readKeyboard();
bool goolState();
bool aStar();
node createNode(node*, int);
int  heuristic_cost_1(int**);
int  heuristic_cost_2(int**);
node getNodeFromOpenWithTheLowestF();
node find(list<node>&, node*);
bool compare(int**, int**);
void remove(list<node>&, node*);
int manhattan(int** , int , int );

int** table;
int n,m;

bool readFromFile = false;
string fileName;

bool solseq = false;

bool pcost = false;
int pcostCounter = 0;

bool nvisited = false;
int nvisitedCounter = 0;

bool h = false;
int H = 0;

list<node> OPEN;
list<node> CLOSE;
int idCouter = 0;

int main(int argc, const char** argv)
{
	try {}
	catch (int*) {}


	if (argc > 1) {
		for (int i = 0; i < argc; i++) {
			if (strcmp(argv[i], "-rand") == 0) {
				sscanf(argv[i + 1], "%d", &n);
				sscanf(argv[i + 2], "%d", &m);
				printRandomTable();
				return 0;
			}
			if (strcmp(argv[i], "-input") == 0) {
				readFromFile = true;
				fileName = argv[++i];
			}
			if (strcmp(argv[i], "-solseq") == 0) {
				solseq = true;
			}
			if (strcmp(argv[i], "-pcost") == 0) {
				pcost = true;
			}
			if (strcmp(argv[i], "-nvisited") == 0) {
				nvisited = true;
			}
			if (strcmp(argv[i], "-h") == 0) {
				h = true;
				sscanf(argv[++i], "%d", &H);
			}
		}
	}
	if (readFromFile) {
		readFile();
	}
	else {
		cout << "insert data\n";
		readKeyboard();
	}

	if (!aStar()) {
		cout << "No solution" << endl;
		return 0;
	}

	if (pcost) {
		cout << "The cost of the solution:" << pcostCounter << endl;
	}
	if (nvisited) {
		cout << "The visited nodes number:" << nvisitedCounter << endl;
	}

	return 0;
}

void print(int** tab) {
	if (solseq) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				printf("%5d ", tab[i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}
}

void printRandomTable() {
	table= new int* [n];
	for (int i = 0; i < n; i++) {
		table[i] = new int[n];
	}
	int counter = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			table[i][j] = counter++;
		}
	}
	srand(time(NULL));
	int x = 0;
	int y = 0;
	int aux;
	for (int i = 0; i < m; i++) {
		int random = rand() % 4;
		switch (random)
		{
		case 0:
			if ((x - 1) >= 0) {
				table[x][y] = table[x - 1][y];
				table[x - 1][y] = 0;
				x--;
			}
			else {
				i--;
			}
			break;
		case 1:
			if ((x + 1) < n) {
				table[x][y] = table[x + 1][y];
				table[x + 1][y] = 0;
				x++;
			}
			else {
				i--;
			}
			break;
		case 2:
			if ((y - 1) >= 0) {
				table[x][y] = table[x][y - 1];
				table[x][y - 1] = 0;
				y--;
			}
			else {
				i--;
			}
			break;
		case 3:
			if ((y + 1) < n) {
				table[x][y] = table[x][y + 1];
				table[x][y + 1] = 0;
				y++;
			}
			else {
				i--;
			}
			break;
		}
	}
	solseq = true;
	print(table);
}

void readFile() {
	ifstream myfile;
	myfile.open(fileName);
	myfile >> n;
	table = new int* [n];
	for (int i = 0; i < n; i++) {
		table[i] = new int[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			myfile >> table[i][j];
		}
	}
	myfile.close();
}
void readKeyboard() {
	cout << "n = ";
	cin >> n;
	cout << endl;
	table = new int* [n];
	for (int i = 0; i < n; i++) {
		table[i] = new int[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << "table[" << i << "][" << j << "] = ";
			cin >> table[i][j];
		}
	}
}
bool goolState() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (table[i][j] != n * i + j) {
				return false;
			}
		}
	}
	return true;
}

bool aStar() {
	node start;
	start.id = idCouter++;
	start.table = table;
	start.g = 0;
	if (H == 2) {
		start.h = heuristic_cost_2(table);
	}
	else {
		start.h = heuristic_cost_1(table);
	}
	start.f = start.g + start.h;
	start.parent = NULL;
	start.back = -1;
	OPEN.push_front(start);
	node N, NCHILD, X;
	while (OPEN.size())
	{
		N = getNodeFromOpenWithTheLowestF();
		remove(OPEN, &N);
		CLOSE.push_front(N);
		nvisitedCounter++;
		if (N.h == 0) {
			pcostCounter = N.g;
			print(N.table);
			return true;
		}
		print(N.table);
		
		for (int i = 0; i < 4; i++) {
			NCHILD = createNode(&N, i);
			if (NCHILD.h < 0) {
				continue;
			}
			
			X = find(OPEN, &NCHILD);
			if (X.h >= 0) {
				if (X.f <= NCHILD.f) {
					continue;
				}
				else {
					remove(OPEN, &X);
				}
			}

			X = find(CLOSE, &NCHILD);
			if (X.h >= 0) {
				if (X.f <= NCHILD.f) {
					continue;
				}
				else {
					remove(CLOSE, &X);
				}
			}
			OPEN.push_front(NCHILD);
		}
	}
	return false;
}

node createNode(node* par, int direction) {
	node res;
	res.h = -1;
	if (direction == par->back) {
		return res;
	}
	int x, y;
	int** aux = new int* [n];
	for (int i = 0; i < n; i++) {
		aux[i] = new int[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			aux[i][j] = par->table[i][j];
			if (par->table[i][j] == 0) {
				x = i;
				y = j;
			}
		}
	}
	switch (direction)
	{
	case 0:
		if ((x - 1) >= 0) {
			aux[x][y] = aux[x - 1][y];
			aux[x - 1][y] = 0;
			res.back = 1;
		}
		else {
			return res;
		}
		break;
	case 1:
		if ((x + 1) < n) {
			aux[x][y] = aux[x + 1][y];
			aux[x + 1][y] = 0;
			res.back = 0;
		}
		else {
			return res;
		}
		break;
	case 2:
		if ((y - 1) >= 0) {
			aux[x][y] = aux[x][y - 1];
			aux[x][y - 1] = 0;
			res.back = 3;
		}
		else {
			return res;
		}
		break;
	case 3:
		if ((y + 1) < n) {
			aux[x][y] = aux[x][y + 1];
			aux[x][y + 1] = 0;
			res.back = 2;
		}
		else {
			return res;
		}
		break;
	}

	res.id = idCouter++;
	res.table = aux;
	res.g = (par->g) + 1;
	if (H == 2) {
		res.h = heuristic_cost_2(aux);
	}
	else {
		res.h = heuristic_cost_1(aux);
	}
	res.f = res.g + res.h;
	res.parent = par;
	return res;
}
int  heuristic_cost_1(int** tab) {
	int res = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tab[i][j] != n * i + j && tab[i][j] !=0) {
				res++;
			}
		}
	}
	return res;
}

int  heuristic_cost_2(int** tab) {
	int res = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			res += manhattan(tab, i, j);
		}
	}
	return res;
}

node getNodeFromOpenWithTheLowestF() {
	node res = OPEN.front();
	for (auto& v : OPEN)
	{
		if (v.f < res.f) {	
			res = v;
		}
		else if (v.f == res.f) {
			if (v.h < res.h) {
				res = v;
			}
		}
	}

	return res;
}

node find(list<node>& container, node* x) {
	node res;
	res.h = -1;
	for (auto& v : container)
	{
		if (compare(x->table, v.table)) {
			return v;
		}
	}

	return res;
}

bool compare(int** a, int** b) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (a[i][j] != b[i][j]) {
				return false;
			}
		}
	}
	return true;
}

void remove(list<node>& container, node* x) {
	for (list<node>::const_iterator itr = container.cbegin(); itr != container.end(); itr++)
	{
		if (itr->id == x->id)
		{
			container.erase(itr--);
			break;
		}
	}
}

int manhattan(int** a, int x, int y) {
	int i = x;
	int j = y;
	int res = 0;
	while (true)
	{
		if (a[x][y] == (n * i) + j || a[x][y] == 0) {
			break;
		}
		else if (a[x][y] > (n * i) + j) {
			if (a[x][y] > (i + 1)* n - 1) {
				res++;
				i++;
			}
			else {
				res++;
				j++;
			}
		}
		else {
			if (a[x][y] < i * n) {
				res++;
				i--;
			}
			else {
				res++;
				j--;
			}
		}

	}
	return res;
}
