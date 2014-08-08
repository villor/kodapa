#include <ncurses.h>
#include <stdlib.h>
#include <signal.h>
#include "log.h"
#include "file.h"
#include "editor.h"

void interrupt_signal();
void resize_signal();
void kodapa_exit();
void init_ncurses();

int wresized = 0;
int busy = 0;

int main(int argc, char const *argv[])
{
	klog("== kodapa start ==");
	int ch;

	if(argc > 0)
		file_open(argv[1]);
	else
		file_open("");

	signal(SIGWINCH, resize_signal);
	signal(SIGINT, interrupt_signal);

	init_ncurses();
	editor_init();

	while(1) {
		if (wresized) {
			editor_resized();
			wresized = 0;
		}

		ch = getch();
		busy = 1;
		if(ch == 27) break;
		editor_input(ch);
		busy = 0;
	}

	kodapa_exit();
}

void kodapa_exit()
{
	editor_cleanup();
	endwin();
	file_save("testsave.file");
	klog("== kodapa end ==");
	exit(0);
}

void init_ncurses()
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	refresh();
}

void interrupt_signal()
{
	klog("SIGINT. Exiting...");
	kodapa_exit();
}

void resize_signal()
{
	if(busy) {
		wresized = 1;
	} else {
		editor_resized();
		wresized = 0;
	}
}
