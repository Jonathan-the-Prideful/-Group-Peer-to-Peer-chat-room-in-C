include <gtk/gtk.h>

int main (int argc, char **argv){

    //So this is used to call visual and color maping and initalizes the gtx library.
    gtk_init (&argc, &argv);

    //window library extends to window
    GtkWidget *window;

    //Shows 
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show_all(window);

    //close window corectly ending program.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main ();// loops the function

    return 0;
}
