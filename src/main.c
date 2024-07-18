#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <habit.h>

#define MAX_LEN_LINE 25

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
	char line[MAX_LEN_LINE];
	int x, y;
	y = 2;
	x = 0;

	char tmp;
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	while((tmp = getch()) != 'q'){
		switch(tmp){
			case 'p':
				printw("%s", "Hey");
				break;
		}
	}


	printw("%s", header_name);
	while(fgets(line, sizeof(line), file) != NULL){
		mvprintw(y, x, "%s", line);
		y++;
	}
	mvprintw(0, 21, "%s", header_count);
	mvhline(1, 0, '-', 25);
	mvvline(0, 20, '|', 20);
	refresh();
	getch();
	endwin();

	fclose(file);
    
	return 0;
}


