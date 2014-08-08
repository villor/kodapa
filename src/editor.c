#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>

#include "editor.h"
#include "log.h"
#include "file.h"

WINDOW *ewin;

int editor_init()
{
	editor_resized();
	return 0;
}

void editor_cleanup()
{
	delwin(ewin);
}

void editor_resized()
{
	int wrow, wcol;

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, wrow, wcol);

	delwin(ewin);
	ewin = newwin(wrow,wcol, 0, 0);
	wrefresh(ewin);
}

void move_cursor(int y, int x) {
	wmove(ewin, y, x);
	wrefresh(ewin);
}

void editor_input(int ch)
{
	int y, x;
	getyx(ewin, y, x);

	switch(ch) {
		case KEY_UP:
			move_cursor(y - 1, x);
			break;
		case KEY_DOWN:
			move_cursor(y + 1, x);
			break;
		case KEY_LEFT:
			move_cursor(y, x - 1);
			break;
		case KEY_RIGHT:
			move_cursor(y, x + 1);
			break;
		default:
			if(isprint(ch)) {
				waddch(ewin, ch);
				wrefresh(ewin);
			}
			break;
	}
}

