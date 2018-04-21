#include "Level.h"
#include "separate.h"

#include <curses.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string.h>
#include <stdlib.h>

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68

using namespace std;

//Format:
/* <levelName>,<numRows>,<numCols>,<numFood>
 * <int[0][0]>,<int[0][1]>, ect..
 * <int[1][0]>,<int[1][1]>, ect..
 * ect..
 **/
Level::Level() 
{
	// This doesn't do anything rn.
}

void 
Level::createRandomized(int r, int c, int m) {

	// Get the meta info
	numRows = r; 
	numCols = c;
	if (m < 2) {
		m = 2;
	}

	// Get actual map
	vector<vector<int>> map;
	for (int i = 0; i < numRows; i++)
	{

		// Get a row
		vector<int> intRow;
		int numRocks;

		for (int j = 0; j < numCols; j++)
		{
			int numRocks = 0;	
			
			while (rand() % m == 0) {numRocks++;}

			intRow.push_back(numRocks);	
		}

		map.push_back(intRow);	
	}

	this->map = map;
}

void 
Level::createFromFile(string filename) {

	// Open the file
	ifstream fin;
	fin.open("./level/" + filename + ".lvl");
	if (!fin.is_open()) {
		printf("ERROR: File not found.\n");
		exit(0);
	}

	// Get the meta info
	string line;
	getline(fin, line);
	vector<string> sep_line = separate(line, ',');
	name = sep_line[0];
	numRows = atoi(sep_line[1].c_str());
	numCols = atoi(sep_line[2].c_str());

	// Get actual map
	vector<vector<int>> map;
	for (int i = 0; i < numRows; i++)
	{

		// Get a row
		getline(fin, line);
		sep_line = separate(line, ',');

		vector<int> intRow;
		for (int j = 0; j < numCols; j++)
		{
			intRow.push_back(atoi(sep_line[j].c_str()));	
		}

		map.push_back(intRow);	
	}

	this->map = map;

}


// Creates clone of default.lvl with given name.
// Writes lvl to file.
void
Level::createDefault(string lvlName) {
	createFromFile("trial");
	this->name = lvlName;
}

void 
Level::writeToFile() {

	// Open stream
	ofstream fout;
	fout.open(("./level/" + name + ".lvl"), ifstream::trunc);

	// Meta info
	fout << name + "," + to_string(numRows) + "," + to_string(numCols) + "\n";

	for (int i = 0; i < numRows; i++)
	{

		// Make a row into a 'put'-able string
		string line = "";

		for (int j = 0; j < numCols; j++)
		{
		   	line += to_string(map[i][j]);

			if (j < (numCols - 1)) 
				line += ",";
		}

		fout << line + "\n";

	}

	fout.close(); 


}

int
Level::play() 
{

	int nLoop = 0;
	
	int x=this->map.size()/2; 	/* Always start in the center			*/
	int y=this->map[0].size()/2;
	int facing = 0;			/* Store which way you are facing		*/
					/* 0=north, 1=east, 2=south, 3=west		*/

	int nRocks = 0;			/* Store number of rocks in inventory		*/
	int rockCheckPosX = 0;
	int relRockCheckPosX = 0;
	int rockCheckPosY = 0;
	int relRockCheckPosY = 0;

	initscr();			/* Screen initialization			*/
	int r, c, ch; char p = 254;	/* Cursor variable initialization		*/
	getmaxyx(stdscr, r, c);		/* Get size of current cmd			*/

	if (has_colors() == false) {	/* Handle color initialization			*/
		endwin();
		printf("Your puny terminal can not *handle* color\n");
		exit(1);
	}
	start_color();

	// Commonly used colors
	init_pair(1, COLOR_WHITE, COLOR_BLACK);		/* Default spawned rocks	*/
	init_pair(2, COLOR_BLACK, COLOR_RED);		/* UI Text			*/
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);	/* User placed rocks		*/
	init_pair(4, COLOR_BLUE, COLOR_CYAN);		/* Player			*/
	init_pair(5, COLOR_CYAN, COLOR_WHITE);		/* Highlighted rock		*/
	
	while (true) 			/* Main loop					*/
	{
		
		for (int i = 0; i < this->map.size(); i++)	/* Display map		*/
		{
			for (int j = 0; j < this->map[0].size(); j++) 
			{
				int relx = (r/2) + i - x;
				int rely = (c/2) + j - y;
				if (relx < 0 || relx > r-1 || rely < 0 || rely > c-2) {continue;}
				move(relx, rely);

				if (i == rockCheckPosX && j == rockCheckPosY) {
					attron(COLOR_PAIR(5));
					if (this->map[i][j] != 0) {	/* Print highlighted	*/
						std::string temp(1, (char)(48 + this->map[i][j]));
						printw(temp.c_str());
					} else {printw(" ");}
					attroff(COLOR_PAIR(5));
				} else {
					if (this->map[i][j] != 0) {	/* Print map	*/
						std::string temp(1, (char)(48 + this->map[i][j]));
						printw(temp.c_str());
					}
				}	
			}
		}

		attron(COLOR_PAIR(4));
		move(r/2, c/2);
		printw("&");	/* Print player "sprite"	*/
		attroff(COLOR_PAIR(4));

		attron(COLOR_PAIR(2));
		move (0, 0);	/* Display UI		*/
		printw("               ");
		move (1, 0);	/* Display rocks	*/
		printw("  nRocks: %3d  ", nRocks);
		move (2, 0);	/* Display UI		*/
		printw("               ");
		attroff(COLOR_PAIR(2));

		move(r-1, 0);	/* Move back to default position*/

		// Get command
		ch = getch();
		
		// Carry out command
		if (ch == KEY_LEFT ) {
			if (facing == 3 && this->map[x][y-1] < 3) {
				y--;
			} else {
				facing=3;
			}
		} else if (ch == KEY_RIGHT) {
			if (facing == 1 && this->map[x][y+1] < 3) {
				y++;
			} else { 
				facing=1;
			}
		} else if (ch == KEY_UP) { 
			if (facing == 0 && this->map[x-1][y] < 3) {
				x--;
			} else { 
				facing = 0;
			}
		} else if (ch == KEY_DOWN) {
			if (facing == 2 && this->map[x+1][y] < 3) {
				x++;
			} else { 
				facing = 2;
			}
		} 
		else if (ch == 'q') { writeToFile(); break;}	// quit
		else if (ch == 'z') 			// pick up rock
		{
			// Check if it can be affected, affect it
			if (this->map[rockCheckPosX][rockCheckPosY] > 0) {
				this->map[rockCheckPosX][rockCheckPosY]--;
				nRocks++;
			}
			
		}
		else if (ch == 'x') {			// drop rock

			// Check if you've got the rocks, drop 'em
			if (nRocks > 0) {
				this->map[rockCheckPosX][rockCheckPosY]++;
				nRocks--;
			}
		
		} 		

		switch (facing) 	/* Recalculate which rock you're facing 	*/
		{
			case 0: rockCheckPosX=x-1; rockCheckPosY=y; 
				relRockCheckPosX= -1; relRockCheckPosY=0; break;
			case 1: rockCheckPosX=x; rockCheckPosY=y+1; 
				relRockCheckPosX=0; relRockCheckPosY=1; break;
			case 2: rockCheckPosX=x+1; rockCheckPosY=y;
				relRockCheckPosX=1; relRockCheckPosY=0; break;
			case 3: rockCheckPosX=x; rockCheckPosY=y-1;
				relRockCheckPosX=0; relRockCheckPosY= -1; break;
		}

		clear();

	} 

	clear();

	getch();

	endwin();

	return 1;
}

int
Level::playcolorless() 
{
	int nLoop = 0;
	
	int x=this->map.size()/2; 	/* Always start in the center			*/
	int y=this->map[0].size()/2;

	int facing = 0;			/* Store which way you are facing		*/
					/* 0=north, 1=east, 2=south, 3=west		*/

	int nRocks = 0;			/* Store number of rocks in inventory		*/

	initscr();			/* Screen initialization			*/
	int r, c, ch; char p = 254;	/* Cursor variable initialization		*/
	getmaxyx(stdscr, r, c);		/* Get size of current cmd			*/

	while (true) 			/* Main loop					*/
	{
		
		for (int i = 0; i < this->map.size(); i++)	/* Display map		*/
		{
			for (int j = 0; j < this->map[0].size(); j++) 
			{
				int relx = (r/2) + i - x;
				int rely = (c/2) + j - y;
				if (relx < 0 || relx > r-1 || rely < 0 || rely > c-2) {continue;}
				move(relx, rely);

				if (this->map[i][j] != 0) {	/* Print map	*/
					std::string temp(1, (char)(48 + this->map[i][j]));
					printw(temp.c_str());
				}
			}
		}

		move(r/2, c/2);
		cout << "\033[1;31";
		printw("&");	/* Print player "sprite"	*/
		cout << "\033[0m";
		switch (facing) /* Print player direction	*/
		{
			case 0: mvprintw(r/2-1, c/2, "+"); break;
			case 1: mvprintw(r/2, c/2+1, "+"); break;
			case 2: mvprintw(r/2+1, c/2, "+"); break;
			case 3: mvprintw(r/2, c/2-1, "+"); break;
		}
		move(r, 0);
		

		// Get command
		/*if ((*/ch = getch()/*) < 65 || ch > 68 ) {continue;}*/;
		
		// Carry out command
		if (ch == KEY_LEFT && this->map[x][y-1] < 3) {y--; facing=3;}		// Left
		else if (ch == KEY_RIGHT && this->map[x][y+1] < 3) {y++; facing=1;} 	// Right
		else if (ch == KEY_UP && this->map[x-1][y] < 3) {x--; facing=0;} 	// Up
		else if (ch == KEY_DOWN && this->map[x+1][y] < 3) {x++; facing=2;} 	// Down
		else if (ch == 'q') {break;} 		// quit
		else if (ch == 'z') 			// pick up rock
		{
			// Get spot to be affected
			int rockCheckPosX = -1;
			int rockCheckPosY = -1;
			switch (facing) 
			{
				case 0: rockCheckPosX=x-2; rockCheckPosY=y; break;
				case 1: rockCheckPosX=x; rockCheckPosY=y+2; break;
				case 2: rockCheckPosX=x+2; rockCheckPosY=y; break;
				case 3: rockCheckPosX=x; rockCheckPosY=y-2; break;
			}

			// Check if it can be affected, affect it
			if (this->map[rockCheckPosX][rockCheckPosY] > 0) {
				this->map[rockCheckPosX][rockCheckPosY]--;
				nRocks++;
			}
			
		}
		else if (ch == 'x') {			// drop rock

			// Get spot to be affected
			int rockCheckPosX = -1;
			int rockCheckPosY = -1;
			switch (facing) 
			{
				case 0: rockCheckPosX=x-2; rockCheckPosY=y; break;
				case 1: rockCheckPosX=x; rockCheckPosY=y+2; break;
				case 2: rockCheckPosX=x+2; rockCheckPosY=y; break;
				case 3: rockCheckPosX=x; rockCheckPosY=y-2; break;
			}

			// Check if you've got the rocks, drop 'em
			if (nRocks > 0) {
				this->map[rockCheckPosX][rockCheckPosY]++;
				nRocks--;
			}
		
		} 		

		clear();

	} 

	clear();

	getch();

	endwin();

	return 1;
}


string
Level::getName() {return this->name;}

void
Level::setName(std::string n) {
	this->name = n;
}

const char *
Level::getCName() {return this->name.c_str();}
