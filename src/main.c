#include <ncurses.h>

int main(int argc, char const *argv[])
{
	initscr();
	printw("Kodapa!");
	refresh();
	getch();
	endwin();

	return 0;
}
