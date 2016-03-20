#include <iostream>
#include <vector>
#include <character.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

vector<PCharacter> monsters;
PCharacter player;
PCharacter princess;

void status(PCharacter ch, int max_hp, const char a[]){
	cout << a << ": " << (double)ch->hitPoint() / max_hp * 100.0 << "\%" << endl;
}

int main(){
	srand(time(0));
	// GameMap myMap("castle.map");
	GameMap myMap = GameMap();
	player = new Knight(&myMap);
	princess = new Princess(&myMap);
	monsters.push_back(player);
	monsters.push_back(princess);
	for (int i = 0; i < DRAGONS_COUNT; i++){
		monsters.push_back(new Dragon(&myMap));
	}
	for(int i = 0; i < ZOMBIE_COUNT; i++){
		monsters.push_back(new Zombie(&myMap));
	}
	bool game = false;
	while(!game){
		myMap.reDraw();
		status(player, HP_KNIGHT, "Knight");
		status(princess, HP_PRINCESS, "Princess");
		if (player->coord() == princess->coord()){
			break;
		}
		for (vector<PCharacter>::iterator i = monsters.begin(); i != monsters.end(); i++){
			(*i)->move();
		}
		if (player->hitPoint() <= 0 || princess->hitPoint() <= 0){
			game = true;
		}
	}	
	for (int i = 0; i < (int)monsters.size(); i++){
		delete monsters[i];
	}
	return 0;
}