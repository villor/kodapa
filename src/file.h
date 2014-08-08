#ifndef KODAPA_FILE_H
#define KODAPA_FILE_H

extern char *file_buffer;
extern int *line_indices;
extern int n_line_indices;

void file_open(const char* path);
int file_save(const char* path);

#endif
