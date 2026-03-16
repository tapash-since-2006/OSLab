#include <stdio.h>
#include <string.h>

int main(){

    int n, i;
    int at[20], bt[20], rt[20];
    int ct[20], wt[20], tat[20];
    int level[20];

    // --- Added Gantt Chart Variables ---
    char gantt[1000] = "";
    char temp[50];
    int last = -2;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter AT and BT\n");

    for(i = 0; i < n; i++){
        printf("P%d AT: ", i + 1);
        scanf("%d", &at[i]);

        printf("P%d BT: ", i + 1);
        scanf("%d", &bt[i]);

        rt[i] = bt[i];
        level[i] = 1; // All processes start in Queue 1
    }

    int time = 0, completed = 0;
    int current_p = 0; // Circular pointer to ensure Round Robin fairness within levels

    // Initialize Gantt chart with the starting time
    sprintf(gantt, "%d", time);

    while(completed < n){

        int idx = -1;
        int highest_priority = 4; // 1 is highest, 3 is lowest

        // --- SELECTION LOGIC ---
        // Step 1: Find the highest priority queue that actually has ready processes
        for(i = 0; i < n; i++) {
            if(at[i] <= time && rt[i] > 0 && level[i] < highest_priority) {
                highest_priority = level[i];
            }
        }

        // Step 2: Use a circular scan to pick the next process in THAT specific queue
        if(highest_priority <= 3) {
            for(int step = 0; step < n; step++) {
                if(at[current_p] <= time && rt[current_p] > 0 && level[current_p] == highest_priority) {
                    idx = current_p;
                    current_p = (current_p + 1) % n; // Move pointer forward for next time
                    break;
                }
                current_p = (current_p + 1) % n;
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
        if(idx == -1) {
            time++; // CPU Idle
        } 
        else {
            int quantum;

            // Set quantum based on the process's current level
            if(level[idx] == 1)
                quantum = 2;
            else if(level[idx] == 2)
                quantum = 4;
            else
                quantum = rt[idx]; // Level 3 is FCFS (runs to completion)

            // Execute the process
            if(rt[idx] > quantum){
                rt[idx] -= quantum;
                time += quantum;
                
                // Demote process to a lower priority queue if it didn't finish
                if(level[idx] < 3)
                    level[idx]++;
            }
            else {
                time += rt[idx];
                rt[idx] = 0;
                ct[idx] = time;
                completed++;
            }
        }

        last = idx; // Remember the state for the next loop
    }

    // Calculate TAT and WT at the very end
    for(i = 0; i < n; i++){
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    // Append the final completion time to close out the Gantt chart
    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    // --- PRINT RESULTS ---
    printf("\nProcess\tAT\tBT\tWT\tTAT\n");
    for(i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\n", i + 1, at[i], bt[i], wt[i], tat[i]);

    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
}