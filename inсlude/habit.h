#ifndef HABIT
#define HABIT

#define MAX_HABIT_LEN 20
#define MAX_LINE_LEN 80
#define BRIGHT_BLACK 8
#define ENTER 10
#define DELETE 127
#define NORMAL 1
#define CREAT 2
#define DELITE 3
#define HORIZONTAL_LEN 31

struct Habit {
	char habit_name[MAX_HABIT_LEN];
	struct tm *date; /*last update*/
	int count;
};

void print_help();

FILE* check_file(char*);

int get_len_file(FILE*);

void table_print();

void creat_new_habit(int, struct Habit*);

void print_status(int);

void remove_habit(struct Habit*, int*, int);

#endif
