//
// Created by PC on 03-Dec-24.
//

#include "Apply_Loan.h"

loan_type loan_info_1[6];
void read_loan_info_1() {
    for (int i = 0; i < 6; i++) {
        char path[100];
        snprintf(path, sizeof(path), "..\\DataBase\\Loans\\loan_%d.bin", i + 1);
        FILE *file = fopen(path, "rb");
        if (file == NULL) {
            fprintf(stderr, "Could not open loan_info.bin for reading\n");
            exit(1);
        }
        fread(&loan_info_1[i], sizeof(loan_type), 1, file);
        fclose(file);
    }
}

void Apply_Loan_UI(GtkWidget *widget, gpointer user_data) {
    read_loan_info_1();
    GtkWidget *main_window = GTK_WIDGET(user_data);
    const gint page = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "Page"));
    const gint id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "id"));

    // Create a new Signup window
    GtkWidget *Apply_Loan_window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Apply_Loan_window), "Apply To Loan");
    gtk_window_set_default_size(GTK_WINDOW(Apply_Loan_window), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(Apply_Loan_window), FALSE);

    g_object_set_data(G_OBJECT(Apply_Loan_window), "id", GINT_TO_POINTER(id));


    GtkWidget *Apply_Loan_fix = gtk_fixed_new();
    gtk_window_set_child(GTK_WINDOW(Apply_Loan_window), Apply_Loan_fix);

    GtkWidget *next_button = gtk_button_new_with_label("Next");
    gtk_widget_set_size_request(next_button, 30, 20);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), next_button, 270, 520);
    g_signal_connect(next_button, "clicked", G_CALLBACK(Apply_Loan_UI), Apply_Loan_window);

    GtkWidget *previous_button = gtk_button_new_with_label("Previous");
    gtk_widget_set_size_request(previous_button, 30, 20);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), previous_button, 25, 520);
    g_signal_connect(previous_button, "clicked", G_CALLBACK(Apply_Loan_UI), Apply_Loan_window);

    GtkWidget *back_button = gtk_button_new_with_label("Exit");
    gtk_widget_set_size_request(back_button, 30, 20);
    gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), back_button, 155, 520);
    g_signal_connect(back_button, "clicked", G_CALLBACK(Active_Member_Page), Apply_Loan_window);

    gtk_widget_set_visible(next_button, FALSE);
    gtk_widget_set_visible(previous_button, FALSE);


    if (page == 1) {
        gtk_widget_set_visible(next_button, TRUE);
        g_object_set_data(G_OBJECT(Apply_Loan_window), "Page", GINT_TO_POINTER(2));

        for (int i = 0; i < 4; i++) {
            int *tab = g_malloc(sizeof(int) * 2); // Allocate memory dynamically
            tab[0] = id;
            tab[1] = i+1;
            g_object_set_data(G_OBJECT(Apply_Loan_window), "Laon_Number", GINT_TO_POINTER(i));
            GtkWidget *frame = gtk_frame_new("====================================");
            gtk_widget_set_size_request(frame, 25, 25);

            GtkWidget *user_id = gtk_label_new("");
            char id_text[1000];
            sprintf(id_text, "<span font='8' weight='bold' foreground='#000000'>TITLE:</span> <span font='8' foreground='#000000'>%s\n%s</span>", loan_info_1[i].loan_title, loan_info_1[i].loan_one_line_description);

            gtk_label_set_markup(GTK_LABEL(user_id), id_text);
            gtk_label_set_xalign(GTK_LABEL(user_id), 0);
            GtkWidget *frame_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
            gtk_label_set_xalign(GTK_LABEL(user_id), 0.1);

            GtkWidget *more_button = gtk_button_new_with_label("More Information");
            gtk_widget_set_size_request(more_button, 290, 20);
            gtk_widget_set_margin_bottom(more_button, 5);
            gtk_widget_set_margin_start(more_button, 5);
            gtk_widget_set_margin_end(more_button, 5);
            g_signal_connect(more_button, "clicked", G_CALLBACK(Apply_Loan_Page_2_UI), tab);


            gtk_box_append(GTK_BOX(frame_box), user_id);
            gtk_box_append(GTK_BOX(frame_box), more_button);
            gtk_frame_set_child(GTK_FRAME(frame), frame_box);

            gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), frame, 25, 20 + 125 * i);
        }
    } else {

        g_object_set_data(G_OBJECT(Apply_Loan_window), "Page", GINT_TO_POINTER(1));
        gtk_widget_set_visible(next_button, FALSE);
        gtk_widget_set_visible(previous_button, TRUE);
        for (int i = 4; i < 6; i++) {
            int *tab = g_malloc(sizeof(int) * 2); // Allocate memory dynamically
            tab[0] = id;
            tab[1] = i+1;
            GtkWidget *frame = gtk_frame_new("====================================");
            gtk_widget_set_size_request(frame, 25, 25);

            GtkWidget *user_id = gtk_label_new("");
            char id_text[1000];
            sprintf(id_text, "<span font='8' weight='bold' foreground='#000000'>TITLE:</span> <span font='8' foreground='#000000'>%s\n%s</span>", loan_info_1[i].loan_title, loan_info_1[i].loan_one_line_description);

            gtk_label_set_markup(GTK_LABEL(user_id), id_text);
            gtk_label_set_xalign(GTK_LABEL(user_id), 0);
            GtkWidget *frame_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
            gtk_label_set_xalign(GTK_LABEL(user_id), 0.1);

            GtkWidget *more_button = gtk_button_new_with_label("More");
            gtk_widget_set_size_request(more_button, 290, 20);
            gtk_widget_set_margin_bottom(more_button, 5);
            gtk_widget_set_margin_start(more_button, 5);
            gtk_widget_set_margin_end(more_button, 5);
            g_signal_connect(more_button, "clicked", G_CALLBACK(Apply_Loan_Page_2_UI), tab);

            gtk_box_append(GTK_BOX(frame_box), user_id);
            gtk_box_append(GTK_BOX(frame_box), more_button);

            gtk_frame_set_child(GTK_FRAME(frame), frame_box);

            gtk_fixed_put(GTK_FIXED(Apply_Loan_fix), frame, 25, 20 + 125 * (i - 4));
        }
    }

    if (page == 1) {
        gtk_widget_set_visible(previous_button, FALSE);
    } else {
        gtk_widget_set_visible(previous_button, TRUE);
    }

    if (page == 2) {
        gtk_widget_set_visible(next_button, FALSE);
    } else {
        gtk_widget_set_visible(next_button, TRUE);
    }

    gtk_widget_set_visible(Apply_Loan_window, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}