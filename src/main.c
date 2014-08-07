#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include "log.h"

void interrupt_signal();
void resize_signal();
void handle_resize();
void handle_input();
void kodapa_exit();

int wresized = 0;
int busy = 0;

int main(int argc, char const *argv[])
{
	klog("== kodapa start ==");

	int ch;

	signal(SIGWINCH, resize_signal);
	signal(SIGINT, interrupt_signal);

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();

	while(1) {
		if (wresized)
			handle_resize();

		ch = getch();
		busy = 1;
		if(ch == 27) break;
		handle_input(ch);
		busy = 0;
	}

	kodapa_exit();
}

void kodapa_exit()
{
	endwin();
	klog("== kodapa end ==");
	exit(0);
}

void interrupt_signal()
{
	klog("SIGINT. Exiting...");
	kodapa_exit();
}

void handle_input(int ch)
{
	int y, x;
	getyx(stdscr, y, x);

	switch(ch) {
		case KEY_UP:
			move(y - 1, x);
			break;
		case KEY_DOWN:
			move(y + 1, x);
			break;
		case KEY_LEFT:
			move(y, x - 1);
			break;
		case KEY_RIGHT:
			move(y, x + 1);
			break;
		default:
			addch(ch);
			refresh();
			break;
	}
}

void handle_resize()
{
	addch('r');
	refresh();
	wresized = 0;
}

void resize_signal()
{
	if(busy)
		wresized = 1;
	else
		handle_resize();
}
