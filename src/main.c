#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>

void handle_resize();
void resize_signal();

int wresized = 0;
int busy = 0;

int main(int argc, char const *argv[])
{
	int ch;

	signal(SIGWINCH, resize_signal);

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();

	while(1) {
		if (wresized)
			handle_resize();

		ch = getch();
		busy = 1;
		addch(ch);
		refresh();
		busy = 0;
	}

	endwin();

	return 0;
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
