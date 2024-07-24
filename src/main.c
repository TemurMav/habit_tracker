#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <habit.h>
#include <time.h>

int main(int argc, char *argv[]){
	if (argc < 1){
		fprintf(stderr, "Fatal error.\n");
		return 1;
	}

	if (argc == 2 && strcmp(argv[1], "-h") == 0){
		print_help();
		return 0;
	}
	
	char data[] = "data/data.txt";

	FILE *file;
	file = check_file(data);
	char line[MAX_LINE_LEN];
	int x, y;
	y = 2;
	x = 0;

	char tmp;
	struct Habit *habit_list;

	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	start_color();
	curs_set(0);

	int n = get_len_file(file);

	table_print();

	if (n == 0){
		mvchgat(n+2, 0, HORIZONTAL_LEN, A_REVERSE, 0, NULL);
		habit_list = (struct Habit*)malloc((++n)*sizeof(struct Habit));
		if(habit_list == NULL){
			fprintf(stderr, "Can`t malloc.\n");
			return 2;
		}
		creat_new_habit(y, &habit_list[n-1]);
		attron(A_REVERSE);
		mvprintw(y, 21, "%5d", habit_list[n-1].count);
		mvprintw(y, 27, " []");
		attroff(A_REVERSE);
	}

	print_status(NORMAL);
	while((tmp = getch()) != 'q'){
		switch(tmp){
			case 'n':
				mvchgat(y++, x, HORIZONTAL_LEN, A_NORMAL, 0, NULL);
				mvchgat(y = (++n)+1, x, HORIZONTAL_LEN, A_REVERSE, 0, NULL);
				habit_list = realloc(habit_list, (n)*sizeof(struct Habit));
				if(habit_list == NULL){
					fprintf(stderr, "Can`t realloc.\n");
					return 2;
				}
				creat_new_habit(y, &habit_list[n-1]);
				attron(A_REVERSE);
				mvprintw(y, 21, "%5d", habit_list[n-1].count);
				mvprintw(y, 27, " []");
				attroff(A_REVERSE);
				break;
			case 'j':
				if (y < n+1){
					mvchgat(y++, x, HORIZONTAL_LEN, A_NORMAL, 0, NULL);
					mvchgat(y, x, HORIZONTAL_LEN, A_REVERSE, 0, NULL);
				}
				break;
			case 'k':
				if (y > 2){
					mvchgat(y--, x, HORIZONTAL_LEN, A_NORMAL, 0, NULL);
					mvchgat(y, x, HORIZONTAL_LEN, A_REVERSE, 0, NULL);
				}
				break;
			case 'd':
				if (n > 0){
					remove_habit(habit_list, &n, y-2);
				}
				break;
			case 'p':
				for (int i = 0; i < n; i++){
					printw("%s\n", habit_list[i].habit_name);
					printw("%d\n", habit_list[i].count);
					printw("Year: %d\n", habit_list[i].date->tm_year + 1900); 
  				printw("Mon: %d\n", habit_list[i].date->tm_mon + 1);  
  				printw("day: %d\n", habit_list[i].date->tm_mday);
  				printw("h: %d\n", habit_list[i].date->tm_hour);
  				printw("m: %d\n", habit_list[i].date->tm_min);
  				printw("s: %d\n", habit_list[i].date->tm_sec);
				}
				break;
		}
	}


	while(fgets(line, sizeof(line), file) != NULL){
		mvprintw(y, x, "%s", line);
		y++;
	}
	endwin();

	fclose(file);
    
	return 0;
}
