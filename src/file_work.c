#include <stdlib.h>
#include <stdio.h>
#include <habit.h>
#include <time.h>

struct Habit {
	char habit_name[MAX_HABIT_LEN];
	struct tm *date;
	int count;
	
};

FILE* check_file(char *data){
	FILE *file;
	if ((file = fopen(data, "r")) == NULL){
		fprintf(stderr, "Can`t opened file.\n");
		exit(1);
	}

	return file;
}
