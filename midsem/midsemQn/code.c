#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 20

int n, quantum;
int at[MAX], bt[MAX], rt[MAX], pid[MAX], priority[MAX];
int wt[MAX], tat[MAX], ct[MAX];

int in_queue[MAX] = {0};

char gantt[1000] = "";
char temp[50];
int last = -2;

// ---------------- THREAD 1: SORT ----------------
// Thread function to sort processes based on Arrival Time and Priority
// This function will be run by a separate thread to prepare the ready queue
void* sort_processes(void* arg) {
    // Outer loop iterates over each process except the last one
    for (int i = 0; i < n - 1; i++) {
        // Inner loop compares process i with all subsequent processes
        for (int j = i + 1; j < n; j++) {
            // Condition to decide if we should swap process i and j:
            // 1. If the arrival time of process i is greater than process j (arrives later)
            // 2. If the arrival times are the same, prefer the process with higher priority
            if (at[i] > at[j] || (at[i] == at[j] && priority[i] < priority[j])) {
                
                // Swap Arrival Times
                int temp = at[i];
                at[i] = at[j];
                at[j] = temp;

                // Swap Burst Times
                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                // Swap Priorities
                temp = priority[i];
                priority[i] = priority[j];
                priority[j] = temp;

                // Swap Process IDs
                temp = pid[i];
                pid[i] = pid[j];
                pid[j] = temp;
            }
        }
    }

    // After sorting, print the ready queue order for verification
    printf("\nThread 1: Ready Queue Order (after sorting by arrival and priority):\n");

    // Loop through the sorted processes to print their order
    for (int i = 0; i < n; i++) {
        printf("P%d", pid[i]);  // Print process ID
        if (i != n - 1) {
            printf(" -> ");    // Arrow to indicate sequence
        }
    }
    printf("\n"); // Newline after printing the queue

    // Exit the thread
    return NULL;
}

// ---------------- THREAD 2: ROUND ROBIN ----------------
void* round_robin(void* arg) {
    // Increased queue size to 1000 to prevent overflow in high-preemption cases
    for(int i=0;i<n;i++){
      in_queue[i]=0;
    }
    int queue[1000], front = 0, rear = 0; 
    int time = 0, completed = 0;

    for(int i = 0; i < n; i++)
        rt[i] = bt[i];

    sprintf(gantt, "%d", time);

    for(int i = 0; i < n; i++) {
        if(at[i] <= time) {
            queue[rear++] = i;
            in_queue[i] = 1;
        }
    }

    while(completed < n) {
        int idx;
        if(front == rear) idx = -1;
        else idx = queue[front++];

        if(idx != last) {
            if(last != -2) {
                sprintf(temp, " | %d", time);
                strcat(gantt, temp);
            }
            if(idx == -1) strcat(gantt, " | Idle");
            else {
                sprintf(temp, " | P%d", pid[idx]);
                strcat(gantt, temp);
            }
        }

        if(idx == -1) {
            time++;
            for(int i = 0; i < n; i++) {
                if(at[i] <= time && !in_queue[i]) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                }
            }
        } else {
            int exec = (rt[idx] > quantum) ? quantum : rt[idx];
            for(int t = 0; t < exec; t++) {
                time++;
                for(int j = 0; j < n; j++) {
                    if(at[j] <= time && !in_queue[j]) {
                        queue[rear++] = j;
                        in_queue[j] = 1;
                    }
                }
            }
            rt[idx] -= exec;
            if(rt[idx] > 0) queue[rear++] = idx;
            else {
                completed++;
                ct[idx] = time;
                tat[idx] = ct[idx] - at[idx];
                wt[idx] = tat[idx] - bt[idx];
            }
        }
        last = idx;
    }

    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    printf("\nThread 2: Round Robin Scheduling (Time Quantum = %d)\n", quantum);
    
    // Gantt Chart Output
    printf("\nGantt Chart:\n%s\n", gantt);

    // Table and Averages Output
    float total_tat = 0, total_wt = 0;
    printf("\n--- Final Scheduling Table ---\n");
    printf("PID\tAT\tBT\tPriority\tCT\tTAT\tWT\n");
    for(int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t\t%d\t%d\t%d\n",
            pid[i], at[i], bt[i], priority[i],
            ct[i], tat[i], wt[i]);
        total_tat += tat[i];
        total_wt += wt[i];
    }

    printf("\nAverage Turnaround Time = %.2f", total_tat / n);
    printf("\nAverage Waiting Time = %.2f\n", total_wt / n);

    return NULL;
}

// ---------------- MAIN ----------------
int main() {
    pthread_t t1, t2;

    printf("Number of Processes = ");
    scanf("%d", &n);

    printf("\nPID AT BT Priority\n");
    for(int i = 0; i < n; i++) {
      scanf("%d %d %d %d", &pid[i], &at[i], &bt[i], &priority[i]);
    }

    quantum = 4; // As per sample output

    // THREAD 1 → SORT
    pthread_create(&t1, NULL, sort_processes, NULL);
    pthread_join(t1, NULL);

    // THREAD 2 → ROUND ROBIN
    pthread_create(&t2, NULL, round_robin, NULL);
    pthread_join(t2, NULL);

    return 0;
}