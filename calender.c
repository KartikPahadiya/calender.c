#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent an event
struct Event {
    int day;
    int month;
    int year;
    char description[100];
};

// Function to get the day of the week for a given date
int getDayOfWeek(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }
    int k = year % 100;
    int j = year / 100;
    int dayOfWeek = (day + 13 * (month + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;
    return dayOfWeek;
}

// Function to display the monthly calendar
void displayCalendar(int month, int year, struct Event *events, int numEvents) {
    printf("Calendar for %d/%d\n", month, year);

    // Days in each month
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Names of the months
    char *monthNames[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

    // Print the header
    printf(" Sun Mon Tue Wed Thu Fri Sat\n");

    // Get the day of the week for the first day of the month
    int dayOfWeek = getDayOfWeek(1, month, year);

    // Print leading spaces
    for (int i = 0; i < dayOfWeek; i++) {
        printf("    ");
    }

    // Print the days of the month
    for (int day = 1; day <= daysInMonth[month]; day++) {
        // Print events for the current day
        printf("%4d", day);
        for (int i = 0; i < numEvents; i++) {
            if (events[i].day == day && events[i].month == month && events[i].year == year) {
                printf("*"); // Indicates an event on this day
                break;
            }
        }

        // Move to the next line after Saturday
        if ((dayOfWeek + day) % 7 == 0) {
            printf("\n");
        }
    }

    // Print a newline character if necessary
    if ((dayOfWeek + daysInMonth[month]) % 7 != 0) {
        printf("\n");
    }
}
// Function to compare events for qsort
int compareEvents(const void *a, const void *b) {
    struct Event *eventA = (struct Event *)a;
    struct Event *eventB = (struct Event *)b;
    
    // Compare years
    if (eventA->year != eventB->year) {
        return eventA->year - eventB->year;
    }
    
    // Compare months
    if (eventA->month != eventB->month) {
        return eventA->month - eventB->month;
    }
    
    // Compare days
    return eventA->day - eventB->day;
}

void addEvent(struct Event *events, int *numEvents) {
    // Implement event addition logic here
    if (*numEvents < 100) {
        printf("Enter event details (DD MM YYYY Description): ");
        scanf("%d %d %d %[^\n]", &events[*numEvents].day, &events[*numEvents].month, &events[*numEvents].year, events[*numEvents].description);
        (*numEvents)++;
        
        // Sort events array after adding a new event
        qsort(events, *numEvents, sizeof(struct Event), compareEvents);
        
        printf("Event added successfully.\n");
    } else {
        printf("Cannot add more events. Calendar is full.\n");
    }
}




// Function to delete an event
void deleteEvent(struct Event *events, int *numEvents, int day, int month, int year) {
    // Implement event deletion logic here
    for (int i = 0; i < *numEvents; i++) {
        if (events[i].day == day && events[i].month == month && events[i].year == year) {
            // Move subsequent events to fill the gap
            for (int j = i; j < *numEvents - 1; j++) {
                events[j] = events[j + 1];
            }
            (*numEvents)--;
            printf("Event deleted successfully.\n");
            return;
        }
    }
    printf("Event not found.\n");
}

void remindEvents(struct Event *events, int numEvents, int currentDay, int currentMonth, int currentYear) {
    // Implement reminder logic here
    printf("Upcoming Events:\n");
    for (int i = 0; i < numEvents; i++) {
        if (events[i].year > currentYear ||
            (events[i].year == currentYear && events[i].month > currentMonth) ||
            (events[i].year == currentYear && events[i].month == currentMonth && events[i].day >= currentDay)) {
            // Print only upcoming events
            printf("%02d/%02d/%04d: %s\n", events[i].day, events[i].month, events[i].year, events[i].description);
        }
    }
}


// Function to save events to a file
void saveToFile(struct Event *events, int numEvents) {
    // Implement file saving logic here
    FILE *file = fopen("calendar_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%d\n", numEvents);
    for (int i = 0; i < numEvents; i++) {
        fprintf(file, "%d %d %d %s\n", events[i].day, events[i].month, events[i].year, events[i].description);
    }

    fclose(file);
    printf("Calendar data saved to file.\n");
}

// Function to load events from a file
void loadFromFile(struct Event *events, int *numEvents) {
    // Implement file loading logic here
    FILE *file = fopen("calendar_data.txt", "r");
    if (file == NULL) {
        printf("No saved calendar data found.\n");
        return;
    }

    fscanf(file, "%d", numEvents);
    for (int i = 0; i < *numEvents; i++) {
        fscanf(file, "%d %d %d %[^\n]", &events[i].day, &events[i].month, &events[i].year, events[i].description);
    }

    fclose(file);
    printf("Calendar data loaded from file.\n");
}

int main() {
    struct Event events[100]; // Assuming a maximum of 100 events
    int numEvents = 0;

    // Load existing events from a file
    loadFromFile(events, &numEvents);

    int choice;
    do {
        // Display menu to the user
        printf("\n1. Display Calendar\n");
        printf("2. Add Event\n");
        printf("3. Delete Event\n");
        printf("4. Remind Events\n");
        printf("5. Save and Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:{
    // Get month and year from the user and display the calendar
    int month, year;
    printf("Enter month and year (MM YYYY): ");
    if (scanf("%d %d", &month, &year) != 2 || month < 1 || month > 12 || year < 0) {
        printf("Invalid input. Please enter a valid month (1-12) and year (>= 0).\n");
        // Clear input buffer
        while (getchar() != '\n');
        continue;  // Repeat the loop
    }
    displayCalendar(month, year, events, numEvents);
    break;}

            case 2:
                // Add an event
                addEvent(events, &numEvents);
                break;

            case 3:{
                // Get date from the user and delete the corresponding event
                int delDay, delMonth, delYear;
                printf("Enter date to delete (DD MM YYYY): ");
                scanf("%d %d %d", &delDay, &delMonth, &delYear);
                deleteEvent(events, &numEvents, delDay, delMonth, delYear);
                break;}

            // Remind upcoming events
case 4:{
    // Get the current date and remind upcoming events
    int currentDay, currentMonth, currentYear;
    printf("Enter current date (DD MM YYYY): ");
    scanf("%d %d %d", &currentDay, &currentMonth, &currentYear);
    remindEvents(events, numEvents, currentDay, currentMonth, currentYear);
    break;}


            case 5:
                // Save events to a file and exit
                saveToFile(events, numEvents);
                printf("Calendar saved. Exiting...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}
