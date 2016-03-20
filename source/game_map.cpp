#include <iostream>
#include <fstream>
#include <stdio.h>
#include <game_map.h>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////
//GameMap
//Стандартный конструктор
GameMap::GameMap(){
	makeRandomRooms(m);
	// for (int i = 0; i < MAP_SIZE; i++){
	// 	for (int j = 0; j < MAP_SIZE; j++){
	// 		m[i][j] = CHR_NOTHING;
	// 	}
	// }
	// Point p = random_point();
	// setCharacter(CHR_DRAGON, p.x, p.y);
	// p = random_point();
	// setCharacter(CHR_KNIGHT, p.x, p.y);
	// p = random_point();
	// setCharacter(CHR_PRINCESS, p.x, p.y);
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
cell GameMap::getCell(const Point& p){
	return m[p.y][p.x];
}

//Получает координаты символа линейным поиском(стоит использовать только для уникальных символов)
Point GameMap::getCoord(const char& c){
	for (int i = 0; i < MAP_SIZE; i++){
		for(int j = 0; j < MAP_SIZE; j++){
			if (m[j][i] == c){
				return Point(i, j);
			}
		}
	}
	Point p = random_point();
	while (m[p.y][p.x] != '.'){
		p = random_point();
	}
	m[p.y][p.x] = c;
	return p;
}

//Получает координаты символа 'D' на момент написания коментария обозначающий дракона
Point GameMap::dragonCoord(){
	return getCoord(CHR_DRAGON);
}

//Получает координаты символа 'K' на момент написания коментария обозначающий рыцаря
Point GameMap::playerCoord(){
	return getCoord(CHR_KNIGHT);
}

//Получает координаты символа 'P' на момент написания коментария обозначающий принцессу
Point GameMap::princessCoord(){
	return getCoord(CHR_PRINCESS);
}
