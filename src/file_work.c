#include <stdlib.h>
#include <stdio.h>
#include <habit.h>
#include <time.h>
#include <ncurses.h>



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
}

void table_print(){
	char header_name[] = "Habit name";
	char header_count[] = "Count";

	printw("%s", header_name);
	mvprintw(0, 21, "%s", header_count);
	mvhline(1, 0, '-', 25);
	mvvline(0, 20, '|', 20);
	refresh();
}
