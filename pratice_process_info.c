/*Write a C Program to store and retrieve the Process details such as pid, process name,
process arrival time, process completion time, process execution time.
Using Data Structure and arrays to demonstrate the above scenario.*/

#include <stdio.h>
#include <stdlib.h>

struct process {
    int process_id;
    char process_name[50];
    int arrival_time;
    int completion_time;
    int execution_time;
};

// Function to input process details
void input_process_details(struct process *p) {
    printf("Enter Process ID: ");
    scanf("%d", &p->process_id);

    printf("Enter Process Name: ");
    scanf("%s", p->process_name);

    printf("Enter Arrival Time: ");
    scanf("%d", &p->arrival_time);

    printf("Enter Completion Time: ");
    scanf("%d", &p->completion_time);

    p->execution_time = p->completion_time - p->arrival_time;
}

// Function to display process details
void display_process_details(struct process p) {
    printf("\nProcess ID: %d\n", p.process_id);
    printf("Process Name: %s\n", p.process_name);
    printf("Arrival Time: %d\n", p.arrival_time);
    printf("Completion Time: %d\n", p.completion_time);
    printf("Execution Time: %d\n", p.execution_time);
}

// Function to search for a process by process_id
void search_process(struct process *p, int n, int search_id) {
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (p[i].process_id == search_id) {
            display_process_details(p[i]);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("Process with ID %d not found.\n", search_id);
    }
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct process processes[n];  // Declare an array of structures

    int choice;
    do {
        printf("\n Menu's:\n");
        printf("1. Input Process Details\n");
        printf("2. Display All Process Details\n");
        printf("3. Search for a Process by ID\n");
        printf("4. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                // Input details for each process
                for (int i = 0; i < n; i++) {
                    printf("\nEnter details for Process %d:\n", i + 1);
                    input_process_details(&processes[i]);
                }
                break;

            case 2:
                // Display details for all processes
                printf("\nProcess Details:\n");
                for (int i = 0; i < n; i++) {
                    printf("\nProcess %d details:\n", i + 1);
                    display_process_details(processes[i]);
                }
                break;

            case 3:
                // Search for a process by ID
                {
                    int search_id;
                    printf("\nEnter Process ID to search: ");
                    scanf("%d", &search_id);
                    search_process(processes, n, search_id);
                }
                break;

            case 4:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 4);

    return 0;
}

