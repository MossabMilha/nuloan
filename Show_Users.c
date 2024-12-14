#include <gtk/gtk.h>
#include <dirent.h>
#include <sys/stat.h>
#include "Show_Users.h"
#include "Structures/User.h"
#include "Super_Admin.h"


int count_files_in_directory(const char *directory_path) {
    int file_count = 0;
    struct dirent *entry;
    DIR *dp = opendir(directory_path);

    if (dp == NULL) {
        return 0;
    }

    while ((entry = readdir(dp))) {
        struct stat path_stat;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", directory_path, entry->d_name);
        stat(full_path, &path_stat);
        if (S_ISREG(path_stat.st_mode)) {
            file_count++;
        }
    }

    closedir(dp);
    return file_count;
}
User Read_User_Information(int id) {
    char filename[100];
    sprintf(filename, "..\\DataBase\\Users\\user_%d.bin", id);
    FILE *file = fopen(filename, "rb");
    User user;
    fread(&user, sizeof(User), 1, file);
    fclose(file);
    return user;
}

void Prev_Page(GtkWidget *widget, gpointer data) {
    int page = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(data), "Page"));
    if (page > 1) {
        page--;
        g_object_set_data(G_OBJECT(data), "Page", GINT_TO_POINTER(page));
        Check_User(widget, data);
    }
}

void Next_Page(GtkWidget *widget, gpointer data) {
    int page = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(data), "Page"));
    page++;
    g_object_set_data(G_OBJECT(data), "Page", GINT_TO_POINTER(page));
    Check_User(widget, data);
}

void update_user_status(int user_id, const char *new_status) {
    char filename[100];
    sprintf(filename, "..\\DataBase\\Users\\user_%d.bin", user_id);

    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file for user ID %d.\n", user_id);
        return;
    }

    User user;
    if (fread(&user, sizeof(User), 1, file) != 1) {
        fprintf(stderr, "Error: Could not read user data for user ID %d.\n", user_id);
        fclose(file);
        return;
    }

    // Update the status
    strncpy(user.status, new_status, sizeof(user.status) - 1);
    user.status[sizeof(user.status) - 1] = '\0';
    user.updated_at = time(NULL); // Update the modification timestamp

    // Write the updated data back to the file
    fseek(file, 0, SEEK_SET);
    if (fwrite(&user, sizeof(User), 1, file) != 1) {
        fprintf(stderr, "Error: Could not write updated user data for user ID %d.\n", user_id);
    }

    fclose(file);
}

void deactivate_account(GtkButton *button, gpointer user_data) {
    int id = GPOINTER_TO_INT(user_data);
    update_user_status(id, "inactive");
    gtk_button_set_label(button, "Done");
    g_signal_handlers_disconnect_by_func(button, deactivate_account, user_data);

}

void activate_account(GtkButton *button, gpointer user_data) {
    int id = GPOINTER_TO_INT(user_data);
    update_user_status(id, "Active");
    gtk_button_set_label(button, "Done");
    g_signal_handlers_disconnect_by_func(button, activate_account, user_data);
}
void Create_User_Frame(GtkWidget *parent, const User user, const int x, const int y) {
    GtkWidget *frame = gtk_frame_new("======================");
    gtk_widget_set_size_request(frame, 25, 25);

    // Create an image widget for the role picture
    GtkWidget *role_image;
    GtkWidget *edit_button = gtk_button_new_with_label("");
    if (strcmp(user.role, "super admin") == 0) {
        role_image = gtk_image_new_from_file("..\\images\\Super_Admin.png");
        gtk_widget_set_tooltip_text(role_image, "Super_Admin");
        gtk_widget_set_visible(edit_button, FALSE);
    } else if (strcmp(user.role, "admin") == 0) {
        role_image = gtk_image_new_from_file("..\\images\\Admin.png");
        gtk_widget_set_tooltip_text(role_image, "Admin");
        gtk_widget_set_visible(edit_button, FALSE);
    } else {
        if (strcmp(user.status, "Active") == 0) {
            role_image = gtk_image_new_from_file("..\\images\\active_account.png");
            gtk_widget_set_tooltip_text(role_image, "Active Member");
            gtk_button_set_label(GTK_BUTTON(edit_button), "Deactivate");
            g_signal_connect(edit_button, "clicked", G_CALLBACK(deactivate_account), GINT_TO_POINTER(user.user_id));

        } else {
            role_image = gtk_image_new_from_file("..\\images\\innactive.png");
            gtk_widget_set_tooltip_text(role_image, "Inactive Member");
            gtk_button_set_label(GTK_BUTTON(edit_button), "Activate");

            g_signal_connect(edit_button, "clicked", G_CALLBACK(activate_account), GINT_TO_POINTER(user.user_id));

        }
    }
    gtk_widget_set_size_request(role_image, 20, 20); // Enforce image size
    gtk_widget_set_halign(role_image, GTK_ALIGN_END);
    gtk_widget_set_valign(role_image, GTK_ALIGN_START);

    GtkWidget *user_id = gtk_label_new("");
    char id_text[1000];
    sprintf(id_text, "<span font='8' weight='bold' foreground='#000000'>  ID : %d</span>", user.user_id);
    gtk_label_set_markup(GTK_LABEL(user_id), id_text);
    gtk_label_set_xalign(GTK_LABEL(user_id), 0);

    GtkWidget *user_first_name = gtk_label_new("");
    char first_name_text[100];
    sprintf(first_name_text, "<span font='8' weight='bold' foreground='#000000'>  First Name : %s</span>", user.first_name);
    gtk_label_set_markup(GTK_LABEL(user_first_name), first_name_text);
    gtk_label_set_xalign(GTK_LABEL(user_first_name), 0);

    GtkWidget *user_last_name = gtk_label_new("");

    char last_name_text[100];
    sprintf(last_name_text, "<span font='8' weight='bold' foreground='#000000'>  Last Name : %s</span>", user.last_name);
    gtk_label_set_markup(GTK_LABEL(user_last_name), last_name_text);
    gtk_label_set_xalign(GTK_LABEL(user_last_name), 0);

    GtkWidget *user_email = gtk_label_new("");
    char email_text[100];
    sprintf(email_text, "<span font='8' weight='bold' foreground='#000000'>  Email : %s </span>", user.email);
    gtk_label_set_markup(GTK_LABEL(user_email), email_text);
    gtk_label_set_xalign(GTK_LABEL(user_email), 0);

    GtkWidget *more_button = gtk_button_new_with_label("More");
    // g_signal_connect(more_button, "clicked", G_CALLBACK(more_wrapper), user_ptr);


    // Create a vertical box to hold all widgets
    GtkWidget *frame_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_append(GTK_BOX(frame_box), role_image);       // Add image directly
    gtk_box_append(GTK_BOX(frame_box), user_id);         // Add user ID label
    gtk_box_append(GTK_BOX(frame_box), user_first_name); // Add first name label
    gtk_box_append(GTK_BOX(frame_box), user_last_name);  // Add last name label
    gtk_box_append(GTK_BOX(frame_box), user_email);      // Add email label
    gtk_box_append(GTK_BOX(frame_box), more_button);     // Add 'More' button
    gtk_box_append(GTK_BOX(frame_box), edit_button);     // Add 'Edit' button

    // Set the box as the child of the frame
    gtk_frame_set_child(GTK_FRAME(frame), frame_box);
    gtk_fixed_put(GTK_FIXED(parent), frame, x, y);
}



void Check_User(GtkWidget *widget, gpointer data) {
    int page = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(data), "Page"));
    GtkWidget *Admin_Page_1 = GTK_WIDGET(data);
    GtkWidget *Admin_Page_Fix = g_object_get_data(G_OBJECT(Admin_Page_1), "Admin_Page_Fix");
    GtkWidget *Admin_OutPut_Fix = g_object_get_data(G_OBJECT(Admin_Page_1), "Admin_OutPut_Fix");

    GtkWidget *Exit = gtk_button_new_with_label("Exit");
    gtk_fixed_put(GTK_FIXED(Admin_Page_Fix), Exit, 75, 400);
    gtk_widget_set_size_request(Exit, 250, 20);
    gtk_widget_set_visible(Exit, TRUE);
    g_signal_connect(Exit, "clicked", G_CALLBACK(Admin_Page),  data);



    // Clear previous content
    GtkWidget *child = gtk_widget_get_first_child(GTK_WIDGET(Admin_OutPut_Fix));
    while (child != NULL) {
        GtkWidget *next_child = gtk_widget_get_next_sibling(child);
        gtk_widget_unparent(child);
        child = next_child;
    }

    // Create Page Title
    GtkWidget *Title = gtk_label_new("User_Information");
    gtk_label_set_markup(GTK_LABEL(Title), "<span font='20' weight='bold' foreground='#000000'>User Information</span>");
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), Title, 40, 50);



    // Create fixed layout for users
    GtkWidget *grid = gtk_grid_new();
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), grid, 10, 100);
    int max=1;
     if(page * 10 > count_files_in_directory("..\\DataBase\\Users")) {
         max = count_files_in_directory("..\\DataBase\\Users") % 10;
     }else {
         max = 10;
     }

    for (int i = 0; i < max; i++) {
        User user = Read_User_Information(i + 1 + (page - 1) * 10);
        int x = (i % 5) * 290 ;
        int y = (i / 5) * 250 + 100;
        Create_User_Frame(Admin_OutPut_Fix, user, x, y);
    }

    // Create navigation buttons
    GtkWidget *prev_button = gtk_button_new_with_label("Previous");
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), prev_button, 0, 600);
    g_signal_connect(prev_button, "clicked", G_CALLBACK(Prev_Page), Admin_Page_1);

    GtkWidget *next_button = gtk_button_new_with_label("Next");
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), next_button, 1300, 600);
    g_signal_connect(next_button, "clicked", G_CALLBACK(Next_Page), Admin_Page_1);

    // Show or hide buttons based on the page number
    if (page == 1) {
        gtk_widget_set_visible(prev_button, FALSE);
    } else {
        gtk_widget_set_visible(prev_button, TRUE);
    }
    int total_files = count_files_in_directory("..\\DataBase\\Users");
    int total_pages = (total_files + 9) / 10; // Calculate total pages needed

    if (page >= total_pages) {
        gtk_widget_set_visible(next_button, FALSE);
    } else {
        gtk_widget_set_visible(next_button, TRUE);
    }

    // Show all widgets
    gtk_widget_set_visible(Admin_OutPut_Fix, TRUE);
}