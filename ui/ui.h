#ifndef UI_H
#define UI_H

#include <gtk/gtk.h>

int initUi(int argc, char *argv[]);
int get_interpolated_value_from_file(int value);
int get_interpolated_value_from_slider(int value);
int get_brightness_level();
void set_brightness_level(int value);
void on_slider_value_changed(GtkRange *range, gpointer data);
void show_file_open_error(void);
void setup_ui(void);

#endif //UI_H