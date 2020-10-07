#include <gtk/gtk.h>
#include "ui.h"

char* BRIGHTNESS_FILE="/sys/class/leds/asus::screenpad/brightness";

GtkBuilder *builder;
GObject *window;

GtkScale *scale;
GtkSwitch *switch_turn_off;
GtkLabel *error_label;

GError *error = NULL;

int last_read_range_value = -1;

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

/**
 * Get brigtness level from file. Returns 0-255
*/
int get_brightness_level(){
    
    FILE* file = fopen (BRIGHTNESS_FILE, "r");

    if (file == NULL) {
        show_file_open_error();
        return 0;
    }
    int i = 255;

    fscanf (file, "%d", &i);    
    fclose (file);
    g_message("Read file %d",i);  
    return i;
}

/**
 * Set brigtness level to file. Value between 0-255
*/
int set_brightness_level(int value){

    FILE * file;
    int i= value;
    file = fopen (BRIGHTNESS_FILE,"w");

    if (file == NULL) {
        show_file_open_error();
        return -1;
    }

    fprintf (file, "%d\n",i);  
    fclose (file);
    return 0;
}

/**
 * Called when slider value changed
 */
void on_slider_value_changed(GtkRange *range, gpointer data){
    
    int a=gtk_range_get_value((GtkRange *)scale);
    int brightness=get_interpolated_value_from_slider(a);

    //g_message("Changed %d",brightness);

    set_brightness_level(brightness);
}


void on_turn_off_switch_changed(GtkSwitch *gtk_switch,gboolean state,gpointer data){
    
    if(state){

        last_read_range_value=gtk_range_get_value((GtkRange *)scale);

        int success = set_brightness_level(0);

        if(success==0){
            gtk_widget_set_sensitive ((GtkWidget*)scale, FALSE);
        }

    }
    else{

        gtk_widget_set_sensitive ((GtkWidget*)scale, FALSE);

        if(last_read_range_value !=-1 && last_read_range_value !=0){
            
            int success=set_brightness_level(get_interpolated_value_from_slider(last_read_range_value));

            if(success==0){
                gtk_range_set_value((GtkRange *)scale,last_read_range_value);
                gtk_widget_set_sensitive ((GtkWidget*)scale, TRUE);
            }

        }
        else{

            //Set half brightness
            int success= set_brightness_level(128);

            if(success==0){
                gtk_range_set_value((GtkRange *)scale,get_interpolated_value_from_file(128));
                gtk_widget_set_sensitive ((GtkWidget*)scale, TRUE);
            }

        }
    }
}

void show_file_open_error(){
    gtk_label_set_text(error_label,"Error oprning file /sys/class/leds/asus::screenpad/brightness");
}

int initUi(int argc, char *argv[]){

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    //Load ui description from built resource - need to generate compiled source with glib-compile-resource
    gtk_builder_add_from_resource(builder,"/com/lakinduakash/asus-screenpad-control/window.ui",&error);

    /* Connect signal handlers to the constructed widgets. */
    window = gtk_builder_get_object(builder, "window");

    scale = (GtkScale *) gtk_builder_get_object(builder, "brightness_controle_scale");
    switch_turn_off= (GtkSwitch *) gtk_builder_get_object(builder, "switch_turn_off_screenpad");
    error_label = (GtkLabel *) gtk_builder_get_object(builder, "error_label");

    //Make sure to call before connecting signals
    setup_ui();

    g_signal_connect (window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(scale, "value-changed", G_CALLBACK(on_slider_value_changed), NULL);
    g_signal_connect(switch_turn_off, "state-set", G_CALLBACK(on_turn_off_switch_changed), NULL);
   

    gtk_main();

    return 0;
}

void setup_ui(){
    gtk_range_set_range((GtkRange *)scale,0,100);
    gtk_range_set_increments ((GtkRange *)scale,1,1);

    gtk_label_set_text(error_label,"");

    int a=get_brightness_level();

    //g_message("Read %d",a);

    if(a==0){
        gtk_switch_set_active(switch_turn_off,TRUE);
        gtk_widget_set_sensitive ((GtkWidget*)scale, FALSE);
    }
    else{
        int brightness=get_interpolated_value_from_file(a);
        gtk_range_set_value((GtkRange *)scale,brightness);
    }

}