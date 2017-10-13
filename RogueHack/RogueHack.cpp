// RogueHack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

#define SCREEN_ROWS 10
#define SCREEN_COLS 30
#define DOOR1_C SCREEN_COLS - 1
#define DOOR1_R 5
#define DOOR2_C 0
#define DOOR2_R 5
#define KEY1_R 3
#define KEY1_C 8
#define DOOR3_R 0
#define DOOR3_C 25
#define STICK_R 6
#define STICK_C 15
#define DOOR4_R SCREEN_ROWS - 2
#define DOOR4_C 25
vector<string> screen;
int room = 1;
vector<string> inventory;

vector<string> classes = { "soldier", "wizard" };

bool monster1Alive = true;
int monster1HP = 33;
bool hasStick = false;

int atk;
int hp;
int gold;
int xp;
int lvl;



#define MAP_EMPTY '.'
#define MAP_VWALL '|'
#define MAP_HWALL '='
#define MAP_PLAYER '@'
#define MAP_DOOR '+'
#define MAP_OBJECT '^'
#define MAP_MONSTER 'm'

void initScreen()
{
	string defaultRow = "";
	screen.clear();

	for (int c = 0; c < SCREEN_COLS; c++)
		defaultRow += MAP_EMPTY;

	for (int r = 0; r < SCREEN_ROWS; r++)
		screen.push_back(defaultRow);
}

void printScreen()
{
	for (int a = 0; a < SCREEN_ROWS; a++)
	{
		cout << screen[a] << endl;
	}
}

void setInfo(int atk, int hp, int gold, int exp, int lvl)
{
	stringstream buffer;

	buffer << "ATK: " << atk << ", HP: " << hp << ", GOLD: " << gold << ", EXP: " << exp << ", LVL: " << lvl;
	screen[SCREEN_ROWS - 1] = buffer.str();
}

void setPlayer(int r, int c)
{
	screen[r][c] = MAP_PLAYER;
}

void setMonster(int r, int c)
{
	screen[r][c] = MAP_MONSTER;
}

int checkLineEmpty(string rOrC, int number)
{
	if (rOrC == "row")
	{
		for (int q = 1; q < SCREEN_COLS - 1; q++)
		{
			if (screen[number][q] != MAP_EMPTY && screen[number][q] != MAP_PLAYER)
			{
				return q;
			}
		}

		return 0;
	}
	else if (rOrC == "column")
	{
		for (int w = 1; w < SCREEN_ROWS - 2; w++)
		{
			if (screen[w][number] != MAP_EMPTY && screen[number][w] != MAP_PLAYER)
			{
				return w;
			}
		}

		return 0;
	}
}

vector<string> checkInventoryWeapon()
{
	bool woodenStick = false;
	vector<string> inventoryWeapons;
	//more weapons here
	if (find(inventory.begin(), inventory.end(), "stick") != inventory.end()) //checks if inventory contains wooden stick
	{
		inventoryWeapons.push_back("stick");
	}
	//insert more weapons here using 'if' (not 'else if')
	return inventoryWeapons;
}

void monsterAttack()
{
	cout << "The monster attacks you." << endl;
	srand((int)time(0));
	int hpLost = (rand() % 20) + 10;
	cout << "You lost " << hpLost << " HP" << endl;
	hp -= hpLost;

	if (!(checkInventoryWeapon().empty())) //checks if you have weapon in inventory
	{
		string weapon_choice = "null";
		while (!(find(inventory.begin(), inventory.end(), weapon_choice) != inventory.end())) //weapon_choice is not in inventory
		{
			cout << "You attack the monster." << endl;
			cout << "Which weapon would you like to use to attack the monster?" << endl;
			for (int i = 0; i < checkInventoryWeapon().size(); i++)
			{
				cout << i + 1 << ". " << checkInventoryWeapon()[i] << ' ' << endl;
			}
			cin >> weapon_choice;
			if (!(find(inventory.begin(), inventory.end(), weapon_choice) != inventory.end()))
			{
				cout << "Please enter a valid option." << endl;
			}
		}

		for (int i = 0; i < checkInventoryWeapon().size(); i++)
		{
			if (weapon_choice == checkInventoryWeapon()[i])
			{
				cout << "You used the " << checkInventoryWeapon()[i] << " to attack the monster" << endl;
			}
		}

		srand((int)time(0));
		int monsterhpLost = (rand() % 20) + atk; //need to make different weapons have different attack power
		if (monsterhpLost >= monster1HP)
		{
			cout << "You dealt " << monsterhpLost << " damage and killed the monster" << endl;
			monster1Alive = false;
		}
		else
		{
			cout << "You dealt " << monsterhpLost << " damage but the monster is still alive" << endl
				<< "You will have to attack again" << endl;
			monster1HP -= monsterhpLost;
		}

	}
	else
	{
		cout << "You do not have a weapon to attack the monster." << endl;
	}
}


int moveMonsterRow(int currentMonsterRow, int currentMonsterCol)
{
	srand((int)time(0));
	int r = (rand() % 2) + 1;
	if (r == 1)
	{
		if (screen[currentMonsterRow - 1][currentMonsterCol] == MAP_EMPTY)
		{			
			currentMonsterRow -= 1;
			return currentMonsterRow;
		}
		else if(screen[currentMonsterRow + 1][currentMonsterCol] == MAP_EMPTY)
		{
			currentMonsterRow += 1;
			return currentMonsterRow;
		}
		else
		{
			monsterAttack();
		}
	}
	else if (r == 2)
	{
		if (screen[currentMonsterRow + 1][currentMonsterCol] == MAP_EMPTY)
		{
			currentMonsterRow += 1;
			return currentMonsterRow;
		}
		else if (screen[currentMonsterRow - 1][currentMonsterCol] == MAP_EMPTY)
		{
			currentMonsterRow -= 1;
			return currentMonsterRow;
		}
		else
		{
			monsterAttack();
		}
	}

}
		
int moveMonsterColumn(int currentMonsterRow, int currentMonsterColumn)
{
	srand((int)time(0));
	int r = (rand() % 2) + 1;
	if (r == 1)
	{
		if (screen[currentMonsterRow][currentMonsterColumn - 1] == MAP_EMPTY)
		{
			currentMonsterColumn -= 1;
			return currentMonsterColumn;
		}
		else if (screen[currentMonsterRow][currentMonsterColumn + 1] == MAP_EMPTY)
		{
			currentMonsterColumn += 1;
			return currentMonsterColumn;
		}
		else
		{
			monsterAttack();
		}
	}
	else if (r == 2)
	{
if (screen[currentMonsterRow][currentMonsterColumn + 1] == MAP_EMPTY)
{
	currentMonsterColumn += 1;
	return currentMonsterColumn;
}
else if (screen[currentMonsterRow][currentMonsterColumn - 1] == MAP_EMPTY)
{
	currentMonsterColumn -= 1;
	return currentMonsterColumn;
}
else
{
	monsterAttack();
}
	}
}

void setMapRoom1()
{
	for (int l = 0; l < SCREEN_ROWS - 1; l++)  //-1 because statInfo takes up the last row
	{
		screen[l][0] = MAP_VWALL;
		screen[l][SCREEN_COLS - 1] = MAP_VWALL;
	}

	for (int m = 0; m < SCREEN_COLS; m++)
	{
		screen[0][m] = MAP_HWALL;
		screen[SCREEN_ROWS - 2][m] = MAP_HWALL;
	}
	screen[DOOR1_R][DOOR1_C] = MAP_DOOR;
	if (!(find(inventory.begin(), inventory.end(), "key") != inventory.end())) //inventory does not contain 'key'
	{
		screen[KEY1_R][KEY1_C] = MAP_OBJECT; //key
	}
}

void setMapRoom2()
{

	for (int l = 0; l < SCREEN_ROWS - 1; l++)  //-1 because statInfo takes up the last row
	{
		screen[l][0] = MAP_VWALL;
		screen[l][SCREEN_COLS - 1] = MAP_VWALL;
	}

	for (int m = 0; m < SCREEN_COLS; m++)
	{
		screen[0][m] = MAP_HWALL;
		screen[SCREEN_ROWS - 2][m] = MAP_HWALL;
	}
	screen[DOOR2_R][DOOR2_C] = MAP_DOOR;

	screen[DOOR3_R][DOOR3_C] = MAP_DOOR;

}

void setMapRoom3()
{

	for (int l = 0; l < SCREEN_ROWS - 1; l++)  //-1 because statInfo takes up the last row
	{
		screen[l][0] = MAP_VWALL;
		screen[l][SCREEN_COLS - 1] = MAP_VWALL;
	}

	for (int m = 0; m < SCREEN_COLS; m++)
	{
		screen[0][m] = MAP_HWALL;
		screen[SCREEN_ROWS - 2][m] = MAP_HWALL;
	}


	if (!hasStick)
	{
		screen[STICK_R][STICK_C] = MAP_OBJECT;
	}
	else
	{
		screen[STICK_R][STICK_C] = MAP_EMPTY;
	}
	screen[DOOR4_R][DOOR4_C] = MAP_DOOR;
}



int main()
{
	string choice = "";
	int myR = 4;
	int myC = 3;
	int monsterR = 5;
	int monsterC = 4;
	cout << "Welcome to RogueHack! (Definitely not a spin-off of Rogue and NetHack)" << endl
		<< "If you want to learn how to play, type 'help'" << endl;
	cout << "Pick your class from the following list: " << endl;

	string classChoice;
	for (int i = 0; i < classes.size(); i++)
	{
		cout << i + 1 << ". " << classes[i] << endl;
	}
	cin >> classChoice;
	for (int i = 0; i < classes.size(); i++)
	{
		if (classChoice == classes[i])
		{
			cout << "You have picked " << classes[i] << "." << endl;
			atk = 15; //need to have different stats based on different classes
			hp = 100;
			gold = 0;
			xp = 10;
			lvl = 1;
		}
		else
		{
			cout << "Please enter a valid choice." << endl;
		}
	}
	

	while (choice != "q" && choice != "Q" && choice != "quit" && choice != "Quit")
	{
		initScreen();
		setInfo(atk, hp, gold, xp, lvl);
		if (room == 1)
		{
			setMapRoom1();

			if (myR == KEY1_R && myC == KEY1_C)
			{
				inventory.push_back("key");
				cout << "You picked up the object, it was a key." << endl;
				screen[KEY1_R][KEY1_C] = MAP_EMPTY;
			}

			if (myR == DOOR1_R && myC == DOOR1_C)
			{
				if (find(inventory.begin(), inventory.end(), "key") != inventory.end()) //checks if inventory contains 'key'
				{
					room = 2;
					myC = 0;
					cout << "You use your key to open the door and proceed into the next room." << endl;
				}
				else
				{
					cout << "The door is locked." << endl;
					myC -= 1;
				}
			}
		}
		else if (room == 2)
		{
			setMapRoom2();
			if (monster1Alive) 
			{
				setMonster(monsterR, monsterC);
				srand((int)time(0));
				int random = (rand() % 3) + 1;
				if (random == 1)
				{
					monsterR = moveMonsterRow(monsterR, monsterC);
				}
				else if (random == 2)
				{
					monsterC = moveMonsterColumn(monsterR, monsterC);
				}
				else if (random == 3)
				{
					monsterR = moveMonsterRow(monsterR, monsterC);
					monsterC = moveMonsterColumn(monsterR, monsterC);
				}
			}
			


			if (myR == DOOR2_R && myC == DOOR2_C)
			{
				room = 1;
				myC = SCREEN_COLS - 1;
			}

			if (myR == DOOR3_R && myC == DOOR3_C)
			{
				room = 3;
				myR = SCREEN_ROWS - 2;
			}
		}
		else if (room == 3)
		{
			if (find(inventory.begin(), inventory.end(), "stick") != inventory.end())
			{
				hasStick = true;
			}
			else
			{
				hasStick = false;
			}

			setMapRoom3();

			if (myR == STICK_R && myC == STICK_C)
			{
				cout << "You obtain a wooden stick" << endl;
				inventory.push_back("stick");
			}

			if (myR == DOOR4_R && myC == DOOR4_C)
			{
				room = 2;
				myR = 0;
			}
		}
		setPlayer(myR, myC);

		printScreen();

		cout << ">> ";
		cin >> choice;

		system("cls");

		if (choice == "a")  // Go left by one
		{
			if (screen[myR][myC - 1] != MAP_VWALL && screen[myR][myC - 1] != MAP_HWALL)
			{
				myC -= 1;
			}
			else
			{
				cout << "You cannot do that." << endl;
			}
		}
		else if (choice == "d")  // Go right by one
		{
			if (screen[myR][myC + 1] != MAP_VWALL && screen[myR][myC + 1] != MAP_HWALL)
			{
				myC += 1;
			}
			else
			{
				cout << "You cannot do that." << endl;
			}
		}
		else if (choice == "w")  // Go up by one
		{
			if (screen[myR - 1][myC] != MAP_VWALL && screen[myR - 1][myC] != MAP_HWALL)
			{
				myR -= 1;
			}
			else
			{
				cout << "You cannot do that." << endl;
			}
		}
		else if (choice == "s")  // Go down by one
		{
			if (screen[myR + 1][myC] != MAP_VWALL && screen[myR + 1][myC] != MAP_HWALL)
			{
				myR += 1;
			}
			else
			{
				cout << "You cannot do that." << endl;
			}
		}
		else if (choice == "wd" || choice == "dw")
		{
			if (screen[myR - 1][myC + 1] != MAP_VWALL && screen[myR - 1][myC + 1] != MAP_HWALL)
			{
				myR -= 1;
				myC += 1;
			}
			else
			{
				cout << "You cannot do that." << endl;
			}
		}
		else if (choice == "wa" || choice == "aw")
		{
			if (screen[myR - 1][myC - 1] != MAP_VWALL && screen[myR - 1][myC - 1] != MAP_HWALL)
			{
				myR -= 1;
				myC -= 1;
			}
			else
			{
				cout << "You cannot do that." << endl;
			}
		}
		else if (choice == "sa" || choice == "as")
		{
			if (screen[myR + 1][myC - 1] != MAP_VWALL && screen[myR + 1][myC - 1] != MAP_HWALL)
			{
				myR += 1;
				myC -= 1;
			}
			else
			{
				cout << "You cannot do that." << endl;
			}
		}
		else if (choice == "sd" || choice == "ds")
		{
			if (screen[myR + 1][myC + 1] != MAP_VWALL && screen[myR + 1][myC + 1] != MAP_HWALL)
			{
				myR += 1;
				myC += 1;
			}
			else
			{
				cout << "You cannot do that." << endl;
			}
		}
		else if (choice == "ww")
		{
			/*if (checkLineEmpty("column", myC) == 0)
			{
				if (screen[1][myC] != MAP_VWALL && screen[1][myC] != MAP_HWALL)
				{
					myR = 1;
				}
				else
				{
					cout << "You cannot do that." << endl;
				}
			}
			else
			{
				myR = checkLineEmpty("column", myC) + 1;
			}*/
			while (screen[myR - 1][myC] == MAP_EMPTY)
			{
				myR -= 1;
			}
		}
		else if (choice == "dd")
		{
			/*if (checkLineEmpty("row", myR) == 0)
			{
				if (screen[myR][SCREEN_COLS - 2] != MAP_VWALL && screen[myR][SCREEN_COLS - 2] != MAP_HWALL)
				{
					myC = SCREEN_COLS - 2;
				}
				else
				{
					cout << "You cannot do that." << endl;
				}
			}
			else
			{
				myC = checkLineEmpty("row", myR) - 1;
			}*/
			while (screen[myR][myC + 1] == MAP_EMPTY)
			{
				myC += 1;
			}
		}
		else if (choice == "aa")
		{
			/*if (checkLineEmpty("row", myR) == 0)
			{
				if (screen[myR][1] != MAP_VWALL && screen[myR][1] != MAP_HWALL)
				{
					myC = 1;
				}
				else
				{
					cout << "You cannot do that." << endl;
				}
			}
			else
			{
				myC = checkLineEmpty("row", myR) + 1;
			}*/
			while (screen[myR][myC - 1] == MAP_EMPTY)
			{
				myC -= 1;
			}
		}
		else if (choice == "ss")
		{
			/*if (checkLineEmpty("column", myC) == 0)
			{
				if (screen[SCREEN_ROWS - 3][myC] != MAP_VWALL && screen[SCREEN_ROWS - 3][myC] != MAP_HWALL)
				{
					myR = SCREEN_ROWS - 3;
				}
				else
				{
					cout << "You cannot do that." << endl;
				}
			}
			else
			{

				myR = checkLineEmpty("column", myC) - 1;*/
				//} */
			while (screen[myR + 1][myC] == MAP_EMPTY)
			{
				myR += 1;
			}
		}
		else if (choice == "H" || choice == "h" || choice == "Help" || choice == "help")
		{
			cout << "RogueHack is a text adventure game with ASCII graphics.\n"
				<< "List of commands: (All commands must be followed with enter)\n"
				<< "W - move up one space\n"
				<< "S - move down one space\n"
				<< "A - move left one space\n"
				<< "D - move right one space\n"
				<< "WW - move up until you hit an object\n"
				<< "SS - move down until you hit an object\n"
				<< "AA - move left until you hit an object\n"
				<< "DD - move right until you hit and object\n"
				<< "WD (or DW) - move diagonally one space up and to the right\n"
				<< "WA (or AW) - move diagonally one space up and to the left\n"
				<< "SD (or DS) - move diagonally one space down and to the right\n"
				<< "SA (or AS) - move diagonally one space down and to the left\n"
				<< "I - view inventory\n"
				<< "Q - quit game\n"
				<< "\n"
				<< "List of objects:\n"
				<< "@ - your player\n"
				<< ". - empty space on map\n"
				<< "| - vertical wall\n"
				<< "= - horizontal wall\n"
				<< "+ - a door\n"
				<< "^ - a useful object\n"
				<< "m - a monster\n"
				;
		}
		else if (choice == "i" || choice == "I" || choice == "inventory" || choice == "Inventory")
		{
			if (inventory.size() == 0)
			{
				cout << "Your inventory is empty" << endl;
			}
			else
			{
				for (int d = 0; d < inventory.size(); d++)
				{
					cout << inventory[d] << endl;
				}
			}
		}
		else if (choice != "q" || choice != "Q" || choice != "quit" || choice != "Quit")
		{
			cout << "I don't understand that" << endl;
		}
		

		if (monsterR == myR && monsterC == myC && monster1Alive)
		{
			monsterAttack();
		}

		if (hp < 1)
		{
			cout << "You died!" << endl;
			cout << "Would you like to quit? [y/n]" << endl;
			cin >> choice;
			if (choice == "y" || choice == "Y" || choice == "yes" || choice == "Yes")
			{
				choice = "q"; //exits out of while loop
			}
			else 
			{
				cout << "Okay." << endl;
				cout << "There is no option to restart yet. If you are mad, please contact Raaghav Malik." << endl;
				cout << "Otherwise, wait on this screen forever.";
				cin >> choice;
			}
		}
	}

	return 0;
}