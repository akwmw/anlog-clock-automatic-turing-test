#include <gtk/gtk.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>

// Define the clock structure
typedef struct {
    GtkWidget *window;
    GtkWidget *clock_area;
    GtkWidget *entry;
    GtkWidget *submit_button;
    GtkWidget *restart_button;
    GtkWidget *answer_button;
    gint hour;
    gint minute;
    gint second;
} Clock;

// Function to draw the clock
gboolean draw_clock(GtkWidget *widget, cairo_t *cr, gpointer data) {
    //printf("Drawing clock\n");
    Clock *clock = (Clock *)data;
    GtkWidget *clock_area = clock->clock_area;
    gint width = gtk_widget_get_allocated_width(clock_area);
    gint height = gtk_widget_get_allocated_height(clock_area);
    gdouble center_x = width / 2.0;
    gdouble center_y = height / 2.0;
    gdouble radius = MIN(width, height) / 2.0 - 10;

    // Clear the drawing area
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Draw the clock circle
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2);
    cairo_arc(cr, center_x, center_y, radius, 0, 2 * M_PI);
    cairo_stroke(cr);

    // Draw the graduations and lines
    cairo_set_line_width(cr, 1);
    for (gint i = 0; i < 60; i++) {
        gdouble angle = i * M_PI / 30.0;
        gdouble x1 = center_x + radius * 0.9 * sin(angle);
        gdouble y1 = center_y - radius * 0.9 * cos(angle);
        gdouble x2 = center_x + radius * 0.95 * sin(angle);
        gdouble y2 = center_y - radius * 0.95 * cos(angle);

        if (i % 5 == 0) {
            // Draw a longer and bolder line for every 5 minutes
            cairo_set_line_width(cr, 2);
            cairo_move_to(cr, x1, y1);
            cairo_line_to(cr, x2, y2);
        } else {
            // Draw a shorter line for other minutes
            cairo_set_line_width(cr, 1);
            cairo_move_to(cr, x1, y1);
            cairo_line_to(cr, x2, y2);
        }
        cairo_stroke(cr);
    }

    // Draw the numbers 1-12 on the clock face
    gchar *numbers[] = {"12", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"};
    for (gint i = 0; i < 12; i++) {
        gdouble angle = i * M_PI / 6.0;
        gdouble x = center_x + radius * 0.8 * sin(angle);
        gdouble y = center_y - radius * 0.8 * cos(angle);
        cairo_move_to(cr, x, y);
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_font_size(cr, 24);
        cairo_show_text(cr, numbers[i]);
    }

    // Draw the hour hand
    gint hour = clock->hour % 12; // Convert to 12-hour format
    if (hour == 0) hour = 12; // Handle 12:00
    gdouble hour_angle = (hour + clock->minute / 60.0) * M_PI / 6.0;
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 5);
    cairo_move_to(cr, center_x, center_y);
    cairo_line_to(cr, center_x + radius * 0.5 * sin(hour_angle), center_y - radius * 0.5 * cos(hour_angle));
    cairo_stroke(cr);

    // Draw the minute hand
    gdouble minute_angle = clock->minute * M_PI / 30.0;
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 3);
    cairo_move_to(cr, center_x, center_y);
    cairo_line_to(cr, center_x + radius * 0.7 * sin(minute_angle), center_y - radius * 0.7 * cos(minute_angle));
    cairo_stroke(cr);

    // Draw the second hand
    gdouble second_angle = clock->second * M_PI / 30.0;
    cairo_set_source_rgb(cr, 1, 0, 0);
    cairo_set_line_width(cr, 1);
    cairo_move_to(cr, center_x, center_y);
    cairo_line_to(cr, center_x + radius * 0.9 * sin(second_angle), center_y - radius * 0.9 * cos(second_angle));
    cairo_stroke(cr);

    return TRUE;
}

// Function to handle the submit button click
void submit_button_clicked(GtkButton *button, gpointer data) {
    Clock *clock = (Clock *)data;
    const gchar *input_text = gtk_entry_get_text(GTK_ENTRY(clock->entry));

    // Check if the input follows the "HH:MM" format
    if (strlen(input_text) != 5 || input_text[2] != ':' ||
        !isdigit(input_text[0]) || !isdigit(input_text[1]) || 
        !isdigit(input_text[3]) || !isdigit(input_text[4])) {
        
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(clock->window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Incorrect format! Please enter the time in HH:MM format.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    // Extract hour and minute from the input
    gint input_hour = (input_text[0] - '0') * 10 + (input_text[1] - '0');
    gint input_minute = (input_text[3] - '0') * 10 + (input_text[4] - '0');

    // Convert the clock's hour to 12-hour format if needed
    gint clock_hour = clock->hour % 12;
    if (clock_hour == 0) {
        clock_hour = 12;
    }

    // Now check if the input matches the clock's time
    if (input_hour == clock_hour && input_minute == clock->minute) {
        // Correct input
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(clock->window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_INFO,
                                                   GTK_BUTTONS_OK,
                                                   "Correct! The time is %02d:%02d.",
                                                   clock_hour, clock->minute);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        // Incorrect input
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(clock->window),
                                                   GTK_DIALOG_MODAL,
                                                   GTK_MESSAGE_ERROR,
                                                   GTK_BUTTONS_OK,
                                                   "Incorrect! Try again.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// Function to handle the answer button click
void answer_button_clicked(GtkButton *button, gpointer data) {
    printf("Answer button clicked\n");
    Clock *clock = (Clock *)data;
    g_print("The correct answer is: %02d:%02d:%02d\n", clock->hour, clock->minute, clock->second);
    GtkWidget *label = gtk_label_new(NULL);
    gtk_label_set_text(GTK_LABEL(label), g_strdup_printf("The correct answer is: %02d:%02d", clock->hour, clock->minute));
    gtk_box_pack_start(GTK_BOX(gtk_widget_get_parent(clock->restart_button)), label, FALSE, FALSE, 0);
    gtk_widget_show_all(gtk_widget_get_parent(clock->restart_button));
}

// Function to handle the restart button click
void restart_button_clicked(GtkButton *button, gpointer data) {
    printf("Restart button clicked\n");
    Clock *clock = (Clock *)data;
    GtkWidget *parent = gtk_widget_get_parent(clock->restart_button);
    
    // Remove any labels added after a previous answer
    GList *children = gtk_container_get_children(GTK_CONTAINER(parent));
    for (GList *child = g_list_last(children); child; child = g_list_previous(child)) {
        GtkWidget *widget = GTK_WIDGET(child->data);
        if (GTK_IS_LABEL(widget) && widget != clock->entry) {
            gtk_widget_destroy(widget);
        }
    }
    g_list_free(children);

    GtkWidget *parent_answer = gtk_widget_get_parent(clock->answer_button);
    GList *children_answer = gtk_container_get_children(GTK_CONTAINER(parent_answer));
    for (GList *child_answer = g_list_last(children_answer); child_answer; child_answer = g_list_previous(child_answer)) {
        GtkWidget *widget_answer = GTK_WIDGET(child_answer->data);
        if (GTK_IS_LABEL(widget_answer) && widget_answer != clock->entry && widget_answer != clock->answer_button) {
            gtk_widget_destroy(widget_answer);
        }
    }
    g_list_free(children_answer);
    
    // Clear the entry field
    gtk_entry_set_text(GTK_ENTRY(clock->entry), "");

    // Randomly generate a new time
    srand(time(NULL));
    do {
        clock->hour = rand() % 12 + 1;
        do {
            clock->minute = rand() % 60;
        } while (clock->minute % 5 == 0);  // Reject multiples of 5

        do {
            clock->second = rand() % 60;
        } while (clock->second % 5 == 0);  // Reject multiples of 5

    } while (clock->hour == 10 && clock->minute <= 14); // Reject 10:00:00 to 10:14:59
    
    // Redraw the clock with the new time
    gtk_widget_queue_draw(clock->clock_area);
}

gboolean window_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
    // Exit the GTK main loop
    gtk_main_quit();
    return TRUE;  
}

int main(int argc, char *argv[]) {
    printf("Program started\n");
    
    gtk_init(&argc, &argv);

    srand(time(NULL));

    Clock clock;
    do {
        clock.hour = rand() % 12 + 1;
        do {
            clock.minute = rand() % 60;
        } while (clock.minute % 5 == 0);  // Reject multiples of 5

        do {
            clock.second = rand() % 60;
        } while (clock.second % 5 == 0);  // Reject multiples of 5

    } while (clock.hour == 10 && clock.minute <= 14); // Reject 10:00:00 to 10:14:59
    // Random hour between 1 and 12
    //clock.hour = rand() % 12 + 1;
    //clock.minute = rand() % 60;
    //clock.second = rand() % 60;
    //debug print
    //printf("Generated time: %02d:%02d:%02d\n", clock->hour, clock->minute, clock->second);

    clock.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(clock.window), "Analog Clock turing test");
    gtk_window_set_default_size(GTK_WINDOW(clock.window), 400, 400);
    g_signal_connect(clock.window, "delete-event", G_CALLBACK(window_delete_event), NULL);

    clock.clock_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(clock.clock_area, 400, 400);
    g_signal_connect(clock.clock_area, "draw", G_CALLBACK(draw_clock), &clock);

    clock.entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(clock.entry), "Input time in the format \"HH:MM\"");

    clock.submit_button = gtk_button_new_with_label("Answer");
    g_signal_connect(clock.submit_button, "clicked", G_CALLBACK(submit_button_clicked), &clock);

    clock.answer_button = gtk_button_new_with_label("I give up show me the Answer");
    g_signal_connect(clock.answer_button, "clicked", G_CALLBACK(answer_button_clicked), &clock);

    clock.restart_button = gtk_button_new_with_label("Restart");
    g_signal_connect(clock.restart_button, "clicked", G_CALLBACK(restart_button_clicked), &clock);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(box), clock.clock_area, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), clock.entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), clock.submit_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), clock.answer_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), clock.restart_button, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(clock.window), box);

    gtk_widget_show_all(clock.window);

    gtk_main();

    return 0;
}
