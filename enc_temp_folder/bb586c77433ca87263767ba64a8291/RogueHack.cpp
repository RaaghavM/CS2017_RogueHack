// RogueHack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define SCREEN_ROWS 20
#define SCREEN_COLS 60
#define DOOR_POSX SCREEN_COLS - 1
#define DOOR_POSY 10
vector<string> screen;

#define MAP_EMPTY '.'
#define MAP_VWALL '|'
#define MAP_HWALL '='
#define MAP_PLAYER '@'
#define MAP_DOOR '+'
#define MAP_WALKWAY '#'

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
	for (int r = 0; r < SCREEN_ROWS; r++)
	{
		cout << screen[r] << endl;
	}
}

void setInfo(int atk, int hp, int gold, int exp, int lvl)
{
	stringstream buffer;

	buffer << "ATK: " << atk << ", HP: " << hp << ", GOLD: " << gold << ", EXP: " << exp << ", LVL: " << lvl;
	screen[SCREEN_ROWS - 1] = buffer.str();
}

void setMap()
{
	for (int r = 0; r < SCREEN_ROWS-1; r++)  //-1 because statInfo takes up the last row
	{
		screen[r][0] = MAP_VWALL;
		screen[r][SCREEN_COLS - 1] = MAP_VWALL;
	}

	for (int c = 0; c < SCREEN_COLS; c++)
	{
		screen[0][c] = MAP_HWALL;
		screen[SCREEN_ROWS - 2][c] = MAP_HWALL;
	}
	screen[DOOR_POSY][DOOR_POSX] = MAP_DOOR;
}

void setPlayer(int r, int c)
{
	screen[r][c] = MAP_PLAYER;
}

int main()
{
	string choice = "";
	int myV = 3;
	int myH = 4;

	while (choice != "q" && choice != "Q" && choice != "quit" && choice != "Quit")
	{
		initScreen();
		setInfo(15, 100, 0, 10, 1);
		setMap();
		setPlayer(myV, myH);

		printScreen();

		cout << ">> ";
		cin >> choice;

		if (choice == "a")
		{
			myH -= 1;
		}
		else if (choice == "d")
		{
			myH += 1;
		}
		else if (choice == "w")
		{
			myV -= 1;
		}
		else if (choice == "s")
		{
			myV += 1;
		}
		else if (choice == "wd" || choice == "dw")
		{
			myH += 1;
			myV -= 1;
		}
		else if (choice == "wa" || choice == "aw") 
		{
			myH -= 1;
			myV -= 1;
		}
		else if (choice == "sa" || choice == "as")
		{
			myH -= 1;
			myV += 1;
		}
		else if (choice == "sd" || choice == "ds")
		{
			myH += 1;
			myV += 1;
		}
		else if (choice == "w+") 
		{
			for (int r = 0; r < SCREEN_ROWS - myV; r++)
			{
				myV -= 1;
			}
		}
		else 
		{
			cout << "I don't understand that" << endl;
		}

		if (myH == DOOR_POSX && myV == DOOR_POSY)
		{
			screen[DOOR_POSY][DOOR_POSX + 1] = MAP_WALKWAY;
		}
	}
	return 0;
}

