// RogueHack.cpp : Defines the entry point for the console application.
//

//Things to improve:
//add more quests and rooms
//function(s) for moving

//For some reason going back to room 1 after killing the monsters causes script to crash??

//Things I added:
//another monster
//secret gateway

//Things I've completed:
//choose number instead of name
//displays room at bottom
//machete
//make room switching smoother
//changed "and" to "an" (typo in help menu)
//displays coordinates at bottom (actually added for debugging, but could stay as game mechanic)



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
#define DOOR3_R 0
#define DOOR3_C 25
#define DOOR4_R SCREEN_ROWS - 2
#define DOOR4_C 25
#define GATEWAY1_R 5
#define GATEWAY1_C SCREEN_COLS - 1
#define DOOR5_R 5
#define DOOR5_C 0
#define HIDDENDOOR_R 0
#define HIDDENDOOR_C 15

#define KEY1_R 3
#define KEY1_C 8
#define STICK_R 6
#define STICK_C 15
#define DAGGER_R 7
#define DAGGER_C 12
#define HEALPOTION_R 3
#define HEALPOTION_C 20
#define NOTE_R 2
#define NOTE_C 10
#define MACHETE_R 6
#define MACHETE_C 10


vector<string> screen;
int room = 1;
vector<string> inventory;
string weaponUsed = "";

vector<string> classes = { "soldier", "wizard" };

int monster1HP = 33;
bool monster1Alive = true;
bool monster2Alive = true;
int monster2HP = 55;
int monster1R = 5;
int monster1C = 4;
int monster2R = 7;
int monster2C = 8;

int atk;
int hp;
int gold;
int xp;
int lvl;
string useItem;

bool gatewayOpen = false;

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

void setInfo(int atk, int hp, int gold, int exp, int lvl, int room, int myR, int myC)
{
    stringstream buffer;
    
    buffer << "ATK: " << atk << ", HP: " << hp << ", GOLD: " << gold << ", EXP: " << exp << ", LVL: " << lvl << ", ROOM: "  << room << ", POS: (" << myR << ", " << myC << ")";
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
        
    }
    return 0;
}

vector<string> weaponsInventory()
{
    vector<string> weaponsInventory;
    if (find(inventory.begin(), inventory.end(), "stick") != inventory.end()) //checks if inventory contains wooden stick
    {
        weaponsInventory.push_back("stick");
    }
    if (find(inventory.begin(), inventory.end(), "dagger") != inventory.end()) //dagger
    {
        
        weaponsInventory.push_back("dagger");
    }
    if (find(inventory.begin(), inventory.end(), "machete") != inventory.end()) //maghete
    {
        weaponsInventory.push_back("machete");
    }
    //insert more weapons here using 'if' (not 'else if')
    return weaponsInventory;
}

int checkInventory(string item)
{
    for (int b = 0; b < inventory.size(); b++)
    {
        if (inventory[b] == item)
        {
            return b;
        }
    }
    return -1;
}

int weaponPower(string weapon)
{
    if (weapon == "stick")
    {
        return 5;
    }
    if (weapon == "dagger")
    {
        return 10;
    }
    if (weapon == "machete")
    {
        return 15;
    }
    //insert more weapons here
    return -1;
}

void monsterAttack(int mNum, int currentMonsterRow, int currentMonsterCol)
{
    cout << "The monster attacks you." << endl;
    srand((int)time(0));
    int hpLost = (rand() % 20) + 10;
    cout << "You lost " << hpLost << " HP" << endl;
    hp -= hpLost;
    
    if (!(weaponsInventory().empty())) //checks if you have weapon in inventory
    {
        string weapon_choice = "null";
        while (!(find(inventory.begin(), inventory.end(), weapon_choice) != inventory.end())) //weapon_choice is not in inventory
        {
            cout << "You attack the monster." << endl;
            cout << "Which weapon would you like to use to attack the monster?" << endl;
            for (int i = 0; i < weaponsInventory().size(); i++)
            {
                cout << i + 1 << ". " << weaponsInventory()[i] << ' ' << endl;
            }
            cin >> weapon_choice;
            if (weapon_choice == "1")
            {
                weapon_choice = weaponsInventory()[0];
            }
            if (weapon_choice == "2")
            {
                weapon_choice = weaponsInventory()[1];
            }
            // add more if (weapon_choice == "num") as you add potential weapons
            if (!(find(inventory.begin(), inventory.end(), weapon_choice) != inventory.end()))
            {
                cout << "Please enter a valid option." << endl;
            }
        }
        
        for (int i = 0; i < weaponsInventory().size(); i++)
        {
            if (weapon_choice == weaponsInventory()[i])
            {
                cout << "You used the " << weaponsInventory()[i] << " to attack the monster" << endl;
                weaponUsed = weaponsInventory()[i];
            }
        }
        if (mNum == 1)
        {
            srand((int)time(0));
            int monster1hpLost = (rand() % 20) + atk + weaponPower(weaponUsed);
            if (monster1hpLost >= monster1HP)
            {
                cout << "You dealt " << monster1hpLost << " damage and killed the monster" << endl;
                screen[currentMonsterRow][currentMonsterCol] = MAP_EMPTY;
                monster1Alive = false;
            }
            else
            {
                cout << "You dealt " << monster1hpLost << " damage but the monster is still alive" << endl
                << "You will have to attack again" << endl;
                monster1HP -= monster1hpLost;
            }
        }
        else if (mNum == 2)
        {
            srand((int)time(0));
            int monster2hpLost = (rand() % 20) + atk + weaponPower(weaponUsed);
            if (monster2hpLost >= monster1HP)
            {
                cout << "You dealt " << monster2hpLost << " damage and killed the monster" << endl;
                screen[currentMonsterRow][currentMonsterCol] = MAP_EMPTY;
                monster2Alive = false;
            }
            else
            {
                cout << "You dealt " << monster2hpLost << " damage but the monster is still alive" << endl
                << "You will have to attack again" << endl;
                monster1HP -= monster2hpLost;
            }
        }
        //else if monster2
        
    }
    else
    {
        cout << "You do not have a weapon to attack the monster." << endl;
    }
}


int moveMonsterRow(int mNum, int currentMonsterRow, int currentMonsterCol)
{
    if (mNum == 1)
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
            else if (screen[currentMonsterRow + 1][currentMonsterCol] == MAP_EMPTY)
            {
                currentMonsterRow += 1;
                return currentMonsterRow;
            }
            else
            {
                monsterAttack(1, currentMonsterRow, currentMonsterCol);
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
                monsterAttack(2, currentMonsterRow, currentMonsterCol);
            }
        }
    }
    else if (mNum == 2)
    {
        srand((int)time(0));
        int v = (rand() % 2) + 1;
        if (v == 1)
        {
            if (screen[currentMonsterRow - 1][currentMonsterCol] == MAP_EMPTY)
            {
                currentMonsterRow -= 1;
                return currentMonsterRow;
            }
            else if (screen[currentMonsterRow + 1][currentMonsterCol] == MAP_EMPTY)
            {
                currentMonsterRow += 1;
                return currentMonsterRow;
            }
            else
            {
                monsterAttack(1, currentMonsterRow, currentMonsterCol);
            }
        }
        else if (v == 2)
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
                monsterAttack(2, currentMonsterRow, currentMonsterCol);
            }
        }
    }
    return -1;
}

int moveMonsterColumn(int currentMonsterRow, int currentMonsterCol)
{
    srand((int)time(0));
    int r = (rand() % 2) + 1;
    if (r == 1)
    {
        if (screen[currentMonsterRow][currentMonsterCol - 1] == MAP_EMPTY)
        {
            currentMonsterCol -= 1;
            return currentMonsterCol;
        }
        else if (screen[currentMonsterRow][currentMonsterCol + 1] == MAP_EMPTY)
        {
            currentMonsterCol += 1;
            return currentMonsterCol;
        }
        else
        {
            monsterAttack(1, currentMonsterRow, currentMonsterCol);
        }
    }
    else if (r == 2)
    {
        if (screen[currentMonsterRow][currentMonsterCol + 1] == MAP_EMPTY)
        {
            currentMonsterCol += 1;
            return currentMonsterCol;
        }
        else if (screen[currentMonsterRow][currentMonsterCol - 1] == MAP_EMPTY)
        {
            currentMonsterCol -= 1;
            return currentMonsterCol;
        }
        else
        {
            monsterAttack(2, currentMonsterRow, currentMonsterCol);
        }
    }
    return -1;
}

//bool monster1Alive()
//{
//	for (int r = 0; r < SCREEN_ROWS - 2; r++)
//	{
//		for (int c = 0; c < SCREEN_COLS - c; c++)
//		{
//			if (screen[r][c] == MAP_MONSTER)
//			{
//				return true;
//			}
//
//		}
//	}
//	return false;
//}

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
}
void setAttributes(int room, bool gatewayOpen)
{
    if (room == 1)
    {
        screen[DOOR1_R][DOOR1_C] = MAP_DOOR;
        if (!(find(inventory.begin(), inventory.end(), "key") != inventory.end())) //inventory does not contain 'key'
        {
            screen[KEY1_R][KEY1_C] = MAP_OBJECT; //key
        }
    }
    else if (room == 2)
    {
        screen[DOOR2_R][DOOR2_C] = MAP_DOOR;
        
        screen[DOOR3_R][DOOR3_C] = MAP_DOOR;
        
        if (gatewayOpen)
        {
            screen[GATEWAY1_R][GATEWAY1_C] = MAP_DOOR;
        }
    }
    else if (room == 3)
    {
        if (!(find(inventory.begin(), inventory.end(), "stick") != inventory.end()))
        {
            screen[STICK_R][STICK_C] = MAP_OBJECT;
        }
        else
        {
            screen[STICK_R][STICK_C] = MAP_EMPTY;
        }
        
        if (!(find(inventory.begin(), inventory.end(), "note") != inventory.end()))
        {
            screen[NOTE_R][NOTE_C] = MAP_OBJECT;
        }
        else
        {
            screen[NOTE_R][NOTE_C] = MAP_EMPTY;
        }
        
        screen[DOOR4_R][DOOR4_C] = MAP_DOOR;
    }
    else if(room == 4)
    {
        if (!(find(inventory.begin(), inventory.end(), "dagger") != inventory.end()))
        {
            screen[DAGGER_R][DAGGER_C] = MAP_OBJECT;
        }
        else
        {
            screen[DAGGER_R][DAGGER_C] = MAP_EMPTY;
        }
        if (!(find(inventory.begin(), inventory.end(), "healing_potion") != inventory.end()))
        {
            screen[HEALPOTION_R][HEALPOTION_C] = MAP_OBJECT;
        }
        screen[DOOR5_R][DOOR5_C] = MAP_DOOR;
    }
    else if (room == 5)
    {
        if (!(find(inventory.begin(), inventory.end(), "machete") != inventory.end()))
        {
            screen[MACHETE_R][MACHETE_C] = MAP_OBJECT;
        }
        else
        {
            screen[MACHETE_R][MACHETE_C] = MAP_EMPTY;
        }
    }
}
void playerMovements(int &myR, int &myC, string choice, string useItem, string classChoice)
{
    //player movements
    if (choice == "a")  // Go left by one
    {
        if (screen[myR][myC - 1] != MAP_VWALL && screen[myR][myC - 1] != MAP_HWALL ||  (myR == HIDDENDOOR_R && myC - 1 == HIDDENDOOR_C))
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
        if (screen[myR][myC + 1] != MAP_VWALL && screen[myR][myC + 1] != MAP_HWALL || (myR == HIDDENDOOR_R && myC + 1 == HIDDENDOOR_C))
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
        if (screen[myR - 1][myC] != MAP_VWALL && screen[myR - 1][myC] != MAP_HWALL ||  (myR - 1 == HIDDENDOOR_R && myC == HIDDENDOOR_C))
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
        if (screen[myR + 1][myC] != MAP_VWALL && screen[myR + 1][myC] != MAP_HWALL ||  (myR + 1 == HIDDENDOOR_R && myC == HIDDENDOOR_C))
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
        if (screen[myR - 1][myC + 1] != MAP_VWALL && screen[myR - 1][myC + 1] != MAP_HWALL ||  (myR - 1 == HIDDENDOOR_R && myC + 1 == HIDDENDOOR_C))
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
        if (screen[myR - 1][myC - 1] != MAP_VWALL && screen[myR - 1][myC - 1] != MAP_HWALL ||  (myR - 1 == HIDDENDOOR_R && myC - 1 == HIDDENDOOR_C))
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
        if (screen[myR + 1][myC - 1] != MAP_VWALL && screen[myR + 1][myC - 1] != MAP_HWALL ||  (myR + 1 == HIDDENDOOR_R && myC - 1 == HIDDENDOOR_C))
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
        if (screen[myR + 1][myC + 1] != MAP_VWALL && screen[myR + 1][myC + 1] != MAP_HWALL || (myR + 1 == HIDDENDOOR_R && myC + 1 == HIDDENDOOR_C))
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
    
    //other commands
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
        << "DD - move right until you hit an object\n"
        << "WD (or DW) - move diagonally one space up and to the right\n"
        << "WA (or AW) - move diagonally one space up and to the left\n"
        << "SD (or DS) - move diagonally one space down and to the right\n"
        << "SA (or AS) - move diagonally one space down and to the left\n"
        << "I - view inventory\n"
        << "E - view weapons\n"
        << "U - use an object in your inventory\n"
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
            cout << "Your inventory: " << endl;
            for (int d = 0; d < inventory.size(); d++)
            {
                cout << inventory[d] << endl;
            }
        }
    }
    else if (choice == "e" || choice == "E" || choice == "weapons" || choice == "Weapons")
    {
        if (weaponsInventory().size() == 0)
        {
            cout << "You don't have any weapons" << endl;
        }
        else
        {
            cout << "Your weapons: " << endl;
            for (int d = 0; d < weaponsInventory().size(); d++)
            {
                cout << weaponsInventory()[d] << endl;
            }
        }
    }
    
    else if (choice == "R@@ghav") //cheat code
    {
        monster1Alive = false;
        monster2Alive = false;
        room = 2;
        inventory.push_back("stick");
        inventory.push_back("key");
    }
    
    else if (choice != "q" || choice != "Q" || choice != "quit" || choice != "Quit")
    {
        cout << "I don't understand that" << endl;
    }
    
    if (monster1R == myR && monster1C == myC && monster1Alive)			{
        monsterAttack(1, monster1R, monster1C);
    }
    if (monster2R == myR && monster2C == myC && monster2Alive)
    {
        monsterAttack(2, monster2R, monster2C);
    }
    if (monster1R == monster2R && monster1C == monster2C)
    {
        cout << "The monsters bump into each other." << endl;
        cout << "Both monsters lose 20 HP" << endl;
        monster1HP -= 20;
        monster2HP -= 20;
    }
    
    if (!monster1Alive && !monster2Alive && !gatewayOpen)
    {
        cout << "You killed both monsters." << endl;
        cout << "A gateway opens on the right." << endl;
        gatewayOpen = true;
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
            cout << "Would you like to restart the game? [y/n]" << endl;
            cin >> choice;
            if (choice == "y" || choice == "Y" || choice == "yes" || choice == "Yes")
            {
                room = 1;
                if (classChoice == classes[0]) //soldier
                {
                    atk = 15;
                    hp = 150;
                    gold = 0;
                    xp = 10;
                    lvl = 1;
                }
                else if (classChoice == classes[1]) //wizard
                {
                    atk = 25;
                    hp = 100;
                    gold = 0;
                    xp = 10;
                    lvl = 1;
                }
            }
            else
            {
                cout << "Okay..." << endl;
                cout << "Um..." << endl;
                cout << "While we are waiting, what is your name?" << endl;
                cin >> choice;
                cout << "I could care less." << endl;
                cin >> choice;
            }
        }
    }
}
int main()
{
    int temp1 = 0;
    string choice = "";
    string useItem = "";
    int myR = 4;
    int myC = 3;
    monster1R = 5;
    monster1C = 4;
    monster2R = 7;
    monster2C = 8;
    cout << "Welcome to RogueHack! (Definitely not a spin-off of Rogue and NetHack)" << endl;
    
    //classes
    cout << "Pick your class from the following list: " << endl;
    string classChoice = "";
    for (int i = 0; i < classes.size(); i++)
    {
        cout << i + 1 << ". " << classes[i] << endl;
    }
    
    bool temp = false;
    while (!temp) //classes loop
    {
        cin >> classChoice;
        if (classChoice == "1")
        {
            classChoice = classes[0];
        }
        if (classChoice == "2")
        {
            classChoice = classes[1];
        }
        //add more if you add more classes
        if (find(classes.begin(), classes.end(), classChoice) != classes.end()) //classChoice is in classes
        {
            temp = true;
            cout << "You have picked " << classChoice << "." << endl;
            if (classChoice == classes[0]) //soldier
            {
                atk = 15;
                hp = 150;
                gold = 0;
                xp = 10;
                lvl = 1;
            }
            else if(classChoice == classes[1]) //wizard
            {
                atk = 25;
                hp = 100;
                gold = 0;
                xp = 10;
                lvl = 1;
            }
        }
        else
        {
            cout << "Please enter a valid choice." << endl;
        }
    }
    
    cout << "If you want to learn how to play, type 'help'" << endl;
    
    
    while (choice != "q" && choice != "Q" && choice != "quit" && choice != "Quit")
    {
        initScreen();
        setInfo(atk, hp, gold, xp, lvl, room, myR, myC);
        setMap();
        setAttributes(room, gatewayOpen);
        setPlayer(myR, myC);
        if (room == 1)
        {
            if (myR == KEY1_R && myC == KEY1_C && !(find(inventory.begin(), inventory.end(), "key") != inventory.end()))
            {
                inventory.push_back("key");
                cout << "You picked up the object, it was a key." << endl;
                //screen[KEY1_R][KEY1_C] = MAP_EMPTY;
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
            
            if (monster1Alive)
            {
                setMonster(monster1R, monster1C);
                srand((int)time(0));
                int random = (rand() % 3) + 1;
                if (random == 1)
                {
                    monster1R = moveMonsterRow(1, monster1R, monster1C);
                }
                else if (random == 2)
                {
                    monster1C = moveMonsterColumn(monster1R, monster1C);
                }
                else if (random == 3)
                {
                    monster1R = moveMonsterRow(1, monster1R, monster1C);
                    monster1C = moveMonsterColumn(monster1R, monster1C);
                }
            }
            if (monster2Alive)
            {
                setMonster(monster2R, monster2C);
                srand((int)((time(0) * 2) / 5)); //need a better way to seperate from monster1
                int random1 = (rand() % 3) + 1;
                if (random1 == 1)
                {
                    monster2R = moveMonsterRow(2, monster2R, monster2C);
                }
                else if (random1 == 2)
                {
                    monster2C = moveMonsterColumn(monster2R, monster2C);
                }
                else if (random1 == 3)
                {
                    monster2R = moveMonsterRow(2, monster2R, monster2C);
                    monster2C = moveMonsterColumn(monster2R, monster2C);
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
            
            if (myR == GATEWAY1_R && myC == GATEWAY1_C && gatewayOpen)
            {
                room = 4;
                myC = 0;
            }
        }
        else if (room == 3)
        {
            
            if (myR == STICK_R && myC == STICK_C && !(find(inventory.begin(), inventory.end(), "stick") != inventory.end()))
            {
                cout << "You obtain a wooden stick" << endl;
                inventory.push_back("stick");
            }
            
            if (myR == NOTE_R && myC == NOTE_C && !(find(inventory.begin(), inventory.end(), "note") != inventory.end()))
            {
                cout << "You find a note." << endl;
                cout << "It says 'TOP, 15'" << endl;
                inventory.push_back("note");
            }
            
            if (myR == DOOR4_R && myC == DOOR4_C)
            {
                room = 2;
                myR = 0;
            }
            
        }
        else if (room == 4)
        {
            if (myR == DAGGER_R && myC == DAGGER_C && !(find(inventory.begin(), inventory.end(), "dagger") != inventory.end()))
            {
                cout << "You obtain a small dagger" << endl;
                inventory.push_back("dagger");
            }
            if (myR == HEALPOTION_R && myC == HEALPOTION_C && !(find(inventory.begin(), inventory.end(), "healing_potion") != inventory.end()))
            {
                cout << "You obtain a healing potion" << endl;
                inventory.push_back("healing_potion");
            }
            if (myR == DOOR5_R && myC == DOOR5_C)
            {
                room = 2;
                myC = SCREEN_COLS - 1;
            }
            if (myR == HIDDENDOOR_R && myC == HIDDENDOOR_C)
            {
                room = 5;
                myR = SCREEN_ROWS - 2;
            }
        }
        else if (room == 5)
        {
            if (myR == MACHETE_R && myC == MACHETE_C && !(find(inventory.begin(), inventory.end(), "machete") != inventory.end()))
            {
                cout << "You obtain a medium-sized machete" << endl;
                inventory.push_back("machete");
            }
        }
        
        
        
        
        printScreen();
        cout << ">> ";
        cin >> choice;
        
        system("clear");
        
        playerMovements(myR, myC, choice, useItem, classChoice);
        
    }
    
    return 0;
}
