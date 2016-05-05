#include <iostream>
#include <fstream>
#include <vector>
#include <character.h>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <configuration.h>
// #include <ncurses.h>

using namespace std;

vector<PCharacter> monsters;
PCharacter player;
PCharacter princess;
ofstream logi("log.txt");
Configuration* conf;

// extern ofstream logi;

void status(PCharacter ch, int max_hp, const char a[]){
	printw("%s: %g%%", a, (double)ch->hitPoint() / max_hp * 100.0);
}

int main(){
	try{
		conf = new Configuration();
	}
	catch(string err){
		cout << err << endl;
		// delete conf;
		return -1;
	}


	// cout << "hp " << HP_KNIGHT << endl;
	// cout << "hp " << HP_PRINCESS << endl;
	// cout << "hp " << HP_ZOMBIE << endl;
	// cout << "hp " << HP_DRAGON << endl;
	// cout << "hp " << HP_OBJECT << endl;
	// cout << "hp " << HP_WITCH << endl;
	// cout << "damage " << DMG_KNIGHT << endl;
	// cout << "damage " << DMG_PRINCESS << endl;
	// cout << "damage " << DMG_ZOMBIE << endl;
	// cout << "damage " << DMG_DRAGON << endl;
	// cout << "damage " << DMG_MEDKIT << endl;
	// cout << "damage " << DMG_FIREBALL << endl;

	// return 0;
	srand(time(0));
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
	monsters.push_back(new Spawner<Zombie, '+'>(&myMap));
	monsters.push_back(new Spawner<Dragon, '&'>(&myMap));
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
	long step_count = 1;
	while(!game){
		logi << "next" << endl;
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
			logi << "start move " << (*i) << endl;
			(*i)->move();
			logi << "done move" << endl;
		}
		logi << "done step" << endl;
		if (player->hitPoint() <= 0 || princess->hitPoint() <= 0){
			game = true;
			clear();
			move(1, 0);
			printw("YOU LOSE!!!!");
		}
		if (!(step_count % 5)){
			monsters.push_back(new Medkit(&myMap));	
		}
		spawn_all();
		step_count++;
	}	
	for (int i = 0; i < (int)monsters.size(); i++){
		delete monsters[i];
	}
	getch();
	endwin();
	return 0;
}