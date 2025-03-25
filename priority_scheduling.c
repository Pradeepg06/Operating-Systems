#include <stdio.h>
#include <stdbool.h>

struct Process
{
    int pid, arrival_time, burst_time, remaining_time, priority;
    int completion_time, turnaround_time, waiting_time, response_time;
};

void calculateTimes(struct Process proc[], int n, bool isPreemptive)
{
    int time = 0, completed = 0;
    bool done[n];
    int first_run[n]; // To track if the process has started running for response time calculation

    for (int i = 0; i < n; i++)
    {
        proc[i].remaining_time = proc[i].burst_time;
        done[i] = false;
        first_run[i] = 0; // Initialize as not started
    }

    while (completed < n)
    {
        int idx = -1;
        // Select the process with highest priority (lowest number means highest priority)
        for (int i = 0; i < n; i++)
        {
            if (proc[i].arrival_time <= time && !done[i] &&
                (idx == -1 || proc[i].priority < proc[idx].priority))
            {
                idx = i;
            }
        }

        if (idx != -1)
        {
            if (first_run[idx] == 0)
            {
                // Response time is the first time the process is executed minus its arrival time
                proc[idx].response_time = time - proc[idx].arrival_time;
                first_run[idx] = 1; // Mark the process as started
            }

            // Process runs for one unit of time
            proc[idx].remaining_time--;
            if (proc[idx].remaining_time == 0)
            {
                proc[idx].completion_time = time + 1;
                proc[idx].turnaround_time = proc[idx].completion_time - proc[idx].arrival_time;
                proc[idx].waiting_time = proc[idx].turnaround_time - proc[idx].burst_time;
                done[idx] = true;
                completed++;
            }
        }

        time++; // Increment time
    }
}

void printAverageTimes(struct Process proc[], int n)
{
    float total_waiting_time = 0, total_turnaround_time = 0, total_response_time = 0;
    for (int i = 0; i < n; i++)
    {
        total_waiting_time += proc[i].waiting_time;
        total_turnaround_time += proc[i].turnaround_time;
        total_response_time += proc[i].response_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Response Time: %.2f\n", total_response_time / n);
}

int main()
{
    int n, choice;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[n];
    for (int i = 0; i < n; i++)
    {
        proc[i].pid = i + 1;
        printf("\nEnter details for Process %d:\n", proc[i].pid);
        printf("Arrival Time : ");
        scanf("%d", &proc[i].arrival_time);
        printf("Burst Time: ");
        scanf("%d", &proc[i].burst_time);
        printf("Priority: ");
        scanf("%d", &proc[i].priority);
    }

    printf("\nChoose the scheduling method:\n");
    printf("1. Preemptive Priority Scheduling\n");
    printf("2. Non-Preemptive Priority Scheduling\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    calculateTimes(proc, n, choice == 1);

    // Display the results in a tabular format
    printf("\nProcess ID | Arrival Time | Burst Time | Priority | Completion Time | Turnaround Time | Waiting Time  | Response Time \n");
    for (int i = 0; i < n; i++)
    {
        printf("%-11d %-13d %-11d %-9d %-16d %-16d %-14d %-12d\n", proc[i].pid, proc[i].arrival_time,
               proc[i].burst_time, proc[i].priority, proc[i].completion_time,
               proc[i].turnaround_time, proc[i].waiting_time, proc[i].response_time);
    }

    // Print average times
    printAverageTimes(proc, n);

    return 0;
}
