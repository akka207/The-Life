#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

const int size1 = 29, size2 = 100;
bool mat[size1][size2];
int colorMat[size1][size2] {};
HANDLE outHd = GetStdHandle(STD_OUTPUT_HANDLE);
int menu = 1;

struct coord
{
	int x, y;
	coord(int x = 0, int y = 0) :x(x), y(y) { }
	coord(const coord& cd) :x(cd.x), y(cd.y) {}
	void Border(int minx, int maxx, int miny, int maxy)
	{
		if (x < minx)x = minx;
		if (x > maxx)x = maxx;
		if (y < miny)y = miny;
		if (y > maxy)y = maxy;
	}
};
class Rules
{
	bool EmptyAlive[9]{};
	bool AliveAlive[9]{};
public:
	Rules(const bool* EmptyAlive, const bool* AliveAlive)
	{
		memcpy(this->EmptyAlive, EmptyAlive, 9);
		memcpy(this->AliveAlive, AliveAlive, 9);
	}
	Rules()
	{
		bool tmp1[] = { 0,0,0,1,0,0,0,0,0 };
		memcpy(EmptyAlive, tmp1, 9);
		bool tmp2[] = { 0,0,1,1,0,0,0,0,0 };
		memcpy(AliveAlive, tmp2, 9);
	}
	friend void life();
	friend int SetRules();
}rules;
class Grid
{
	string name;
	bool matr[size1][size2]{};
public:
	Grid(string name, bool(&matr)[29][100]) :name(name)
	{
		memcpy(this->matr, matr, size1 * size2);
	}
	Grid()
	{
		name = "no name";
		for (int i = 0; i < size1; i++)
			for (int j = 0; j < size2; j++)
				matr[i][j] = 0;
	}

	void Use()
	{
		memcpy(mat, this->matr, size1 * size2);
	}

	auto GetMat() const { return matr; }
	auto GetName() const { return name; }
};
class GridGreater
{
	bool _less;
public:
	explicit GridGreater(bool less = true) :_less(less) {}
	constexpr bool operator()(const Grid& left, const Grid& right) const
	{
		return _less ? left.GetName() < right.GetName() : left.GetName() > right.GetName();
	}
};

list<Grid> matList;

void Cursor(coord& cd, short color = 240, coord dir = { 0,0 }, int length = 0)
{
	SetConsoleTextAttribute(outHd, color);
	if (dir.x == 0 && dir.y == 0)
	{
		SetConsoleCursorPosition(outHd, { (short)cd.y,(short)cd.x });
		printf("%c", char(254));
	}
	else if (dir.x == 1 && dir.y == 0)
	{
		cd.Border(0, 29, 0, 119 - length);
		SetConsoleCursorPosition(outHd, { (short)cd.y,(short)cd.x });
		printf("%s>", string(length, '-').c_str());
	}
	else if (dir.x == -1 && dir.y == 0)
	{
		cd.Border(0, 29, 0, 119 - length);
		SetConsoleCursorPosition(outHd, { (short)cd.y,(short)cd.x });
		printf("<%s", string(length, '-').c_str());
	}
	else if (dir.x == 0 && dir.y == 1)
	{
		cd.Border(0, 29 - length, 0, 119);
		SetConsoleCursorPosition(outHd, { (short)cd.y,(short)cd.x });
		printf("^");
		for (int i = 0; i < length; i++)
		{
			SetConsoleCursorPosition(outHd, { (short)cd.y,(short)(cd.x + i + 1) });
			printf("|");
		}
	}
	else if (dir.x == 0 && dir.y == -1)
	{
		cd.Border(0, 29 - length, 0, 119);
		for (int i = 0; i < length; i++)
		{
			SetConsoleCursorPosition(outHd, { (short)cd.y,(short)(cd.x + i) });
			printf("|");
		}
		SetConsoleCursorPosition(outHd, { (short)cd.y,(short)(cd.x + length) });
		printf("V");
	}
	SetConsoleTextAttribute(outHd, 15);
}
void SetPixel(char pixel, coord cd, short colour = 15)
{
	SetConsoleCursorPosition(outHd, { (short)cd.x,(short)cd.y });
	SetConsoleTextAttribute(outHd, colour);
	printf("%c", char(pixel));
	SetConsoleTextAttribute(outHd, 15);
	SetConsoleCursorPosition(outHd, { 0,0 });
}
void SetPixel(string str, coord cd, short colour = 15)
{
	SetConsoleCursorPosition(outHd, { (short)cd.x,(short)cd.y });
	SetConsoleTextAttribute(outHd, colour);
	printf("%s", str.c_str());
	SetConsoleTextAttribute(outHd, 15);
	SetConsoleCursorPosition(outHd, { 0,0 });
}
void SetPixel(char pixel, coord from, coord to, short colour = 15)
{
	for (short i = from.x; i < to.x; i++)
	{
		for (short j = from.y; j < to.y; j++)
		{
			SetConsoleCursorPosition(outHd, { i,j });
			SetConsoleTextAttribute(outHd, colour);
			printf("%c", char(pixel));
		}
	}
	SetConsoleTextAttribute(outHd, 15);
	SetConsoleCursorPosition(outHd, { 0,0 });
}
void PrintGrid()
{
	SetConsoleCursorPosition(outHd, { 0,0 });
	for (int i = 0; i < size1; i++)
	{
		for (int j = 0; j < size2; j++)
		{
			if (!mat[i][j])
			{
				SetConsoleTextAttribute(outHd, 0);
			}
			else if (colorMat[i][j] < 10)
			{
				SetConsoleTextAttribute(outHd, 170);
			}
			else if (colorMat[i][j] < 20)
			{
				SetConsoleTextAttribute(outHd, 174);
			}
			else if (colorMat[i][j] < 30)
			{
				SetConsoleTextAttribute(outHd, 238);
			}
			else if (colorMat[i][j] < 40)
			{
				SetConsoleTextAttribute(outHd, 230);
			}
			else if (colorMat[i][j] < 50)
			{
				SetConsoleTextAttribute(outHd, 102);
			}
			else if (colorMat[i][j] < 60)
			{
				SetConsoleTextAttribute(outHd, 100);
			}
			else
			{
				SetConsoleTextAttribute(outHd, 68);
			}

			printf("%c", mat[i][j] ? char(177) : ' ');
		}
		if (i + 1 != size1)printf("\n");
	}
	SetConsoleTextAttribute(outHd, 15);
}
void RenewColorGrid()
{
	for (int i = 0; i < size1; i++)
		for (int j = 0; j < size2; j++)
			if (mat[i][j])
				colorMat[i][j]++;
			else
				colorMat[i][j] = 0;
}
void CleanColorMat()
{
	for (int i = 0; i < size1; i++)
		for (int j = 0; j < size2; j++)
			colorMat[i][j] = 0;
}

void SetRandomCell()
{
	coord cd(rand() % size1, rand() % size2);
	if (mat[cd.x][cd.y] == 1)
		SetRandomCell();
	else
		mat[cd.x][cd.y] = 1;
}
void SpawnGlyder()
{
	coord cd(rand() % size1 - 5, rand() % size2 - 5);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			mat[cd.x + i][cd.y + j] = 0;
		}
	}
	mat[cd.x + 1][cd.y + 2] = 1;
	mat[cd.x + 2][cd.y + 3] = 1;
	mat[cd.x + 3][cd.y + 1] = 1;
	mat[cd.x + 3][cd.y + 2] = 1;
	mat[cd.x + 3][cd.y + 3] = 1;
}

int checkNeig(coord cell)
{
	int count = 0;
	if (cell.x - 1 >= 0 && cell.y - 1 >= 0 && mat[cell.x - 1][cell.y - 1] == true) count++;
	if (cell.y - 1 >= 0 && mat[cell.x][cell.y - 1] == true) count++;
	if (cell.x + 1 < size1 && cell.y - 1 >= 0 && mat[cell.x + 1][cell.y - 1] == true) count++;
	if (cell.x + 1 < size1 && mat[cell.x + 1][cell.y] == true) count++;
	if (cell.x + 1 < size1 && cell.y + 1 < size2 && mat[cell.x + 1][cell.y + 1] == true) count++;
	if (cell.y + 1 < size2 && mat[cell.x][cell.y + 1] == true) count++;
	if (cell.x - 1 >= 0 && cell.y + 1 < size2 && mat[cell.x - 1][cell.y + 1] == true) count++;
	if (cell.x - 1 >= 0 && mat[cell.x - 1][cell.y] == true) count++;
	return count;
}
int Count()
{
	int count = 0;
	for (int i = 0; i < size1; i++)
		for (int j = 0; j < size2; j++)
			if (mat[i][j] == 1)
				count++;
	return count;
}
string GenerateName(const char* name)
{
	string res(name);
	if (find_if(matList.begin(), matList.end(), [&name](Grid gd) {return !strcmp(gd.GetName().c_str(), name); }) != matList.end())
	{
		res += " copy";
		res = GenerateName(res.c_str());
	}
	return res;
}

void SaveList(const char* path = "base.bin")
{
	FILE* f = fopen(path, "wb");
	int tmp = matList.size();
	fwrite(&tmp, sizeof(int), 1, f);

	for (auto item : matList)
	{
		tmp = item.GetName().size();
		fwrite(&tmp, sizeof(int), 1, f);
		fwrite(item.GetName().c_str(), sizeof(char), tmp, f);

		fwrite(item.GetMat(), sizeof(bool), size1 * size2, f);
	}
	fclose(f);
}
void LoadList(const char* path = "base.bin")
{
	matList.clear();
	FILE* f = fopen(path, "rb");
	if (f)
	{
		int size;
		fread(&size, sizeof(int), 1, f);
		for (int i = 0; i < size; i++)
		{
			int tmp = 0;
			fread(&tmp, sizeof(int), 1, f);
			char buf[250];
			fread(buf, sizeof(char), tmp, f);
			buf[tmp] = '\0';
			bool mmat[size1][size2]{};
			fread(mmat, sizeof(bool), size1 * size2, f);

			matList.insert(matList.begin(), Grid(buf, mmat));
		}
		fclose(f);
	}
	else
	{
		SaveList();
	}
}

void life()
{
	bool newmat[size1][size2] = {};
	for (int i = 0; i < size1; i++)
	{
		for (int j = 0; j < size2; j++)
		{
			int count = checkNeig({ i,j });

			if (mat[i][j])
				newmat[i][j] = rules.AliveAlive[count];
			else
				newmat[i][j] = rules.EmptyAlive[count];
		}
	}
	RenewColorGrid();
	memcpy(mat, newmat, size1 * size2);
}
int SetGrid()
{
	system("cls");
	coord cursor(0, 0);
	string tmp;

	SetPixel("w a s d - moving   e - enter   c - clear grid   f - exit   k - save   l - load", { 0,29 }, 128);
	while (true)
	{
		PrintGrid();
		cursor.Border(0, 28, 0, 89);
		Cursor(cursor, 15);
		switch (_getch())
		{
		case 'w':
			cursor.x--;
			break;
		case 's':
			cursor.x++;
			break;
		case 'a':
			cursor.y--;
			break;
		case 'd':
			cursor.y++;
			break;
		case 'e':
			mat[cursor.x][cursor.y] = !mat[cursor.x][cursor.y];
			break;
		case 'f':
			menu = 1;
			return 0;
		case 'k':
			char buf[250];
			system("cls");
			printf("Enter name: ");
			cin.getline(buf, 250);
			matList.insert(matList.begin(), Grid(GenerateName(buf), mat));
			SaveList();
			return 0;
		case 'l':
			menu = 5;
			return 0;
		case 'c':
			for (int i = 0; i < size1; i++)
				for (int j = 0; j < size2; j++)
					mat[i][j] = false;
			return 0;
		}
	}
}
int SetRules()
{
	system("cls");
	SetPixel("Alive - Alive", { 1,0 });
	SetPixel("Empty - Alive", { 21,0 });
	for (int i = 0; i < 9; i++)
	{
		SetPixel(to_string(i) + " - ", { 4,1 + i });
		SetPixel(to_string(i) + " - ", { 24,1 + i });
	}
	coord cursor(0, 0);
	SetPixel("w a s d - moving   e - enter   f - exit", { 0,29 }, 128);
	while (true)
	{
		SetPixel(' ', { 0,1 }, { 3,10 });
		SetPixel(' ', { 20,1 }, { 23,10 });
		cursor.Border(1, 9, 0, 119);
		Cursor(cursor, 240, { 1,0 }, 2);
		for (int i = 0; i < 9; i++)
		{
			SetPixel(to_string(int(rules.AliveAlive[i])), { 9,1 + i });
			SetPixel(to_string(int(rules.EmptyAlive[i])), { 29,1 + i });
		}
		switch (_getch())
		{
		case 'w':
			cursor.x--;
			break;
		case 's':
			cursor.x++;
			break;
		case 'a':
			cursor.y = 0;
			break;
		case 'd':
			cursor.y = 20;
			break;
		case 'e':
			if (!cursor.y)
			{
				rules.AliveAlive[cursor.x - 1] = !rules.AliveAlive[cursor.x - 1];
			}
			else
			{
				rules.EmptyAlive[cursor.x - 1] = !rules.EmptyAlive[cursor.x - 1];
			}
			break;
		case 'f':
			menu = 1;
			return 0;
		}
	}
}
int main_menu()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(outHd, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(outHd, &cursorInfo);
	SetConsoleCursorPosition(outHd, { 0,0 });
	SetConsoleTextAttribute(outHd, 15);
	system("cls");
	printf(
		"\t\t\t\tThe Life\n"
		"\tSet Grid\n"
		"\tSet Rules\n"
		"\tPlay\n"
		"\tExit\n");
	SetPixel("w - up   s - down   e - enter", { 0,29 }, 128);

	coord cursor(0, 0);
	while (true)
	{
		cursor.Border(1, 4, 0, 0);
		SetPixel(' ', { 0,1 }, { 4,5 });
		Cursor(cursor, 240, { 1,0 }, 3);
		switch (_getch())
		{
		case 'w':
			cursor.x--;
			break;
		case 's':
			cursor.x++;
			break;
		case 'e':
			switch (cursor.x)
			{
			case 1:
				menu = 2;
				return 0;
			case 2:
				menu = 3;
				return 0;
			case 3:
				menu = 4;
				return 0;
			case 4:
				menu = 0;
				return 0;
			}
		}
	}
}
int Play()
{
	SetPixel('#', { size2,0 }, { size2 + 1,30 });
	SetPixel('#', { size2 + 1,14 }, { 120,15 });
	SetPixel("Time:", { size2 + 2,1 });
	SetPixel("Cells:", { size2 + 2,4 });
	SetPixel("Step:", { size2 + 2,7 });
	SetPixel("Pause", { size2 + 5,16 });
	SetPixel("Spawn Random", { size2 + 5,17 });
	SetPixel("Spawn Glider", { size2 + 5,18 });
	SetPixel("Exit", { size2 + 5,20 });
	SetPixel("w - up   s - down   e - enter", { 0,29 }, 128);
	CleanColorMat();

	int start = time(0);
	coord cursor(0, 0);
	int i = 0;
	bool pause = false;
	while (true)
	{
		if (!pause)
		{
			life();
			SetPixel(to_string(i++), { size2 + 2,8 });
			SetPixel(to_string(time(0) - start), { size2 + 2,2 });
		}
		SetPixel(to_string(Count()) + "   ", { size2 + 2,5 });
		PrintGrid();
		if (_kbhit())
		{
			switch (_getch())
			{
			case 'w':
				cursor.x--;
				break;
			case 's':
				cursor.x++;
				break;
			case 'e':
				switch (cursor.x)
				{
				case 16:
					pause = !pause;
					break;
				case 17:
					SetRandomCell();
					SetRandomCell();
					SetRandomCell();
					break;
				case 18:
					SpawnGlyder();
					break;
				case 20:
					menu = 1;
					return 0;
				}
			}
			cursor.Border(16, 20, size2 + 2, size2 + 2);
			SetPixel(' ', { size2 + 2,16 }, { size2 + 5,21 });
			Cursor(cursor, 240, { 1,0 }, 2);
		}
	}
}
int LoadMatMenu()
{
	system("cls");
	SetPixel("Saved in file", { 2,0 });
	SetPixel("w - up   s - down   e - choose   q - delete   f - exit", { 66,29 }, 128);
	int i = 1;
	for_each(matList.begin(), matList.end(), [&i](Grid item) {
		SetPixel(item.GetName(), { 4,i++ }); });

	int size = matList.size();
	coord cursor(0, 0);
	auto it = matList.begin();
	while (true)
	{
		cursor.Border(1, size == 0 ? 1 : size, 0, 0);
		SetPixel(' ', { 0,1 }, { 4,size + 3 });
		Cursor(cursor, 240, { 1,0 }, 3);
		switch (_getch())
		{
		case 'w':
			cursor.x--;
			break;
		case 's':
			cursor.x++;
			break;
		case 'e':
			if (matList.size() != 0)
			{
				advance(it, cursor.x - 1);
				(*it).Use();
				menu = 2;
			}
			return 0;
		case 'q':
			if (matList.size() != 0)
			{
				advance(it, cursor.x - 1);
				matList.erase(it);
				SaveList();
			}
			return 0;
		case 'f':
			menu = 2;
			return 0;
		}
	}
}

int main()
{
	LoadList();
	while (true)
	{
		switch (menu)
		{
		case 0:
			return 0;
		case 1:
			main_menu();
			break;
		case 2:
			SetGrid();
			break;
		case 3:
			SetRules();
			break;
		case 4:
			Play();
			break;
		case 5:
			LoadMatMenu();
			break;
		}
	}
}