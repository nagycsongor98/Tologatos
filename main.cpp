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
}node;

void print(int**);
void printRandomTable();
void readFile();
void readKeyboard();
bool goolState();
bool aStar();
node createNode(node*, int);
int  heuristic_cost_1(int**);
node getNodeFromOpenWithTheLowestF();
node find(list<node>&, node*);
bool compare(int**, int**);
void remove(list<node>&, node*);

int** table;
int n,m;

bool readFromFile = false;
string fileName;

bool solseq = false;

bool pcost = false;
int counter = 0;

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
		print(table);
	}
	else {
		cout << "insert data\n";
		readKeyboard();
		print(table);
	}

	aStar();

	if (pcost) {
		cout << "The cost of the solution:" << counter << endl;
	}
	if (nvisited) {
		cout << "The visited nodes number:" << nvisitedCounter << endl;
	}

	//node elso;
	//elso.table = table;
	//elso.f = 3;
	//elso.g = 555;
	//elso.h = 4;
	//elso.parent = NULL;

	//node masodik;
	//masodik.table = table;
	//masodik.f = 10;
	//masodik.g = 35;
	//masodik.h = 43;
	//masodik.parent = &elso;

	////cout << "jooohgho " << masodik.parent->g;
	//OPEN.push_front(elso);
	//OPEN.push_front(masodik);
	//cout << "open: " << OPEN.size() << endl;
	//cout << "close: "<<CLOSE.size() << endl;

	//for (auto& v : OPEN)
	//{
	//	cout << v.f<<endl;
	//}
	//
	//for (list<node>::const_iterator itr = OPEN.cbegin(); itr != OPEN.end(); itr++)
	//{
	//	// remove strings having length 5
	//	if (itr->g == 555)
	//	{
	//		// decrement iterator after it is passed to erase()
	//		// but before erase() is executed 
	//		OPEN.erase(itr--);
	//	}
	//}

	//cout << "open: " << OPEN.size() << endl;
	//cout << "close: " << CLOSE.size() << endl;

	//for (auto& v : OPEN)
	//{
	//	cout << v.f << " ";
	//}

	/*node start;
	start.id = idCouter++;
	start.table = table;
	start.g = 0;
	start.h = heuristic_cost_1(table);
	start.f = start.g + start.h;
	start.parent = NULL;
	OPEN.push_front(start);
	table[0][0] = 55;
	OPEN.push_front(createNode(&start));
	table[0][0] = 66;
	OPEN.push_front(createNode(&start));

	for (auto& v : OPEN)
	{
		printf("\n");
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				printf("%5d ", v.table[i][j]);
			}
			printf("\n");
		}
		printf("\n%d\n", v.f);
	}*/

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
	cin >> n;
	table = new int* [n];
	for (int i = 0; i < n; i++) {
		table[i] = new int[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
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
	start.g = heuristic_cost_1(table);
	start.h = heuristic_cost_1(table);
	start.f = start.g + start.h;
	start.parent = NULL;
	OPEN.push_front(start);
	node N, NCHILD, X;
	while (OPEN.size()<50)
	{
		N = getNodeFromOpenWithTheLowestF();
		CLOSE.push_front(N);
		if (N.h == 0) {
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
		cout << "-------------------\n";
		for (auto& v : OPEN)
		{
			printf("\n");
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					printf("%5d ", v.table[i][j]);
				}
				printf("\n");
			}
			printf("\nH %d\n", v.h);
			printf("\nF %d\n", v.f);
		}
		cout << "-------------------\n";
	}
	return false;
}

node createNode(node* par, int direction) {
	node res;
	res.h = -1;
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
		}
		else {
			return res;
		}
		break;
	case 1:
		if ((x + 1) < n) {
			aux[x][y] = aux[x + 1][y];
			aux[x + 1][y] = 0;
		}
		else {
			return res;
		}
		break;
	case 2:
		if ((y - 1) >= 0) {
			aux[x][y] = aux[x][y - 1];
			aux[x][y - 1] = 0;
		}
		else {
			return res;
		}
		break;
	case 3:
		if ((y + 1) < n) {
			aux[x][y] = aux[x][y + 1];
			aux[x][y + 1] = 0;
		}
		else {
			return res;
		}
		break;
	}

	res.id = idCouter++;
	res.table = aux;
	res.g = par->g;
	res.h = heuristic_cost_1(aux);
	res.f = res.g + res.h;
	res.parent = par;
	return res;
}
int  heuristic_cost_1(int** tab) {
	int res = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tab[i][j] != n * i + j && tabtab[i][j] !=0) {
				res++;
			}
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