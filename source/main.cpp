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
	noecho();
	int n = getmaxy(stdscr) - 3;
	int m = getmaxx(stdscr) - 1;
	start_color();

	start_game(n, m, NULL);
	getch();
	endwin();
	return 0;
}
