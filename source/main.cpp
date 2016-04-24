#include <iostream>
#include <fstream>
#include <vector>
#include <character.h>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
// #include <ncurses.h>

using namespace std;

vector<PCharacter> monsters;
PCharacter player;
PCharacter princess;
ofstream logi("log.txt");

// extern ofstream logi;

void status(PCharacter ch, int max_hp, const char a[]){
	printw("%s: %g%%", a, (double)ch->hitPoint() / max_hp * 100.0);
}

int main(){
	// logi.open;
	srand(time(0));
	// GameMap myMap("castle.map");
	initscr();
	int n = getmaxy(stdscr) - 3;
	int m = getmaxx(stdscr) - 1;
	int drag_count = max(m * n / 200, DRAGONS_COUNT);
	int witch_count = max(m * n / 200, 4);
	int zomb_count = m * n / 25;
	GameMap myMap = GameMap(n, m);
	start_color();
	init_pair(1, COLOR_RED, COLOR_WHITE);
	init_pair(2, COLOR_GREEN, COLOR_WHITE);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	noecho();
	player = new Knight(&myMap);
	princess = new Princess(&myMap);
	monsters.push_back(player);
	monsters.push_back(princess);
	for (int i = 0; i < drag_count; i++){
		monsters.push_back(new Dragon(&myMap));
	}
	for(int i = 0; i < zomb_count; i++){
		monsters.push_back(new Zombie(&myMap));
	}
	for(int i = 0; i < witch_count; i++){
		monsters.push_back(new Witch(&myMap));
	}
	bool game = false;
	while(!game){
		clear();
		myMap.reDraw();
		move(0, 0);
		status(player, HP_KNIGHT, "Knight");
		move(1, 0);
		status(princess, HP_PRINCESS, "Princess");
		if (player->coord() == princess->coord()){
			clear();
			move(1, 0);
			printw("YOU WIN!!!!");
			break;
		}
		for (vector<PCharacter>::iterator i = monsters.begin(); i != monsters.end(); i++){
			(*i)->move();
		}
		if (player->hitPoint() <= 0 || princess->hitPoint() <= 0){
			game = true;
			clear();
			move(1, 0);
			printw("YOU LOSE!!!!");
		}
	}	
	for (int i = 0; i < (int)monsters.size(); i++){
		delete monsters[i];
	}
	getch();
	// for (int i = 0; i < 10; i++){
	// 	for (int j = 0; j < 10; j++){
	// 		cout << '#';
	// 	}
	// 	cout << endl;
	// }
	// int x = 1; 
	// int y = 1;
	// move(y, x);
	// addch('K' | COLOR_PAIR(1));
	// move(11, 11);
	// printw("Hellow");
	// bool goo = true;
	// while (goo){
	// 	refresh();
	// 	int ch = getch();
	// 	move(y, x);
	// 	addch('#');
	// 	switch (ch){
	// 		case (int)'w':
	// 			y--;
	// 			break;
	// 		case (int)'a':
	// 			x--;
	// 			break;
	// 		case (int)'d':
	// 			x++;
	// 			break;
	// 		case (int)'s':
	// 			y++;
	// 			break;
	// 	}
	// 	move(y, x);
	// 	addch('K' | COLOR_PAIR(1));
	// 	move(0, 0);
	// }
	endwin();
	return 0;
}