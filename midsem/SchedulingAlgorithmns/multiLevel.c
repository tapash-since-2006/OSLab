#include <stdio.h>
#include <string.h>

int main(){

    int n, i, quantum;
    int at[20], bt[20], rt[20], q[20];
    int ct[20], wt[20], tat[20];

    // --- Added Gantt Chart Variables ---
    char gantt[1000] = "";
    char temp[50];
    int last = -2;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter AT, BT and Queue (1=RR, 2=FCFS)\n");

    for(i = 0; i < n; i++){
        printf("P%d AT: ", i + 1);
        scanf("%d", &at[i]);

        printf("P%d BT: ", i + 1);
        scanf("%d", &bt[i]);

        printf("Queue: ");
        scanf("%d", &q[i]);

        rt[i] = bt[i];
    }

    printf("\nEnter Time Quantum for Queue1: ");
    scanf("%d", &quantum);

    int time = 0, completed = 0;
    int current_q1 = 0; // Circular pointer for Round Robin (Queue 1)

    // Initialize Gantt chart with the starting time
    sprintf(gantt, "%d", time);

    while(completed < n){

        int idx = -1;
        int is_rr = 0; // Flag to tell our math block which rule to apply

        // --- SELECTION LOGIC ---
        // 1. Check Queue 1 (Round Robin) FIRST (Higher Priority)
        for(int step = 0; step < n; step++){
            if(q[current_q1] == 1 && at[current_q1] <= time && rt[current_q1] > 0){
                idx = current_q1;
                is_rr = 1;
                current_q1 = (current_q1 + 1) % n; // Move RR pointer forward
                break;
            }
            current_q1 = (current_q1 + 1) % n;
        }

        // 2. If Queue 1 is empty, check Queue 2 (FCFS)
        if(idx == -1){
            int min_at = 9999;
            for(i = 0; i < n; i++){
                if(q[i] == 2 && at[i] <= time && rt[i] > 0){
                    // Find the process in Q2 that arrived earliest
                    if(at[i] < min_at){
                        min_at = at[i];
                        idx = i;
                        is_rr = 0;
                    }
                }
            }
        }

        // --- 1. GANTT CHART LOGIC (Formatting Only) ---
        if(idx != last) {
            if(last != -2) {
                sprintf(temp, " | %d", time);
                strcat(gantt, temp);
            }
            if(idx == -1) {
                strcat(gantt, " | Idle");
            } else {
                sprintf(temp, " | P%d", idx + 1);
                strcat(gantt, temp);
            }
        }

        // --- 2. MATHEMATICAL / PROCESS LOGIC (State Updates Only) ---
        if(idx == -1){
            time++; // CPU idle
        }
        else {
            if(is_rr == 1) {
                // Execute using Round Robin rules
                if(rt[idx] > quantum){
                    time += quantum;
                    rt[idx] -= quantum;
                }
                else {
                    time += rt[idx];
                    rt[idx] = 0;
                    ct[idx] = time;
                    completed++;
                }
            } 
            else {
                // Execute using FCFS rules
                time += rt[idx]; // Runs to completion
                rt[idx] = 0;
                ct[idx] = time;
                completed++;
            }
        }

        last = idx; // Remember the state for the next loop
    }

    // Append the final completion time to close out the Gantt chart
    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    // Calculate TAT and WT at the very end
    for(i = 0; i < n; i++){
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    // --- PRINT RESULTS ---
    printf("\nProcess\tAT\tBT\tQ\tWT\tTAT\n");
    for(i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt[i], q[i], wt[i], tat[i]);

    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
}