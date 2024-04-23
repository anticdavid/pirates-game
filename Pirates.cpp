#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include "ConsoleApplication13.h"
using namespace std;

string board[25][25];

//number of intact galleries
static int noOIG = 5;

//response with coordinates and indicator
struct response {
	int i;
	int j;
	bool ind;
};

//used for both player and enemies
struct entity {
	int i;
	int j;
	float health;
	int actions;
	int pieces_of_art;
	string name;
};
//signum function
int sgn(int n) {
	if (n > 0) return 1;
	if (n < 0) return -1;
	return 0;
}

//creating a 25x25 board
void initializeBoard() {
	for (int i = 0; i < 25; i++)
		for (int j = 0; j < 25; j++)
			board[i][j] = "[]";
	for (int k = 0; k < 5; k++) { //placing 5 Art Galleries at random places
		int i = rand() % 25;
		int j = rand() % 23 + 1;
		board[i][j] = "AG";
	} //placing Art Collector, Silly Pirates and Greedy Pirates at corners
	board[0][0] = "AC";
	board[0][24] = "SP";
	board[24][24] = "GP";
}

//allow player to move on board
entity moveOnBoard(entity player) {
	bool success = false;
	int ans = -1;
	while (!success) {
		cout << "________________________________________________" << endl;
		cout << "Which square do you want to go to? " << endl;
		cout << "______________________________" << endl;
		cout << "(1) Left square" << endl;
		cout << "(2) Right square" << endl;
		cout << "(3) Upper square" << endl;
		cout << "(4) Lower square" << endl;
		cout << "(5) Upper left square" << endl;
		cout << "(6) Lower left square" << endl;
		cout << "(7) Upper right square" << endl;
		cout << "(8) Lower right square" << endl;
		cout << "________________________________________________" << endl;
		cin >> ans;
		if (ans == 1 && player.j - 1 >= 0) {

			if (board[player.i][player.j - 1] == "[]") {
				player.j = player.j - 1;
				board[player.i][player.j] = "AC";
				board[player.i][player.j + 1] = "[]";
				success = true;
			} 
		}
		if (ans == 2 && player.j + 1 < 25) {
			if (board[player.i][player.j + 1] == "[]") {
				player.j = player.j + 1;
				board[player.i][player.j] = "AC";
				board[player.i][player.j - 1] = "[]";
				success = true;
			}
		}
		if (ans == 3 && player.i - 1 >= 0) {
			if (board[player.i - 1][player.j] == "[]") {
				player.i = player.i - 1;
				board[player.i][player.j] = "AC";
				board[player.i + 1][player.j] = "[]";
				success = true;
			}
		}
		if (ans == 4 && player.i + 1 < 25) {
			if (board[player.i + 1][player.j] == "[]") {
				player.i = player.i + 1;
				board[player.i][player.j] = "AC";
				board[player.i - 1][player.j] = "[]";
				success = true;
			}
		}
		if (ans == 5 && player.j - 1 >= 0 && player.i - 1 >= 0) {
			if (board[player.i -1 ][player.j - 1] == "[]") {
				player.j = player.j - 1;
				player.i = player.i - 1;
				board[player.i][player.j] = "AC";
				board[player.i + 1][player.j + 1] = "[]";
				success = true;
			}
		}
		if (ans == 6 && player.j - 1 >= 0 && player.i + 1 < 25) {
			if (board[player.i + 1][player.j - 1] == "[]") {
				player.j = player.j - 1;
				player.i = player.i + 1;
				board[player.i][player.j] = "AC";
				board[player.i - 1][player.j + 1] = "[]";
				success = true;
			}
		}
		if (ans == 7 && player.j + 1 < 25 && player.i - 1 >= 0) {
			if (board[player.i - 1][player.j + 1] == "[]") {
				player.j = player.j + 1;
				player.i = player.i - 1;
				board[player.i][player.j] = "AC";
				board[player.i + 1][player.j - 1] = "[]";
				success = true;
			}
		}
		if (ans == 8 && player.j - 1 < 25 && player.i + 1 < 25) {
			if (board[player.i + 1][player.j + 1] == "[]") {
				player.j = player.j + 1;
				player.i = player.i + 1;
				board[player.i][player.j] = "AC";
				board[player.i - 1][player.j - 1] = "[]";
				success = true;
			}
		}
		if (!success) {
			cout << "Can't go there... Try going somewhere else" << endl;
		}
	}
	return player;
}

//generate response for search for nearby gallery.
response playerNearby(entity enemy, entity player) {
	response r;
	if (enemy.name == "GP") {
		r.i = player.i;
		r.j = player.j;
		r.ind = true;
		return r;
	}
	if (abs(enemy.i - player.i) < 2 && abs(enemy.j - player.j) < 2) {
		r.i = player.i;
		r.j = player.j;
		r.ind = true;
	} else r.ind = false;
	return r;
}

response nearbyGalleryExists(int _i, int _j) {
	response r;
	for (int k = 0; k < 25; k++) {
		for (int l = 1; l < 24; l++) {
			if (board[k][l] == "AG" && abs(_i - k) < 5 && abs(_j - l) < 5) {
				r.i = k;
				r.j = l;
				r.ind = true;
				return r; //if there is a gallery 5 squares away, return response with
				//indicator set to true and coordinates of gallery
			}	
		}
	}
	r.ind = false;
	return r; //otherwise, just return response with indicator set to false
}

//create Silly Pirates
entity createSP() {
	entity sp;
	sp.actions = 1;
	sp.health = 1;
	sp.pieces_of_art = 0;
	sp.i = 0;
	sp.j = 24;
	sp.name = "SP";
	return sp;
}

//create Greedy Pirates
entity createGP() {
	entity gp;
	gp.actions = 2;
	gp.health = 2;
	gp.pieces_of_art = 0;
	gp.i = 24;
	gp.j = 24;
	gp.name = "GP";
	return gp;
}

//function that prints board on screen
void showBoard() {
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++)
			cout << board[i][j] <<" ";
		cout << endl;
	}
}

entity moveAtRandom(entity enemy) {
	int _i = rand() % 3 - 1, _j = rand() % 3 - 1;
	while (board[enemy.i + _i][enemy.j + _j] != "[]" || enemy.i + _i < 0 || enemy.i + _i > 24 || enemy.j + _j < 0 || enemy.j + _j>24) {
		_i = rand() % 3 - 1;
		_j = rand() % 3 - 1;
	}
	board[enemy.i][enemy.j] = "[]";
	enemy.i = enemy.i + _i;
	enemy.j = enemy.j + _j;
	board[enemy.i][enemy.j] = enemy.name;
	return enemy;
}

entity simulateEnemy(entity enemy, entity* player, vector<entity>* enemies) {
	response r = playerNearby(enemy, *player);
	response r2 = nearbyGalleryExists(enemy.i, enemy.j);
	if (enemy.name == "SP") {
		for (int i = 0; i < enemy.actions; i++) {
			if (r.ind) { //SP is one square away from player
				player->health = player->health - 0.5; // players health gets lowered by 1/2
				if (player->actions > 2) {
					player->actions = 2;
				}
				int steal = rand() % 5; //60% chance of stealing 
				cout << "Silly pirates tried stealing art piece from you" << endl;
				if (steal < 3) {
					player->pieces_of_art--;
					enemy.pieces_of_art++;
					cout << "They successfully stole a piece of art from you!" << endl;
				}
				else {
					cout << "Stealing was not successful." << endl;
				}
			}
			else if (r2.ind) {
				if (abs(r2.i - enemy.i) < 2 && abs(r2.j - enemy.j) < 2) { //SP is one square away from gallery
					enemy.pieces_of_art += 3;
					cout << "Gallery destroyed" << endl;
					noOIG--;
					board[r2.i][r2.j] = "[]";
				}
				else { //try going towards gallery
					int itemp, jtemp;
					itemp = sgn(r2.i - enemy.i);
					jtemp = sgn(r2.j - enemy.j);
					if (board[enemy.i + itemp][enemy.j + jtemp] == "[]") {
						enemy.i = enemy.i + itemp;
						enemy.j = enemy.j + jtemp;
						board[enemy.i][enemy.j] = "SP";
						board[enemy.i - itemp][enemy.j - jtemp] = "[]";
					}
					else if (board[enemy.i + itemp][enemy.j] == "[]") {
						enemy.i = enemy.i + itemp;
						board[enemy.i][enemy.j] = "SP";
						board[enemy.i - itemp][enemy.j] = "[]";
					}
					else if (board[enemy.i][enemy.j + jtemp] == "[]") {
						enemy.j = enemy.j + jtemp;
						board[enemy.i][enemy.j] = "SP";
						board[enemy.i][enemy.j - jtemp] = "[]";
					}
					else {
						enemy = moveAtRandom(enemy);
					}
				}
			}
			else {
				enemy = moveAtRandom(enemy);
			}
		}
	}
	else { // else, if the enemy is GP
		for (int i = 0; i < enemy.actions; i++) {
			//if there is gallery nearby and player is not nearby
			if (r2.ind && (abs(enemy.i - player->i) > 5 || abs(enemy.j - player->j) > 5)) {
				if (abs(r2.i - enemy.i) < 2 && abs(r2.j - enemy.j) < 2) { //SP is one square away from gallery
					enemy.pieces_of_art += 3;
					cout << "Gallery destroyed" << endl;
					noOIG--;
					board[r2.i][r2.j] = "[]";
					r2 = nearbyGalleryExists(enemy.i, enemy.j); //new report
				}
				else { //try going towards gallery
					int itemp, jtemp;
					itemp = sgn(r2.i - enemy.i);
					jtemp = sgn(r2.j - enemy.j);
					if (board[enemy.i + itemp][enemy.j + jtemp] == "[]") {
						enemy.i = enemy.i + itemp;
						enemy.j = enemy.j + jtemp;
						board[enemy.i][enemy.j] = "GP";
						board[enemy.i - itemp][enemy.j - jtemp] = "[]";
					}
					else if (board[enemy.i + itemp][enemy.j] == "[]") {
						enemy.i = enemy.i + itemp;
						board[enemy.i][enemy.j] = "GP";
						board[enemy.i - itemp][enemy.j] = "[]";
					}
					else if (board[enemy.i][enemy.j + jtemp] == "[]") {
						enemy.j = enemy.j + jtemp;
						board[enemy.i][enemy.j] = "GP";
						board[enemy.i][enemy.j - jtemp] = "[]";
					}
					else {
						enemy = moveAtRandom(enemy);
					}
				}
			}
			else {
				if (abs(enemy.i - player->i) < 2 && abs(enemy.j - player->j) < 2) {
					player->health = player->health - 0.5; // players health gets lowered by 1/2 (1 in one round)
					if (player->actions > 1) {
						player->actions = 1;
					}
					int steal = rand() % 100;
					cout << "Greedy pirates tried stealing art piece from you" << endl;
					if (steal < 90) { //90% chance of stealing 
						player->pieces_of_art--;
						enemy.pieces_of_art++;
						cout << "They successfully stole a piece of art from you!" << endl;
						int spawn = rand() % 4;
						//50% chance of spawning new enemy, 25% for both types
						if (spawn == 0) {
							enemies->push_back(createGP());
						}
						else if (spawn == 1) {
							enemies->push_back(createSP());
						}
					}
					else {
						cout << "Stealing was not successful." << endl;
					}
				}
				else if (abs(enemy.i - player->i) < 3 && abs(enemy.j - player->j) < 3) {
					int steal = rand() % 100;
					cout << "Greedy pirates tried stealing art piece from you" << endl;
					if (steal < 65) { //65% chance of stealing
						player->pieces_of_art--;
						enemy.pieces_of_art++;
						cout << "They successfully stole a piece of art from you!" << endl;
						int itemp, jtemp;
						itemp = sgn(player->i - enemy.i);
						jtemp = sgn(player->j - enemy.j);
						if (board[enemy.i + itemp][enemy.j + jtemp] == "[]") {
							enemy.i = enemy.i + itemp;
							enemy.j = enemy.j + jtemp;
							board[enemy.i][enemy.j] = "GP";
							board[enemy.i - itemp][enemy.j - jtemp] = "[]";
						}
						else if (board[enemy.i + itemp][enemy.j] == "[]") {
							enemy.i = enemy.i + itemp;
							board[enemy.i][enemy.j] = "GP";
							board[enemy.i - itemp][enemy.j] = "[]";
						}
						else if (board[enemy.i][enemy.j + jtemp] == "[]") {
							enemy.j = enemy.j + jtemp;
							board[enemy.i][enemy.j] = "GP";
							board[enemy.i][enemy.j - jtemp] = "[]";
						}
						int spawn = rand() % 4;
						//50% chance of spawning new enemy, 25% for both types
						if (spawn == 0) {
							enemies->push_back(createGP());
						}
						else if (spawn == 1) {
							enemies->push_back(createSP());
						}
					}
					else {
						cout << "Stealing was not successful." << endl;
					}
				}
				else {
					int itemp = sgn(player->i - enemy.i);
					int jtemp = sgn(player->j - enemy.j);
					if (board[enemy.i + itemp][enemy.j + jtemp] == "[]") {
						enemy.i = enemy.i + itemp;
						enemy.j = enemy.j + jtemp;
						board[enemy.i][enemy.j] = "GP";
						board[enemy.i - itemp][enemy.j - jtemp] = "[]";
					}
					else if (board[enemy.i + itemp][enemy.j] == "[]") {
						enemy.i = enemy.i + itemp;
						board[enemy.i][enemy.j] = "GP";
						board[enemy.i - itemp][enemy.j] = "[]";
					}
					else if (board[enemy.i][enemy.j + jtemp] == "[]") {
						enemy.j = enemy.j + jtemp;
						board[enemy.i][enemy.j] = "GP";
						board[enemy.i][enemy.j - jtemp] = "[]";
					}
					else {
						enemy = moveAtRandom(enemy);
					}
				}
			}
		}
	}
	return enemy;
}

entity emptyGallery(entity player) {
	if (player.i > 0 && player.j > 0)
		if (board[player.i - 1][player.j - 1] == "AG") {
			board[player.i - 1][player.j - 1] = "EG";
			player.pieces_of_art += 3;
			return player;
		}
	if (player.i > 0)
		if (board[player.i - 1][player.j] == "AG") {
			board[player.i - 1][player.j] = "EG";
			player.pieces_of_art += 3;
			return player;
		}
	if (player.j > 0)
		if (board[player.i][player.j - 1] == "AG") {
			board[player.i][player.j - 1] = "EG";
			player.pieces_of_art += 3;
			return player;
		}
	if (player.i > 0 && player.j < 24)
		if (board[player.i - 1][player.j + 1] == "AG") {
			board[player.i - 1][player.j + 1] = "EG";
			player.pieces_of_art += 3;
			return player;
		}
	if (player.j < 24)
		if (board[player.i][player.j + 1] == "AG") {
			board[player.i][player.j + 1] = "EG";
			player.pieces_of_art += 3;
			return player;
		}
	if (player.i < 24)
		if (board[player.i + 1][player.j] == "AG") {
			board[player.i + 1][player.j] = "EG";
			player.pieces_of_art += 3;
			return player;
		}
	if (player.j > 0 && player.i < 24)
		if (board[player.i + 1][player.j - 1] == "AG") {
			board[player.i + 1][player.j - 1] = "EG";
			player.pieces_of_art += 3;
			return player;
		}
	if (player.i < 24 && player.j < 24)
		if (board[player.i + 1][player.j + 1] == "AG") {
			board[player.i + 1][player.j + 1] = "EG";
			player.pieces_of_art += 3;
			return player;
		}
	cout << "There are no galleries nearby" << endl;
	return player;
}

double damage(double health, string enemy) {
	int r = rand() % 100;
	if (enemy == "SP") {
		if (r < 75) {
			cout << "Attack successful" << endl;
			return 0.5;
		}
		else {
			cout << "Attack failed" << endl;
			return 0;
		}
	}
	else if (health<=2.5){
		if (r < 80) {
			cout << "Attack successful" << endl;
			return 1;
		}
		else {
			cout << "Attack failed" << endl;
			return 0;
		}
	}
	else {
		if (r < 60) {
			cout << "Attack successful" << endl;
			return 1;
		}
		else {
			cout << "Attack failed" << endl;
			return 0;
		}
	}
}

entity theParrotOfDoom(entity player, vector<entity>* enemies) {
	if (player.i > 0 && player.j > 0)
		if (board[player.i - 1][player.j - 1] == "SP" || board[player.i - 1][player.j - 1] == "GP") {
			for (int i = 0; i < enemies->size(); i++) {
				if ((*enemies)[i].i == player.i - 1 && (*enemies)[i].j == player.j - 1) {
					(*enemies)[i].health -= damage(player.health, board[player.i - 1][player.j - 1]);
					if ((*enemies)[i].health <= 0) {
						player.pieces_of_art += (*enemies)[i].pieces_of_art;
						board[player.i - 1][player.j - 1] = "[]";
					}
				}
			}
			return player;
		}
	if (player.i > 0)
		if (board[player.i - 1][player.j] == "SP" || board[player.i - 1][player.j] == "GP") {
			for (int i = 0; i < enemies->size(); i++) {
				if ((*enemies)[i].i == player.i - 1 && (*enemies)[i].j == player.j) {
					(*enemies)[i].health -= damage(player.health, board[player.i - 1][player.j]);
					if ((*enemies)[i].health <= 0) {
						player.pieces_of_art += (*enemies)[i].pieces_of_art;
						board[player.i - 1][player.j] = "[]";
					}
				}
			}
			return player;
		}
	if (player.j > 0)
		if (board[player.i][player.j - 1] == "SP" || board[player.i][player.j - 1] == "GP") {
			for (int i = 0; i < enemies->size(); i++) {
				if ((*enemies)[i].i == player.i && (*enemies)[i].j == player.j - 1) {
					(*enemies)[i].health -= damage(player.health, board[player.i][player.j - 1]);
					if ((*enemies)[i].health <= 0) {
						player.pieces_of_art += (*enemies)[i].pieces_of_art;
						board[player.i][player.j - 1] = "[]";
					}
				}
			}
			return player;
		}
	if (player.i > 0 && player.j < 24)
		if (board[player.i - 1][player.j + 1] == "SP" || board[player.i - 1][player.j + 1] == "GP") {
			for (int i = 0; i < enemies->size(); i++) {
				if ((*enemies)[i].i == player.i - 1 && (*enemies)[i].j == player.j + 1) {
					(*enemies)[i].health -= damage(player.health, board[player.i - 1][player.j + 1]);
					if ((*enemies)[i].health <= 0) {
						player.pieces_of_art += (*enemies)[i].pieces_of_art;
						board[player.i - 1][player.j + 1] = "[]";
					}
				}
			}
			return player;
		}
	if (player.j < 24)
		if (board[player.i][player.j + 1] == "SP" || board[player.i][player.j + 1] == "GP") {
			for (int i = 0; i < enemies->size(); i++) {
				if ((*enemies)[i].i == player.i && (*enemies)[i].j == player.j + 1) {
					(*enemies)[i].health -= damage(player.health, board[player.i][player.j + 1]);
					if ((*enemies)[i].health <= 0) {
						player.pieces_of_art += (*enemies)[i].pieces_of_art;
						board[player.i][player.j + 1] = "[]";
					}
				}
			}
			return player;
		}
	if (player.i < 24)
		if (board[player.i + 1][player.j] == "SP" || board[player.i + 1][player.j] == "GP") {
			for (int i = 0; i < enemies->size(); i++) {
				if ((*enemies)[i].i == player.i + 1 && (*enemies)[i].j == player.j) {
					(*enemies)[i].health -= damage(player.health, board[player.i + 1][player.j]);
					if ((*enemies)[i].health <= 0) {
						player.pieces_of_art += (*enemies)[i].pieces_of_art;
						//enemies->erase(enemies->begin + i);
						board[player.i + 1][player.j] = "[]";
					}
				}
			}
			return player;
		}
	if (player.j > 0 && player.i < 24)
		if (board[player.i + 1][player.j - 1] == "SP" || board[player.i + 1][player.j - 1] == "GP") {
			for (int i = 0; i < enemies->size(); i++) {
				if ((*enemies)[i].i == player.i + 1 && (*enemies)[i].j == player.j - 1) {
					(*enemies)[i].health -= damage(player.health, board[player.i + 1][player.j - 1]);
					if ((*enemies)[i].health <= 0) {
						player.pieces_of_art += (*enemies)[i].pieces_of_art;
						board[player.i + 1][player.j - 1] = "[]";
					}
				}
			}
			return player;
		}
	if (player.i < 24 && player.j < 24)
		if (board[player.i + 1][player.j + 1] == "SP" || board[player.i + 1][player.j + 1] == "GP") {
			for (int i = 0; i < enemies->size(); i++) {
				if ((*enemies)[i].i == player.i + 1 && (*enemies)[i].j == player.j + 1) {
					(*enemies)[i].health -= damage(player.health, board[player.i + 1][player.j + 1]);
					if ((*enemies)[i].health <= 0) {
						player.pieces_of_art += (*enemies)[i].pieces_of_art;
						board[player.i + 1][player.j + 1] = "[]";
					}
				}
			}
			return player;
		}
	cout << "There are no enemies nearby" << endl;
	return player;
}

//game
void game() {
	//initializing all variables
	initializeBoard();
	//creating player
	entity player;
	player.health = 5;
	player.actions = 3;
	player.pieces_of_art = 3;
	player.i = 0;
	player.j = 0;
	player.name = "AC";
	//enemies
	vector<entity> enemies;
	enemies.push_back(createGP());
	enemies.push_back(createSP());
	//the pirate chains
	bool piratechains = false;
	int i_chain, j_chain;
	//
	bool end = false;
	bool exit = false;
	int ans;
	while (!end) {
		showBoard();//show the board at the beginning of every turn
		for (int i = 0; i < player.actions; i++) {
			cout << "________________________________________________" << endl;
			cout << "What do you want to do? " << endl; //player menu
			cout << "______________________________" << endl;
			cout << "(1) Walk" << endl;
			cout << "(2) Attack using 'The Parrot of Doom'" << endl;
			cout << "(3) Attack using 'The Pirate Chains'" << endl;
			cout << "(4) Take items from art gallery" << endl;
			cout << "(5) Exit game" << endl;
			cout << "________________________________________________" << endl;
			cin >> ans;
			while (ans != 1 && ans != 2 && ans != 3 && ans != 4 && ans != 5) { //while input is not ok
				cout << "________________________________________________" << endl;
				cout << "What do you want to do? " << endl;
				cout << "______________________________" << endl;
				cout << "(1) Walk" << endl;
				cout << "(2) Attack using 'The Parrot of Doom'" << endl;
				cout << "(3) Attack using 'The Pirate Chains'" << endl;
				cout << "(4) Take items from art gallery" << endl;
				cout << "(5) Exit game" << endl;
				cout << "________________________________________________" << endl;
				cin >> ans;
			}
			switch (ans) { //do things from player menu
			case 1:
				player = moveOnBoard(player);
				break;
			case 2:
				player = theParrotOfDoom(player, &enemies);
				break;
			case 3:
				piratechains = true;
				i_chain = player.i;
				j_chain = player.j;
				break;
			case 4:
				player = emptyGallery(player);
				break;
			case 5:
				exit = true;
				break;
			}
			if (exit) break;
		}
		//normalizing player's actions
		player.actions = 3;
		for (int i = 0; i < enemies.size(); i++) { //removing defeated enemies from vector
			if (board[enemies[i].i][enemies[i].j] == "[]")
				enemies.erase(enemies.begin() + i);
		}
		for (int i = 0; i < enemies.size(); i++) { //simulating enemies
			if (!piratechains) { //if pirate chains are not active, proceed as usual
				enemies[i] = simulateEnemy(enemies[i], &player, &enemies);
			}
			else {
				int r = rand() % 100;
				if(abs(enemies[i].i - i_chain) < 2 && abs(enemies[i].j - j_chain) < 2)
				{
					if (r >= 60) { //60% chance of stunning enemy one square away
						enemies[i] = simulateEnemy(enemies[i], &player, &enemies);
					}
					else {
						cout << "Enemy successfully stunned" << endl;
					}
				}
				else if (abs(enemies[i].i - i_chain) < 3 && abs(enemies[i].j - j_chain) < 3) {
					if (r >= 30) { //30% chance of stunning enemy one square away
						enemies[i] = simulateEnemy(enemies[i], &player, &enemies);
					}
					else {
						cout << "Enemy successfully stunned" << endl;
					}
				}
				else {
					enemies[i] = simulateEnemy(enemies[i], &player, &enemies);
				}
			}
		}
		piratechains = false;
		end = exit || player.health <= 0 || noOIG == 0 || enemies.size() == 0 || player.pieces_of_art== 18;
		//game ends when player's health equals to or drops below 0, all galleries are destroyed,
		//there are no enemies or all art is collected
	}
	if (player.health <= 0 || noOIG == 0) {
		cout << "You lost..." << endl;
		cout << noOIG << endl;
	}
	else if (exit) {
		cout << "You exited the game." << endl;
	}
	else {
		cout << "You won!" << endl;
	}
	showBoard();
}

int main()
{
	srand(time(0));
	game();
    return 0;
}

