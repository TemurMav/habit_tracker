#ifndef HABIT
#define HABIT

#define MAX_HABIT_LEN 20
#define MAX_LINE_LEN 80
#define BRIGHT_BLACK 8
#define ENTER 10
#define DELETE 127

struct Habit {
	char habit_name[MAX_HABIT_LEN];
	struct tm *date;
	int count;
};

void print_help();

FILE* check_file(char*);

int get_len_file(FILE*);

void table_print();

void creat_new_habit(int, struct Habit*);

#endif
