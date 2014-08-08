#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>

#include "editor.h"
#include "log.h"
#include "file.h"

void draw_text();
void move_cursor(int y, int x);

WINDOW *ewin;

int linepos = 0;

int editor_init()
{
	initscr();
	cbreak();
	noecho();
	refresh();

	editor_resized();

	return 0;
}

void editor_cleanup()
{
	delwin(ewin);
	endwin();
}

void editor_resized()
{
	int wrow, wcol;
	int oldcury, oldcurx;

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, wrow, wcol);

	getyx(ewin, oldcury, oldcurx);
	delwin(ewin);
	ewin = newwin(wrow,wcol, 0, 0);
	keypad(ewin, TRUE);
	move_cursor(oldcury, oldcurx);
	wrefresh(ewin);

	draw_text();
}

void move_cursor(int y, int x)
{
	wmove(ewin, y, x);
	wrefresh(ewin);
}

int editor_get_input()
{
	return wgetch(ewin);
}

void editor_key_press(int ch)
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

			break;
	}
}

void draw_text()
{
	int eheight, txtheight, li;
	int oldcury, oldcurx;

	getyx(ewin, oldcury, oldcurx);
	wmove(ewin, 0, 0);

	eheight = getmaxy(ewin);
	wclear(ewin);

	if (linepos > n_line_indices)
		linepos = n_line_indices;

	txtheight = n_line_indices - linepos;
	if (txtheight < 0)
		txtheight = 0;
	if (txtheight > eheight)
		txtheight = eheight;

	for (li = linepos; li < txtheight; li++) {
		int i = 0;
		while(1) {
			if(file_buffer[line_indices[li] + i] != '\n' && file_buffer[line_indices[li] + i] != '\0') {
				mvwaddch(ewin, li, i, file_buffer[line_indices[li] + i]);
				i++;
				continue;
			}
			break;
		}
	}

	move_cursor(oldcury, oldcurx);
	wrefresh(ewin);
}
