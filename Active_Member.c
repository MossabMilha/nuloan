
#include "Active_Member.h"





void Active_Member_Page(GtkWidget *widget, gpointer user_data){
    GtkWidget *main_window = GTK_WIDGET(user_data);
    const gint id = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(main_window), "id"));

    // Create a new Signup window
    GtkWidget *Home_Page = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(Home_Page), "Home Page");
    gtk_window_set_default_size(GTK_WINDOW(Home_Page), 350, 600);
    gtk_window_set_resizable(GTK_WINDOW(Home_Page), FALSE);
    g_object_set_data(G_OBJECT(Home_Page), "id", GINT_TO_POINTER(id));
    g_object_set_data(G_OBJECT(Home_Page), "id", GINT_TO_POINTER(id));

    User user;
    char path[100];
    snprintf(path, sizeof(path), "..\\DataBase\\Users\\user_%d.bin", id);
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        return;
    }
    fread(&user, sizeof(User), 1, file);
    fclose(file);

    // Create a fixed layout for the Bank Information window
    GtkWidget *fix = gtk_fixed_new();

    GtkWidget *image = gtk_image_new_from_file("..\\images\\profile.png");
    gtk_image_set_pixel_size(GTK_IMAGE(image), 110);
    gtk_fixed_put(GTK_FIXED(fix), image, 20, 25);

    GtkWidget *Full_Name = gtk_label_new("");
    char Full_Name_text[100];
    sprintf(Full_Name_text, "<span font='8' weight='bold' foreground='#000000'>  First Name : %s %s</span>", user.first_name, user.last_name);
    gtk_label_set_markup(GTK_LABEL(Full_Name), Full_Name_text);
    gtk_fixed_put(GTK_FIXED(fix), Full_Name, 130, 45);

    GtkWidget *ID = gtk_label_new("");
    char ID_text[100];
    sprintf(ID_text, "<span font='8' weight='bold' foreground='#000000'>  ID : %d</span>", user.user_id);
    gtk_label_set_markup(GTK_LABEL(ID), ID_text);
    gtk_fixed_put(GTK_FIXED(fix), ID, 130, 65);

    GtkWidget *CIN = gtk_label_new("");
    char CIN_text[100];
    sprintf(CIN_text, "<span font='8' weight='bold' foreground='#000000'>  CIN : %s</span>", user.cin);
    gtk_label_set_markup(GTK_LABEL(CIN), CIN_text);
    gtk_fixed_put(GTK_FIXED(fix), CIN, 130, 85);

    GtkWidget *Email = gtk_label_new("");
    char Email_text[100];
    sprintf(Email_text, "<span font='8' weight='bold' foreground='#000000'>  Email : %s</span>", user.email);
    gtk_label_set_markup(GTK_LABEL(Email), Email_text);
    gtk_fixed_put(GTK_FIXED(fix), Email, 130, 105);

    GtkWidget *Edit_Information = gtk_button_new_with_label("Edit Information");
    gtk_fixed_put(GTK_FIXED(fix), Edit_Information, 25, 180);
    gtk_widget_set_size_request(Edit_Information, 300, 20);

    GtkWidget *Apply_Loan = gtk_button_new_with_label("Apply For Loan");
    gtk_fixed_put(GTK_FIXED(fix), Apply_Loan, 25, 230);
    gtk_widget_set_size_request(Apply_Loan, 300, 20);

    GtkWidget *Check_Loan = gtk_button_new_with_label("Check Loan Status");
    gtk_fixed_put(GTK_FIXED(fix), Check_Loan, 25, 280);
    gtk_widget_set_size_request(Check_Loan, 300, 20);

    GtkWidget *Payments_history = gtk_button_new_with_label("Payments History");
    gtk_fixed_put(GTK_FIXED(fix), Payments_history, 25, 330);
    gtk_widget_set_size_request(Payments_history, 300, 20);

    GtkWidget *Loan_History = gtk_button_new_with_label("Loan Application History");
    gtk_fixed_put(GTK_FIXED(fix), Loan_History, 25, 380);
    gtk_widget_set_size_request(Loan_History, 300, 20);

    GtkWidget *Log_Out = gtk_button_new_with_label("Log Out");
    gtk_fixed_put(GTK_FIXED(fix), Log_Out, 25, 430);
    gtk_widget_set_size_request(Log_Out, 300, 20);

    g_object_set_data(G_OBJECT(Home_Page), "Page", GINT_TO_POINTER(1));
    g_signal_connect(Apply_Loan, "clicked", G_CALLBACK(Apply_Loan_UI), Home_Page);
    g_signal_connect(Check_Loan, "clicked", G_CALLBACK(Loan_Status_UI), Home_Page);
    g_signal_connect(Log_Out, "clicked", G_CALLBACK(First_Page_UI), Home_Page);




    gtk_window_set_child(GTK_WINDOW(Home_Page), fix);

    gtk_widget_set_visible(Home_Page, TRUE);
    gtk_widget_set_visible(main_window, FALSE);
}