#include "Level.h"
#include <stdio.h>
#include <curses.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68
#define ENTER 13 
#define SPACE 32 

int main(int argc, char* argv[]) 
{

	string 		lvlName;
 	Level*	 	lvl = new Level();
	int 		highlighted = 2;

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
	init_pair(1, COLOR_BLACK, COLOR_RED);		/* UI Text			*/
	init_pair(2, COLOR_RED, COLOR_WHITE);		/* Highlighted UI Text		*/
	init_pair(3, COLOR_YELLOW, COLOR_RED);		/* Logo lines			*/
	init_pair(4, COLOR_WHITE, COLOR_RED);		/* Logo lines			*/

	attron(COLOR_PAIR(3));
	int logoTop = 10; 				/* Print logo			*/
	mvprintw(r/2-logoTop, c/2-23, "     __    __    ___   __ _                 ");logoTop--; 
	attroff(COLOR_PAIR(3));attron(COLOR_PAIR(4));
	mvprintw(r/2-logoTop, c/2-23, "    /  \\  /  \\  /  _/ / / /                 ");logoTop--;
	attroff(COLOR_PAIR(4));attron(COLOR_PAIR(3));
	mvprintw(r/2-logoTop, c/2-23, "   / * / / / / / /_  /  <                   ");logoTop--;
	attroff(COLOR_PAIR(3));attron(COLOR_PAIR(4));
	mvprintw(r/2-logoTop, c/2-23, "  /_/\\_\\ \\__/  \\__/ /_/\\_\\                  ");logoTop--;
	attroff(COLOR_PAIR(4));attron(COLOR_PAIR(3));
	mvprintw(r/2-logoTop, c/2-23, "         _  _  _   __    __     _      __   ");logoTop--;
	attroff(COLOR_PAIR(3));attron(COLOR_PAIR(4));
	mvprintw(r/2-logoTop, c/2-23, "        | || |/ / /  \\  /  \\   / /    /  \\  ");logoTop--;
	attroff(COLOR_PAIR(4));attron(COLOR_PAIR(3));
	mvprintw(r/2-logoTop, c/2-23, "        | |  / / / / / / * /  / /__  / / /  ");logoTop--;
	attroff(COLOR_PAIR(3));attron(COLOR_PAIR(4));
	mvprintw(r/2-logoTop, c/2-23, "        |_____/  \\__/ /_/\\_\\ /____/ /__ /   ");logoTop--;
	attroff(COLOR_PAIR(4));attron(COLOR_PAIR(3));
	mvprintw(r/2-logoTop, c/2-23, "                                            ");
	attroff(COLOR_PAIR(3));

	attron(COLOR_PAIR(1));

	int uiTop;
	int temp;
	while (true) {					/* Menu Loop			*/

							/* Print UI			*/		
		uiTop = -2;
		move(r/2-uiTop, 0);
		temp = 0; while( temp < c/2-4 ) {printw(" "); temp++;} uiTop--;
		if (highlighted == 2) {attroff(COLOR_PAIR(1)); attron(COLOR_PAIR(2));}
		mvprintw(r/2-uiTop, c/2-14, " New  Game ");uiTop--;
		if (highlighted == 2) {attroff(COLOR_PAIR(2)); attron(COLOR_PAIR(1));}
		mvprintw(r/2-uiTop, c/2-13, "           ");uiTop--;
		if (highlighted == 1) {attroff(COLOR_PAIR(1)); attron(COLOR_PAIR(2));}
		mvprintw(r/2-uiTop, c/2-12, " Load Game ");uiTop--;
		if (highlighted == 1) {attroff(COLOR_PAIR(2)); attron(COLOR_PAIR(1));}
		mvprintw(r/2-uiTop, c/2-11, "           ");uiTop--;
		if (highlighted == 0) {attroff(COLOR_PAIR(1)); attron(COLOR_PAIR(2));}
		mvprintw(r/2-uiTop, c/2-10, "  Quit It  ");uiTop--;
		if (highlighted == 0) {attroff(COLOR_PAIR(2)); attron(COLOR_PAIR(1));}
		move(r/2-uiTop, c/2-9);
		temp = c/2-9; while( temp < c-1 ) {printw(" "); temp++;} uiTop--;
		move(r-1, 0);

		ch = getch();				/* Handle input			*/
		if (ch == KEY_UP) { 
			highlighted++;
		} else if (ch == KEY_DOWN) {
			highlighted--;
		} else if (ch == ENTER || ch == SPACE) {
			break;
		}

		highlighted = (highlighted+3) % 3;
	}
	
	if (highlighted == 2) { 	// New Game

		char str[80];

		mvprintw(r/2, c/2-20, "Enter Level To Create:                  ");
		move(r/2, c/2+3);
		getstr(str);
		clear();
		endwin();

		lvl->createRandomized(100, 100, 3);
		lvl->setName(str);
		lvl->play();

	} else if (highlighted == 1)   { // Load game
		
		char str[80];

		mvprintw(r/2, c/2-20, "Enter Level To Load:                  ");
		move(r/2, c/2+3);
		getstr(str);
		clear();
		endwin();

		lvl->createFromFile(str);
		lvl->play();

	} else { 			// Quit it

		endwin();

	}

	delete lvl;

	return 0;

}
