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
	GameMap myMap("castle.map");
	srand(time(0));
	player = new Knight(&myMap);
	princess = new Princess(&myMap);
	monsters.push_back(player);
	monsters.push_back(princess);
	monsters.push_back(new Dragon(&myMap));
	for(int i = 0; i < ZOMBIE_COUNT; i++){
		monsters.push_back(new Zombie(&myMap));
	}
	bool finish_game = false;
	myMap.reDraw();
		status(player, HP_KNIGHT, "Knight");
		status(princess, HP_PRINCESS, "Princess");
	while (!finish_game){
		for (int i = 0; i < (int)monsters.size(); i++){
			monsters[i]->move();
		}
		finish_game = !(player->hitPoint() > 0) || !(princess->hitPoint() > 0) ||  (player->coord() == princess->coord());
		myMap.reDraw();
		status(player, HP_KNIGHT, "Knight");
		status(princess, HP_PRINCESS, "Princess");
	}	
	for (int i = 0; i < (int)monsters.size(); i++){
		delete monsters[i];
	}
	return 0;
}