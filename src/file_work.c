#include <stdlib.h>
#include <stdio.h>
#include <habit.h>
#include <time.h>
#include <ncurses.h>
#include <string.h>


FILE* check_file(char *data){
	FILE *file;
	if ((file = fopen(data, "r")) == NULL){
		fprintf(stderr, "Can`t opened file.\n");
		exit(1);
	}

	return file;
}

int get_len_file(FILE *file){
	char tmp[MAX_LINE_LEN];	
	int count = 0;
	
	while(fgets(tmp, sizeof(tmp), file) != NULL){
		count++;
	}
	
	rewind(file);

	return count;
}

void creat_new_habit(int last_y_poss, struct Habit *habit){
	print_status(CREAT);
	char tmp;
	int x = 0;
	time_t current_time;
	struct tm *habit_time;

	move(last_y_poss, x);
	
	while((tmp = getch()) != ENTER && x != MAX_HABIT_LEN - 1){
		if (tmp == DELETE){
			if (x == 0){
			}
			else{
				move(last_y_poss, --x);
				addch(' ' | A_REVERSE);
				move(last_y_poss, x);
			}
		}
		else{
			addch(tmp | A_REVERSE);
			habit->habit_name[x] = tmp;
			x++;
		}
	}
	
	current_time = time(NULL);
	habit_time = localtime(&current_time);
	habit_time->tm_sec = 1;
	habit_time->tm_min = 0;
	habit_time->tm_hour = 0;

	habit->habit_name[x] = '\0';
	habit->count = 0;
	habit->date = habit_time;
	print_status(NORMAL);
}

void remove_habit(struct Habit *habit, int *number_habit, int del_element){
	for (int i = del_element; i < *number_habit - 1; i++){
		strcpy(habit[i].habit_name, habit[i+1].habit_name);
		habit[i].count = habit[i+1].count;
		habit[i].date = habit[i+1].date;
		mvhline(i+2, 0, ' ', 19);
		mvprintw(i+2, 0, "%s", habit[i].habit_name);
		mvhline(i+2, 21, ' ', HORIZONTAL_LEN);
		mvprintw(i+2, 21, "%5d", habit[i].count);
		mvaddch(i+2, 26, '|');
		mvprintw(i+2, 27, " []");
	}

	int last_el = *number_habit;
	
	mvhline(last_el+1, 0, ' ', 19);
	mvhline(last_el+1, 21, ' ', HORIZONTAL_LEN);
	mvchgat(del_element + 2, 0, HORIZONTAL_LEN, A_REVERSE, 0, NULL);
	mvaddch(last_el+1, 26, '|');
	
	if (last_el > 1){
		habit = realloc(habit, (--(*number_habit))*sizeof(struct Habit));
	}
}

void print_status(int status){
	int row, col;

	getmaxyx(stdscr,row,col);

	if (status == NORMAL){
		attron(A_REVERSE);
		mvprintw(row - 1, 0, "NORMAL");
		attroff(A_REVERSE);
	}
	else if (status == CREAT){
		attron(A_REVERSE);
		mvprintw(row - 1, 0, "CREAT ");
		attroff(A_REVERSE);
	}
}

void table_print(){
	char header_name[] = "Habit name";
	char header_count[] = "Count";

	printw("%s", header_name);
	mvprintw(0, 21, "%s", header_count);
	mvhline(1, 0, '-', 31);
	mvvline(0, 20, '|', 20);
	mvprintw(0, 27, "%s", "done");
	mvvline(0, 26, '|', 20);
	refresh();
}
