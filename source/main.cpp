#include <iostream>
#include <vector>
#include <character.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

vector<PCharacter> monsters;
PCharacter player;
PCharacter princess;

int main(){
	GameMap myMap("castle.map");
	srand(time(0));
	player = new Knight(&myMap);
	princess = new Princess(&myMap);
	monsters.push_back(new Dragon(&myMap));
	for(int i = 0; i < ZOMBIE_COUNT; i++){
		monsters.push_back(new Zombie(&myMap));
	}
	bool finish_game = false;
	myMap.reDraw();
	while (!finish_game){
		player->move();
		for (int i = 0; i < monsters.size(); i++){
			monsters[i]->move();
		}
		finish_game = !(player->hitPoint() > 0) || !(princess->hitPoint() > 0) ||  (player->coord() == princess->coord());
		myMap.reDraw();
		cout << player->hitPoint() << "/" << HP_KNIGHT << endl;
	}	
	for (int i = 0; i < monsters.size(); i++){
		delete monsters[i];
	}
	delete princess;
	delete player;
	return 0;
}