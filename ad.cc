#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "game.hh"

std::ifstream ifs;
std::string fbuf;
std::string cbuf;
std::string inames[32];
char fname[99];

game::character ply;
game::character enemy;
game::thing backpack[10];

bool nexit = false;
bool sexit = false;
bool eexit = false;
bool wexit = false;
bool keyexit = false;
int keyexit_id = 0;
// key exits are only applicable to the north
// this means you cannot have a key exit to the south, west or east.

bool item_taken[128][128];
bool firstlook = true;
bool itempresent = false;
int itempresent_id = 0;
bool enemypresent = false;
int enemypresent_id = 0;

int main(void) {

	ply.health = 10.0f;
	ply.max_hp = ply.health;
	ply.hit_dmg = 0.85f;
	ply.name = "Player Character";

	int ictl = 0;
	ifs.open("game/items");
	if (ifs.is_open()) {
		while(!ifs.eof()) {
			std::getline(ifs, inames[ictl]);
			ictl++;
			if (ictl > 32)
				break;	
		}
	}
	ifs.close();

	ifs.open("game/title");
	if (ifs.is_open()) {
		while(!ifs.eof()) {
			std::getline(ifs, fbuf);
			std::cout << fbuf << std::endl;
		}
	}
	ifs.close();
	std::getline(std::cin, cbuf);
	
	while(cbuf != "quit") {
		sprintf(fname, "game/%i,%i", ply.mapx, ply.mapy);
		
		int line_counter = 0;
		if (firstlook) {
			itempresent_id = 0;
			itempresent = false;
			enemypresent_id = 0;
			enemypresent = false;

			keyexit = false;
			nexit = false;
			sexit = false;
			eexit = false;
			wexit = false;
			firstlook = false;


			// the above will be reread from the file
			// and set accordingly.
			// this is to ensure consistency from area to area in the game.

			std::cout << std::endl << std::endl << "------------------- Area " << ply.mapx << ", " << ply.mapy << " --------------------------" << std::endl;

			ifs.open(fname);
			if (ifs.is_open()) {
				while(!ifs.eof()) {
					std::getline(ifs, fbuf);
					line_counter++;
					if (line_counter > 6) {
						std::cout << fbuf << std::endl;
					} else {
						char command_base;
						char command_param;
						char command_param_string[99];
						char command_param_2_string[99];

						sscanf(fbuf.c_str(), "%c %c", &command_base, &command_param);
						sscanf(fbuf.c_str(), "%c %s", &command_base, command_param_string);
						sscanf(fbuf.c_str(), "%c %s %s", &command_base, command_param_string, command_param_2_string);

						if (command_base == 'n') {
							if (command_param == 'y') {
								nexit = true;
							} else if (command_param == 'k') {
								keyexit = true;
								keyexit_id = atoi(command_param_2_string);
							}
						} 
						if (command_base == 's') {
							if (command_param == 'y') {
								sexit = true;
							}
						} 
						if (command_base == 'e') {
							if (command_param == 'y') {
								eexit = true;
							}
						}
						if (command_base == 'w') {
							if (command_param == 'y') {
								wexit = true;
							}
						}
						if (command_base == 'i') {
							if (!item_taken[ply.mapx][ply.mapy]) {
								if (atoi(command_param_string) != 0) {
									itempresent_id = atoi(command_param_string);
									itempresent = true;
									std::cout << "An item is present here." << std::endl;
								}
							}
						}
						if (command_base == 'x') {
							if (atoi(command_param_string) != 0) {
								enemypresent = true;
								enemypresent_id = atoi(command_param_string);
								if (enemypresent_id == 1) {
									enemy.health = 4.0f;
									enemy.hit_dmg = 0.5f;
									enemy.name = "Skeleton";
								}
								std::cout << "An enemy is present here." << std::endl;
							}
						}
					}
				}
			} else {
				std::cout << "File object is not open" << std::endl;
			}	
			ifs.close();
		}

		firstlook = false;
		std::cout << "do: ";
		std::getline(std::cin, cbuf);
		// --------	
			if (cbuf == "backpack") {
				for (int show_backpack = 0; show_backpack < 10; show_backpack++) {
					std::cout << show_backpack << " >>> "  << backpack[show_backpack].name << std::endl;
				}
			}

			if (cbuf == "go north") {
				if (nexit) {
					ply.mapy++;
					firstlook = true;
				} else if (keyexit) { 
					std::cout << "What slot is your " << inames[keyexit_id] << " in? ";
					std::string keyslot;
					std::getline(std::cin, keyslot);
					int keyslot_int = atoi(keyslot.c_str());
					std::cout << backpack[keyslot_int].thing_id << "    " << keyexit_id << std::endl;
					if (backpack[keyslot_int].thing_id == keyexit_id + 1) {
						// key is present at slot
						ply.mapy++;
						std::cout << "You use the key to open the door." << std::endl;
						backpack[keyslot_int].thing_id = 0;
						backpack[keyslot_int].name = "";
						firstlook = true;
					} else {
						std::cout << "That slot does not contain the required item." << std::endl;
					}
				} else {
					std::cout << "You cannot travel that way." << std::endl;
					
				}
			}
			else if (cbuf == "go south") {
				if (sexit) {
					ply.mapy--;
					firstlook = true;
				} else {
					std::cout << "You cannot travel that way." << std::endl;
				}
			}
			else if (cbuf == "go east") {
				if (eexit) {
					ply.mapx++;
					firstlook = true;
				} else {
					std::cout << "You cannot travel that way." << std::endl;
				}
			}
			else if (cbuf == "go west") {
				if (wexit) {
					ply.mapx--;
					firstlook = true;
				} else {
					std::cout << "You cannot travel that way." << std::endl;
				}
			}
			else if (cbuf == "look around") {
				firstlook = true;
			}
			else if (cbuf == "take item") {
				if (itempresent) {
					std::string desired_slot;
					int desired_slot_int;
					std::cout << "Choose what slot you want to take the item in: ";
					std::getline(std::cin, desired_slot);
					desired_slot_int = atoi(desired_slot.c_str());
					if (backpack[desired_slot_int].thing_id == 0) {
						backpack[desired_slot_int].thing_id = itempresent_id;
						backpack[desired_slot_int].name = inames[itempresent_id - 1];
						item_taken[ply.mapx][ply.mapy] = true;
					} else {
						std::cout << "That slot is not free!" << std::endl;
					}
				} else {
					std::cout << "There is no item to take." << std::endl;
				}
			}
			else if (cbuf == "use item") {
				std::string item_slot_s;
				int item_slot = 0;
				std::cout << "What slot is the item you want to use in? " << std::endl;
				std::getline(std::cin, item_slot_s);
				item_slot = atoi(item_slot_s.c_str());
				int item_id = backpack[item_slot].thing_id;

				if (item_id != 0) {
					if (item_id == 1 || item_id == 2) {
						std::cout << "You eat the " << backpack[item_slot].name << "." << std::endl;
						backpack[item_slot].thing_id = 0;
						backpack[item_slot].name = "";

						ply.health += item_id;

						// the above line means that:
						// apples heal 1.0 health points
						// bread loafs heal 2.0 health points.
					} else {
						std::cout << "I don't know what to do with that." << std::endl;
					}
				} else {
					std::cout << "There is no item in that slot" << std::endl;
				}
			}
			else if (cbuf == "attack enemy") {
				std::string ebuf;
				int round_counter = 0;
				std::cout << "Your hits: " << ply.hit_dmg << std::endl;
				std::cout << "Enemy hits: " << enemy.hit_dmg << std::endl;

				std::cout << "Press <ENTER> to begin" << std::endl;
				std::getline(std::cin, ebuf);

				while (enemy.health > 0.0f) {
					round_counter++;

					std::cout << std::endl << "------------ ROUND " << round_counter << " -----------------" << std::endl;

					ply.health -= enemy.hit_dmg;
					std::cout << "The enemy hits you for " << enemy.hit_dmg << std::endl;

					std::cout << "Your HP: " << ply.health << std::endl;
					std::cout << "Press <ENTER>" << std::endl;

					round_counter++;

					std::getline(std::cin, ebuf);
					std::cout << std::endl << "------------ ROUND " << round_counter << " -----------------" << std::endl;

					std::cout << "You hit the enemy for " << ply.hit_dmg << std::endl;
					enemy.health -= ply.hit_dmg;

					std::cout << "Enemy's HP: " << enemy.health << std::endl;
				}
				std::cout << "The target dies from your blows." << std::endl;
				std::cout << "For winning the fight, you are granted 1.0 health points." << std::endl;

				ply.health += 1.0f;
			}
			else if (cbuf == "status") {
				std::cout << "Health points: " << ply.health << std::endl;
				std::cout << "You currently hit for " << ply.hit_dmg << " health points." << std::endl;
			}
			else if (cbuf == "look around") {
				firstlook = true;
			}
			else if (cbuf == "exits") {
				std::cout << "Available exits are:" << std::endl;
				if (nexit)
					std::cout << "north ";
				if (keyexit)
					std::cout << "north (requires " << inames[keyexit_id] << ") ";
				if (sexit)
					std::cout << "south ";
				if (eexit)
					std::cout << "east ";
				if (wexit)
					std::cout << "west ";
				std::cout << "." << std::endl;
			}
		// --------
	}
	return 0;
}
