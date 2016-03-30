#pragma once
#include <genmap.h>
#include <ncurses.h>

#define MAP_SIZE 50
#define CHR_WALL '#'
#define CHR_NOTHING '.'
#define CHR_KNIGHT 'K'
#define CHR_PRINCESS 'P'
#define CHR_ZOMBIE 'Z'
#define CHR_DRAGON 'D'

#define UP_CODE 0
#define RIGHT_CODE 1
#define LEFT_CODE 2
#define DOWN_CODE 3

typedef char cell;


class GameMap{
public:
	GameMap();
	GameMap(int n, int m);
	GameMap(const char file[]);
	void setCharacter(cell c, int x, int y);
	void moveCharacter(int x1, int y1, int x2, int y2);
	void reDraw();
	cell getCell(const Point& p);
	Point dragonCoord();
	Point playerCoord();
	Point princessCoord();
	Point zombieCoord();
	~GameMap();
	int ny;
	int nx;
private:
	Point getCoord(const char& c);
	cell** m;
};

typedef GameMap* PGameMap;
