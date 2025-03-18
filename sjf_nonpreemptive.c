#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct process_sjf {
    char process_id[5];
    int arrival_time;
    int burst_time;
    int remaining_burst_time;
    int completion_time;
    int turn_around_time;
    int waiting_time;
    int response_time;
    int start_time;
};

// Function to input process details
void input_process_details(struct process_sjf *p) {
    printf("Enter Process ID: ");
    scanf("%s", p->process_id);
    printf("Enter Arrival Time: ");
    scanf("%d", &p->arrival_time);
    printf("Enter Burst Time: ");
    scanf("%d", &p->burst_time);
    p->remaining_burst_time = p->burst_time; // Initialize remaining burst time
    p->start_time = -1; // Initialize start time
}

// Function to calculate times for non-preemptive SJF
void calculate_times_non_preemptive(struct process_sjf *p, int n) {
    int current_time = 0, completed = 0;
    bool is_completed[n];
    for (int i = 0; i < n; i++) is_completed[i] = false;

    while (completed < n) {
        int idx = -1, min_burst_time = 10000;
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && p[i].arrival_time <= current_time && p[i].burst_time < min_burst_time) {
                min_burst_time = p[i].burst_time;
                idx = i;
            }
        }

        if (idx != -1) {
            if (p[idx].start_time == -1) p[idx].start_time = current_time;
            current_time += p[idx].burst_time;
            p[idx].completion_time = current_time;
            p[idx].turn_around_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turn_around_time - p[idx].burst_time;
            p[idx].response_time = p[idx].waiting_time;
            is_completed[idx] = true;
            completed++;
        } else {
            current_time++;
        }
    }
}

// Function to calculate times for preemptive SJF
void calculate_times_preemptive(struct process_sjf *p, int n) {
    int current_time = 0, completed = 0;
    bool is_completed[n];
    for (int i = 0; i < n; i++) is_completed[i] = false;

    while (completed < n) {
        int idx = -1, min_burst_time = 10000;
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && p[i].arrival_time <= current_time && p[i].remaining_burst_time < min_burst_time) {
                min_burst_time = p[i].remaining_burst_time;
                idx = i;
            }
        }

        if (idx != -1) {
            if (p[idx].start_time == -1) p[idx].start_time = current_time;
            p[idx].remaining_burst_time--;
            current_time++;

            if (p[idx].remaining_burst_time == 0) {
                is_completed[idx] = true;
                completed++;
                p[idx].completion_time = current_time;
                p[idx].turn_around_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turn_around_time - p[idx].burst_time;
                p[idx].response_time = p[idx].start_time - p[idx].arrival_time;
            }
        } else {
            current_time++;
        }
    }
}

void calculate_averages(struct process_sjf p[], int n) {
    int total_turnaround_time = 0, total_waiting_time = 0, total_response_time = 0;
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

void display_process_details(struct process_sjf p[], int n) {
    printf("\n%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n", "Process ID", "Arrival Time", "Burst Time", "Completion Time", "Turn Around Time", "Waiting Time", "Response Time");
    for (int i = 0; i < n; i++) {
        printf("%-15s%-15d%-15d%-15d%-15d%-15d%-15d\n", p[i].process_id, p[i].arrival_time, p[i].burst_time, p[i].completion_time, p[i].turn_around_time, p[i].waiting_time, p[i].response_time);
    }
    calculate_averages(p, n);
}

int main() {
    int n, choice, scheduling_type;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    struct process_sjf processes[n];

    for (int i = 0; i < n; i++) {
        printf("\nEnter details for Process %d:\n", i + 1);
        input_process_details(&processes[i]);
    }

    printf("\nChoose scheduling type:\n1. Non-Preemptive\n2. Preemptive\n");
    scanf("%d", &scheduling_type);

    switch(scheduling_type) {
        case 1:
            calculate_times_non_preemptive(processes, n);
            break;
        case 2:
            calculate_times_preemptive(processes, n);
            break;
        default:
            printf("Invalid choice, using non-preemptive scheduling by default.\n");
            calculate_times_non_preemptive(processes, n);
    }

    display_process_details(processes, n);
    return 0;
}
