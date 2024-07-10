#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <habit.h>

int main(int argc, char *argv[])
{
	if (argc < 1){
		fprintf(stderr, "Fatal error.\n");
		return 1;
	}

	if (argc == 2 && strcmp(argv[1], "-h") == 0){
		print_help();
		return 0;
	}
	
	char data[] = "data/data.txt";
	char header_name[] = "Habit name";
	char header_count[] = "Count";
	FILE *file;
	file = check_file(data);

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	printw("%s", header_name);
	mvprintw(0, 21, "%s", header_count);
	getch();
	endwin();
    
	return 0;
}


