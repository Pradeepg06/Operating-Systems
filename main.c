#include <stdio.h>
#include <stdlib.h>

struct process_rr {
    char process_id[5];
    int arrival_time;
    int burst_time;
    int remaining_time; // Remaining time for the process
    int completion_time;
    int turn_around_time;
    int waiting_time;
    int response_time;
};

// Function to input process details
void input_process_details(struct process_rr *p) {
    printf("Enter Process ID: ");
    scanf("%s", p->process_id);

    printf("Enter Arrival Time: ");
    scanf("%d", &p->arrival_time);

    printf("Enter Burst Time: ");
    scanf("%d", &p->burst_time);
    p->remaining_time = p->burst_time; // Initialize remaining time
}

// Function to calculate completion time, waiting time, and turn around time
void calculate_times(struct process_rr *p, int n, int quantum) {
    int current_time = 0;
    int completed = 0;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (p[i].remaining_time > 0) {
                if (current_time < p[i].arrival_time) {
                    current_time = p[i].arrival_time; // Jump to the arrival time if idle
                }

                if (p[i].remaining_time > quantum) {
                    current_time += quantum;
                    p[i].remaining_time -= quantum;
                } else {
                    current_time += p[i].remaining_time;
                    p[i].completion_time = current_time;
                    p[i].turn_around_time = p[i].completion_time - p[i].arrival_time;
                    p[i].waiting_time = p[i].turn_around_time - p[i].burst_time;
                    p[i].response_time = p[i].waiting_time; // Response time is same as waiting time in RR
                    p[i].remaining_time = 0; // Process is completed
                    completed++;
                }
            }
        }
    }
}

// Function to display process details in table format
void display_process_details(struct process_rr p[], int n) {
    // Print header row
    printf("\n%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n", "Process ID", "Arrival Time", "Burst Time", "Completion Time", "Turn Around Time", "Waiting Time", "Response Time");
    printf("------------------------------------------------------------------------------------------------------\n");

    // Print process details for each process
    for (int i = 0; i < n; i++) {
        printf("%-15s%-15d%-15d%-15d%-15d%-15d%-15d\n", p[i].process_id, p[i].arrival_time, p[i].burst_time, p[i].completion_time, p[i].turn_around_time, p[i].waiting_time, p[i].response_time);
    }
}

// Function to sort processes based on arrival time (Bubble Sort)
void sort_processes_by_arrival_time(struct process_rr p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                // Swap the processes
                struct process_rr temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

// Function to calculate averages for turnaround time, waiting time, and response time
void calculate_averages(struct process_rr p[], int n) {
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;

    for (int i = 0; i < n; i++) {
        total_turnaround_time += p[i].turn_around_time;
        total_waiting_time += p[i].waiting_time;
        total_response_time += p[i].response_time;
    }

    printf("\nAverages:\n");
    printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / n);
    printf("Average Response Time: %.2f\n", (float)total_response_time / n);
}

int main() {
    int n, quantum;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct process_rr processes[n];  // Declare an array of structures

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Input Process Details\n");
        printf("2. Display All Process Details\n");
        printf("3. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                // Input details for each process
                for (int i = 0; i < n; i++) {
                    printf("\nEnter details for Process %d:\n", i + 1);
                    input_process_details(&processes[i]);
                }
                printf("Enter Time Quantum: ");
                scanf("%d", &quantum);
                break;

            case 2:
                // Sort processes by arrival time before calculating and displaying
                sort_processes_by_arrival_time(processes, n);
                calculate_times(processes, n, quantum);
                display_process_details(processes, n);
                calculate_averages(processes, n);  // Calculate and display averages
                break;

            case 3:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 3);

    return 0;
}
