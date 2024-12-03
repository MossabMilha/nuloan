#include <gtk/gtk.h>
#include <dirent.h>
#include <sys/stat.h>
#include "Show_Users.h"
#include "Structures/User.h"


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
    printf("---%s\n", user.first_name);
    printf("---%s\n", user.last_name);
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
void more(User user) {
    printf("User ID: %d\n", user.user_id);
}
void more_wrapper(GtkWidget *button, gpointer data) {
    User *user = (User *)data;
    more(*user); // Call your original `more` function with the parameter
}

void Create_User_Frame(GtkWidget *parent, const User user, const int x, const int y) {
    GtkWidget *frame = gtk_frame_new("======================");
    gtk_widget_set_size_request(frame, 25, 25);

    // Create an image widget for the role picture
    GtkWidget *role_image;
    if (strcmp(user.role, "super admin") == 0) {
        role_image = gtk_image_new_from_file("..\\images\\Super_Admin.png");
        gtk_widget_set_tooltip_text(role_image, "Super_Admin");
    } else if (strcmp(user.role, "admin") == 0) {
        role_image = gtk_image_new_from_file("..\\images\\Admin.png");
        gtk_widget_set_tooltip_text(role_image, "Admin");
    } else {
        if (strcmp(user.status, "active") == 0) {
            role_image = gtk_image_new_from_file("..\\images\\active_account.png");
            gtk_widget_set_tooltip_text(role_image, "Active Member");
        } else {
            role_image = gtk_image_new_from_file("..\\images\\innactive.png");
            gtk_widget_set_tooltip_text(role_image, "Inactive Member");
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
    printf("%s\n", user.last_name);
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
    User *user_ptr = malloc(sizeof(User));
    *user_ptr = user;
    g_signal_connect(more_button, "clicked", G_CALLBACK(more_wrapper), user_ptr);

    GtkWidget *edit_button = gtk_button_new_with_label("Edit");

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
    GtkWidget *Admin_Page = GTK_WIDGET(data);
    GtkWidget *Admin_Page_Fix = g_object_get_data(G_OBJECT(Admin_Page), "Admin_Page_Fix");
    GtkWidget *Admin_OutPut_Fix = g_object_get_data(G_OBJECT(Admin_Page), "Admin_OutPut_Fix");

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
    printf("%d\n",max);
    for (int i = 0; i < max; i++) {
        User user = Read_User_Information(i + 1 + (page - 1) * 10);
        int x = (i % 5) * 290 ;
        int y = (i / 5) * 250 + 100;
        Create_User_Frame(Admin_OutPut_Fix, user, x, y);
    }

    // Create navigation buttons
    GtkWidget *prev_button = gtk_button_new_with_label("Previous");
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), prev_button, 0, 600);
    g_signal_connect(prev_button, "clicked", G_CALLBACK(Prev_Page), Admin_Page);

    GtkWidget *next_button = gtk_button_new_with_label("Next");
    gtk_fixed_put(GTK_FIXED(Admin_OutPut_Fix), next_button, 1300, 600);
    g_signal_connect(next_button, "clicked", G_CALLBACK(Next_Page), Admin_Page);

    // Show or hide buttons based on the page number
    if (page == 1) {
        gtk_widget_set_visible(prev_button, FALSE);
    } else {
        gtk_widget_set_visible(prev_button, TRUE);
    }
    int total_files = count_files_in_directory("..\\Users");
    int total_pages = (total_files + 9) / 10; // Calculate total pages needed

    if (page >= total_pages) {
        gtk_widget_set_visible(next_button, FALSE);
    } else {
        gtk_widget_set_visible(next_button, TRUE);
    }

    // Show all widgets
    gtk_widget_set_visible(Admin_OutPut_Fix, TRUE);
}