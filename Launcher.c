#include <gtk/gtk.h>
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500

int main (int argc, char **argv){

    //So this is used to call visual and color maping and initalizes the gtx library.
    gtk_init (&argc, &argv);

    //window library extends to window
    GtkWidget *window;

    //Displays the window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);// Main window
    
    gtk_window_set_title(GTK_WINDOW(window), "The Awsome Chat Room!");// set title of application
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);// set size in pixles
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);//center window
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);// prevent user from resizing window

    gtk_widget_show_all(window);

    //close window corectly ending program.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main ();// loops the function

    return 0;
}
