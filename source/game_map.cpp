#include <iostream>
#include <fstream>
#include <stdio.h>
#include <game_map.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////
//point
//Конструктор по умолчанию
point::point(){
	x = 0;
	y = 0;
}

//Конструктор по 2-м координатам
point::point(int _x, int _y){
	x = _x;
	y = _y;
}

//Оператор +
point operator+(const point& f, const point& s){
	return point(f.x + s.x, f.y + s.y);
}

//Оператор ==
bool operator==(const point& f, const point& s){
	return (f.x == s.x) && (f.y == s.y);
}

//Генерация случайной точки
point random_point(){
	int a = rand() % (MAP_SIZE*MAP_SIZE);
	return point(a / MAP_SIZE, a % MAP_SIZE);
}

///////////////////////////////////////////////////////////////////////////////////////////
//GameMap
//Стандартный конструктор
GameMap::GameMap(){
	for (int i = 0; i < MAP_SIZE; i++){
		for (int j = 0; j < MAP_SIZE; j++){
			m[i][j] = CHR_NOTHING;
		}
	}
	point p = random_point();
	setCharacter(CHR_DRAGON, p.x, p.y);
	p = random_point();
	setCharacter(CHR_KNIGHT, p.x, p.y);
	p = random_point();
	setCharacter(CHR_PRINCESS, p.x, p.y);
}

//Конструктор из файла с картой
GameMap::GameMap(const char file[]){
	ifstream input(file);
	for (int i = 0; i < MAP_SIZE; i++){
		input >> m[i];
	}
	input.close();
}

//Устанавливает символ "с" в ячейку с координатами (x, y)
void GameMap::setCharacter(cell c, int x, int y){
	m[y][x] = c;
}

//Перемещает(точнее говоря меняет местами с) точку с координатами (x1, y1) на место (x2, y2)
void GameMap::moveCharacter(int x1, int y1, int x2, int y2){
	swap(m[y1][x1], m[y2][x2]);
}

//Выводит карту на экран(желательно предворительно его почистить)
void GameMap::reDraw(){
	system("clear");
	for (int i = 0; i < MAP_SIZE; i++){
		cout << m[i] << endl;
	}
}

//Получение символа из ячейки
cell GameMap::getCell(const point& p){
	return m[p.y][p.x];
}

//Получает координаты символа линейным поиском(стоит использовать только для уникальных символов)
point GameMap::getCoord(const char& c){
	for (int i = 0; i < MAP_SIZE; i++){
		for(int j = 0; j < MAP_SIZE; j++){
			if (m[j][i] == c){
				return point(i, j);
			}
		}
	}
	point p = random_point();
	while (m[p.y][p.x] != '.'){
		p = random_point();
	}
	m[p.y][p.x] = c;
	return p;
}

//Получает координаты символа 'D' на момент написания коментария обозначающий дракона
point GameMap::dragonCoord(){
	return getCoord(CHR_DRAGON);
}

//Получает координаты символа 'K' на момент написания коментария обозначающий рыцаря
point GameMap::playerCoord(){
	return getCoord(CHR_KNIGHT);
}

//Получает координаты символа 'P' на момент написания коментария обозначающий принцессу
point GameMap::princessCoord(){
	return getCoord(CHR_PRINCESS);
}
