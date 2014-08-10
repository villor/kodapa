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
	int eheight;
	eheight = getmaxy(ewin);

	/* Scroll up */
	if (y < 0 && linepos > 0) {
		linepos--;
		draw_text();
	}

	/* Scroll down */
	if (y > eheight - 1 && linepos < n_line_indices) {
		linepos++;
		draw_text();
	}

	if (y - linepos > n_line_indices - 1)
		return;

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


	if (linepos > n_line_indices - eheight)
		linepos = n_line_indices - eheight;
	if (linepos < 0)
		linepos = 0;

	txtheight = n_line_indices - linepos;
	if (txtheight > eheight)
		txtheight = eheight;
	if (txtheight < 1)
		txtheight = 1;

	for (li = linepos; li - linepos < txtheight; li++) {
		int i = 0;
		while(1) {
			if(file_buffer[line_indices[li] + i] != '\n'
			   && file_buffer[line_indices[li] + i] != '\0') {
				mvwaddch(ewin,
					 li - linepos,
					 i,
					 file_buffer[line_indices[li] + i]);
				i++;
				continue;
			}
			break;
		}
	}

	move_cursor(oldcury, oldcurx);
	wrefresh(ewin);
}
