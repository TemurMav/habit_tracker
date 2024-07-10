#include <stdlib.h>
#include <stdio.h>

FILE* check_file(char *data){
	FILE *file;
	if ((file = fopen(data, "w+")) == NULL){
		fprintf(stderr, "Can`t created file.\n");
		exit(1);
	}
	return file;
}
