#include <ncurses.h>

int main(int argc, char const *argv[])
{
	initscr();			/* Initialize curses 				*/
	printw("Kodapa!");	/* Print 'Kodapa!' to the screen 	*/
	refresh();			/* Print it to the actual screen	*/
	getch();			/* Wait for user input				*/
	endwin();			/* End curses mode					*/

	return 0;
}
