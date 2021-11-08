#include <gtk/gtk.h>
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 500

void inizilize (GtkWidget *window){
    GtkWidget *buttonServer;
    GtkWidget *buttonClient;
    GtkWidget *box;
    GtkWidget *instructions;

    instructions = gtk_label_new("What would you like to do?");

    //setup box to contain items
    box = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), box);//add box to window

    //Add buttons
    buttonServer = gtk_button_new_with_label("Host Chat");
    gtk_widget_set_size_request(GTK_WIDGET(buttonServer), 200, 100);
    buttonClient = gtk_button_new_with_label("Join Chat");
    gtk_widget_set_size_request(GTK_WIDGET(buttonClient), 200, 100);
    gtk_fixed_put(GTK_FIXED(box), buttonServer, 100, 250);
    gtk_fixed_put(GTK_FIXED(box), buttonClient, 400, 250);
    gtk_fixed_put(GTK_FIXED(box), instructions, (WINDOW_WIDTH / 2) - 80, 50);

}

int main (int argc, char **argv){

    //So this is used to call visual and color maping and initalizes the gtx library.
    gtk_init (&argc, &argv);

    //widget library extends to window
    GtkWidget *window;

    //Displays the window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);// Main window
    
    gtk_window_set_title(GTK_WINDOW(window), "The Awsome Chat Room!");// set title of application
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);// set size in pixles
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);//center window
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);// prevent user from resizing window

    inizilize(window);// pass the window to inizlize

    gtk_widget_show_all(window);

    //close window corectly ending program.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main ();// loops the function

    return 0;
}
