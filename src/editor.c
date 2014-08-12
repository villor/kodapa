#include <ncurses.h>
#include <stdlib.h>
#include <ctype.h>

#include "editor.h"
#include "log.h"
#include "file.h"

void draw_text();
void move_cursor(int y, int x);
int init_ln_window(int y, int x, int height);
int calc_num_digits(int num);
int linelen(int index);

WINDOW *ewin;	/* Editor window */
WINDOW *lnwin;	/* Line-number window */

int linepos = 0;

int editor_init()
{
	initscr();
	cbreak();
	noecho();
	refresh();

	set_escdelay(25);

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
	int lnwinwidth;

	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, wrow, wcol);

	lnwinwidth = init_ln_window(0, 0, wrow);

	getyx(ewin, oldcury, oldcurx);
	delwin(ewin);
	ewin = newwin(wrow, wcol - lnwinwidth, 0, lnwinwidth);
	keypad(ewin, TRUE);
	move_cursor(oldcury, oldcurx);
	wrefresh(ewin);

	draw_text();
}

int init_ln_window(int y, int x, int height)
{
	int width = calc_num_digits(n_line_indices) + 2;

	delwin(lnwin);
	lnwin = newwin(height, width, y, x);
	wrefresh(lnwin);

	return width;
}

void move_cursor(int y, int x)
{
	int eheight;
	int lwidth;

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

	lwidth = linelen(line_indices[linepos + y]);
	if (x > lwidth) {
		x = lwidth;
	}

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

	curs_set(0);
	getyx(ewin, oldcury, oldcurx);
	wmove(ewin, 0, 0);

	eheight = getmaxy(ewin);
	wclear(ewin);
	wclear(lnwin);

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
		int x = 0;
		while(1) {
			if(file_buffer[line_indices[li] + i] != '\n'
			   && file_buffer[line_indices[li] + i] != '\0') {
				mvwaddch(ewin,
					 li - linepos,
					 x,
					 file_buffer[line_indices[li] + i]);

				if (file_buffer[line_indices[li] + i] == '\t')
					x += TABSIZE;
				else
					x++;

				i++;
				continue;
			}
			break;
		}

		mvwprintw(lnwin, li - linepos, calc_num_digits(n_line_indices) - calc_num_digits(li + 1) + 1, "%d", li + 1);
	}

	wrefresh(lnwin);

	move_cursor(oldcury, oldcurx);
	wrefresh(ewin);
	curs_set(1);
}

int linelen(int index)
{
	int i = 0;
	int length = 0;
	while(1) {
		if(file_buffer[index + i] != '\n'
		   && file_buffer[index + i] != '\0') {
			if (file_buffer[index + i] == '\t')
				length += TABSIZE;
			else
				length++;

			i++;
			continue;
		}
		break;
	}
	return length;
}

int calc_num_digits(int num)
{
	int count = 0;
	while (num != 0) {
		num /= 10;
		count++;
	}
	return count;
}
