#include <gtk/gtk.h>
#include "ui.h"

char* BRIGHTNESS_FILE="/sys/class/leds/asus::screenpad/brightness";

GtkBuilder *builder;
GObject *window;

GtkScale *scale;
GtkLabel *error_label;

GError *error = NULL;

/**
 * Map values 0-100 to 1-255
*/
int get_interpolated_value_from_slider(int value){
    return (int)(255.0/100*value)+1;
}

/**
 * Map values 0-100 to 1-255
*/
int get_interpolated_value_from_file(int value){
    return (int)(100.0/255*value);
}
int get_brightness_level(){
    
    FILE* file = fopen (BRIGHTNESS_FILE, "r");

    if (file == NULL) {
        show_file_open_error();
        return 0;
    }
    int i = 255;

    fscanf (file, "%d", &i);    
    fclose (file);    
    return i;
}

void set_brightness_level(int value){

    FILE * file;
    int i= value;
    file = fopen (BRIGHTNESS_FILE,"w");

    if (file == NULL) {
        show_file_open_error();
        return;
    }

    fprintf (file, "%d\n",i);  
    fclose (file);

}


void on_slider_value_changed(GtkRange *range, gpointer data){
    
    int a=gtk_range_get_value((GtkRange *)scale);
    int brightness=get_interpolated_value_from_slider(a);

    //g_message("Changed %d",brightness);

    set_brightness_level(brightness);
}

void show_file_open_error(){
    gtk_label_set_text(error_label,"Error oprning file /sys/class/leds/asus::screenpad/brightness");

}

void setup_ui(){
    gtk_range_set_range((GtkRange *)scale,0,100);
    gtk_range_set_increments ((GtkRange *)scale,1,1);

    gtk_label_set_text(error_label,"");

    int a=get_brightness_level();
    int brightness=get_interpolated_value_from_file(a);

    //g_message("Read %d",brightness);

    gtk_range_set_value((GtkRange *)scale,brightness);

}

int initUi(int argc, char *argv[]){

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    //Load ui description from built resource - need to generate compiled source with glib-compile-resource
    gtk_builder_add_from_resource(builder,"/com/lakinduakash/asus-screenpad-control/window.ui",&error);

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object(builder, "window");

   scale = (GtkScale *) gtk_builder_get_object(builder, "brightness_controle_scale");
   error_label = (GtkLabel *) gtk_builder_get_object(builder, "error_label");

   g_signal_connect(scale, "value-changed", G_CALLBACK(on_slider_value_changed), NULL);

    setup_ui();

    gtk_main();

    return 0;
}