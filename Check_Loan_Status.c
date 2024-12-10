//
// Created by PC on 03-Dec-24.
//

#include "Check_Loan_Status.h"
#include "Active_Member.h"
void Loan_Status_UI(GtkWidget *widget, gpointer user_data) {
    GtkWidget *main_window = GTK_WIDGET(user_data);

    // Create a new Signup window
    GtkWidget *Loan_Status_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Loan_Status_window), "Loan Satus");
    gtk_window_set_default_size(GTK_WINDOW(Loan_Status_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(Loan_Status_window), FALSE);

    GtkWidget *Loan_Status_fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(Loan_Status_window), Loan_Status_fix);
    for (int i = 1; i <5; i++) {

        GtkWidget *frame = gtk_frame_new("====================================");
        gtk_widget_set_size_request(frame, 25, 25);

        GtkWidget *user_id = gtk_label_new("");
        char id_text[1000];
        sprintf(id_text, "<span font='8' weight='bold' foreground='#000000'>ID LOAN : %d \nYOUR LOAN IS DEClINED \n</span>", i);
        gtk_label_set_markup(GTK_LABEL(user_id), id_text);
        gtk_label_set_xalign(GTK_LABEL(user_id), 0);
        GtkWidget *frame_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
        gtk_label_set_xalign(GTK_LABEL(user_id), 0.1);

        GtkWidget *more_button = gtk_button_new_with_label("More");
        gtk_widget_set_size_request(more_button, 290, 20);
        gtk_widget_set_margin_bottom(more_button, 5);
        gtk_widget_set_margin_start(more_button, 5);
        gtk_widget_set_margin_end(more_button, 5);



        gtk_box_append(GTK_BOX(frame_box), user_id);
        gtk_box_append(GTK_BOX(frame_box), more_button);

        gtk_frame_set_child(GTK_FRAME(frame), frame_box);

        gtk_fixed_put(GTK_FIXED(Loan_Status_fix), frame, 25, 20+125*(i-1));
    }

    GtkWidget *next_button = gtk_button_new_with_label("Next");
    gtk_widget_set_size_request(next_button, 30, 20);
    gtk_fixed_put(GTK_FIXED(Loan_Status_fix), next_button, 270, 520);

    GtkWidget *Previous_button = gtk_button_new_with_label("Previous");
    gtk_widget_set_size_request(Previous_button, 30, 20);
    gtk_fixed_put(GTK_FIXED(Loan_Status_fix), Previous_button, 25, 520);

    GtkWidget *back_button = gtk_button_new_with_label("Exit");
    gtk_widget_set_size_request(back_button, 30, 20);
    gtk_fixed_put(GTK_FIXED(Loan_Status_fix), back_button, 155, 520);
    g_signal_connect(back_button, "clicked", G_CALLBACK(Active_Member_Page), Loan_Status_window);




    gtk_widget_set_visible(Loan_Status_window, TRUE);
    gtk_widget_set_visible(main_window, FALSE);


}