#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "file.h"

char *file_buffer;
int *line_indices;
int n_line_indices = 1;

void file_open(const char *path)
{
	FILE *fp;
	int fsize;
	if((fp = fopen(path, "r"))) {
		fseek(fp, 0L, SEEK_END);
		fsize = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		file_buffer = (char*) malloc(fsize + 1);

		int bufindex = 0;
		char c;
		while ((c = fgetc(fp)) != EOF) {
			file_buffer[bufindex] = c;
			if(c == '\n')
				n_line_indices++;
			bufindex++;
    		}

	} else {
		fsize = 0;
		file_buffer = (char*) malloc(fsize + 1);
		n_line_indices = 1;
	}
	fclose(fp);

	file_buffer[fsize] = '\0';
	line_indices = (int*) malloc(sizeof (int) * n_line_indices);
	line_indices[0] = 0;

	int curr_index = 1;
	for (int i = 0; i < strlen(file_buffer); i++) {
		if (file_buffer[i] == '\n') {
			line_indices[curr_index] = i + 1;
			curr_index++;
		}
	}

	klog("File opened. %d lines.", n_line_indices);
}

int file_save(const char *path)
{
	FILE *fp;
	if((fp = fopen(path, "w"))) {
		fputs(file_buffer, fp);
		fclose(fp);
		klog("File saved(%d lines): %s", n_line_indices, path);
		return 0;
	}
	fclose(fp);

	klog("Failed to save file '%s', error code: %d", path, fp);
	return 1;
}
