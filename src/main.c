#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <habit.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>


int main(int argc, char *argv[]){
	if (argc < 1){
		fprintf(stderr, "Fatal error.\n");
		return 1;
	}

	if (argc == 2 && strcmp(argv[1], "-h") == 0){
		print_help();
		return 0;
	}
	
	char data[] = "data/data.habit";

	int fd = open(data, O_RDWR);
	if (fd == -1){
    exit(2);
	}

	uint32_t n; 
	

	ssize_t header_read_bytes = read(fd, &n, sizeof(uint32_t));
	if (header_read_bytes != sizeof(uint32_t)) {
		fprintf(stderr, "Wrong file size: %zd\n", header_read_bytes);
		close(fd);
		exit(3);
	}

	off_t file_len = sizeof(uint32_t) + n * sizeof(struct Habit);
    
	void *start_mapping = mmap(NULL, file_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (start_mapping == MAP_FAILED) {
		fprintf(stderr, "mmap failed: %s\n", strerror(errno));
		close(fd);
		exit(5);
	}

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
	else{
		habit_list = (struct Habit *)malloc(n*sizeof(struct Habit));
		memcpy(habit_list, (struct Habit *)(start_mapping + sizeof(uint32_t)), n*sizeof(struct Habit)); 
		for (int i = 0; i < n; i++){
			mvprintw(y, 0, "%s", habit_list[i].habit_name);
			time_t now;
			time(&now);
			if (now - habit_list[i].date < 24* 3600 && habit_list[i].count > 0){
				mvprintw(y, 27, "[x]");
			}
			else{
				mvprintw(y, 27, " []");
			}
			mvprintw(y++, 21, "%5d", habit_list[i].count);
		}
		mvchgat(y = 2, x, HORIZONTAL_LEN, A_REVERSE, 0, NULL);
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
			case 'x':
				check_habit(&habit_list[y-2], y);
				mvchgat(y, x, HORIZONTAL_LEN, A_REVERSE, 0, NULL);
				break;
			case 'p':
				for (int i = 0; i < n; i++){
					printw("%s\n", habit_list[i].habit_name);
					printw("%d\n", habit_list[i].count);
					struct tm *tmp;
					tmp = localtime(&habit_list[i].date);
					printw("Year: %d\n", tmp->tm_year + 1900); 
  				printw("Mon: %d\n", tmp->tm_mon + 1);  
  				printw("day: %d\n", tmp->tm_mday);
  				printw("h: %d\n", tmp->tm_hour);
  				printw("m: %d\n", tmp->tm_min);
  				printw("s: %d\n", tmp->tm_sec);
				}
				break;
		}
	}
	

	off_t old_file_len = file_len;
	file_len = sizeof(uint32_t) + n * sizeof(struct Habit);

	struct Habit *buffer = malloc(file_len - sizeof(n));
	memcpy(buffer, habit_list, file_len - sizeof(n));

	munmap(start_mapping, old_file_len);
	if (ftruncate(fd, file_len) == -1) {
		perror("ftruncate");
    exit(5);
  }
	
	start_mapping = mmap(NULL, file_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (start_mapping == MAP_FAILED) {
		perror("mmap failed");
		close(fd);
    exit(EXIT_FAILURE);
  }

	memcpy((struct Habit *)(start_mapping + sizeof(n)), buffer, file_len - sizeof(n));

	*((uint32_t *)start_mapping) = n;
	int sync_result = msync(start_mapping, file_len, MS_SYNC);
	if (sync_result == -1) {
		fprintf(stderr, "msync failed: %s\n", strerror(errno));
		munmap(start_mapping, file_len);
		close(fd);
		exit(6);
	}

	endwin();

	free(habit_list);
	munmap(start_mapping, file_len);
	close(fd);
    
	return 0;
}
