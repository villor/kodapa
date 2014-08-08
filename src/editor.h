#ifndef KODAPA_EDITOR_H
#define KODAPA_EDITOR_H

int editor_init();
void editor_cleanup();
int editor_get_input();
void editor_key_press(int ch);
void editor_resized();

#endif
