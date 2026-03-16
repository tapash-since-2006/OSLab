// #include<stdio.h>

// int main(){
//   int n, bt[20], wt[20], tat[20], i;
//   float avg_wt=0, avg_tat=0;

//   printf("Enter the number of processes: ");
//   scanf("%d", &n);

//   printf("Enter the burst times: \n");
//   for(int i=0;i<n;i++){
//     printf("P%d: ", i+1);
//     scanf("%d", &bt[i]);
//   }

//   wt[0]=0;

//   for(int i=1;i<n;i++){
//     wt[i]=wt[i-1]+bt[i-1];
//   }

//   for(int i=0;i<n;i++){
//     tat[i]=bt[i]+wt[i];
//     avg_wt+=wt[i];
//     avg_tat+=tat[i];
//   }
//   printf("\nProcess\tBT\tWT\tTAT\n");
//   for(i=0;i<n;i++)
//     printf("P%d\t%d\t%d\t%d\n",i+1,bt[i],wt[i],tat[i]);

//   printf("\nAverage WT = %.2f",avg_wt/n);
//   printf("\nAverage TAT = %.2f\n",avg_tat/n);
// }



// with the arrival time
#include <stdio.h>
#include <string.h>

int main(){

    int n, i, j;
    int at[20], bt[20], wt[20], tat[20], ct[20], pid[20];
    float avg_wt=0, avg_tat=0;

    // --- Added Gantt Chart Variables ---
    char gantt[1000] = "";
    char temp[50];
    int last = -2;   // tracks last running state (-2 initial, -1 idle, >=0 process index)

    printf("Enter number of processes: ");
    scanf("%d",&n);

    for(i=0; i<n; i++){
        pid[i] = i+1;   // store process ID
    }

    printf("\nEnter Arrival Time and Burst Time:\n");
    for(i=0; i<n; i++){
        printf("P%d AT: ",pid[i]);
        scanf("%d",&at[i]);
        printf("P%d BT: ",pid[i]);
        scanf("%d",&bt[i]);
    }

    // Sort by arrival time
    for(i=0; i<n-1; i++){
        for(j=i+1; j<n; j++){
            if(at[i] > at[j]){
                int temp;

                temp = at[i];
                at[i] = at[j];
                at[j] = temp;

                temp = bt[i];
                bt[i] = bt[j];
                bt[j] = temp;

                temp = pid[i];      // swap process id also
                pid[i] = pid[j];
                pid[j] = temp;
            }
        }
    }

    int time = 0, completed = 0;
    int current_p = 0; // Pointer to the next process in our sorted list

    // Initialize Gantt chart with the starting time
    sprintf(gantt, "%d", time);

    while(completed < n){
        int idx = -1;

        // In FCFS, we just check if the *next* process in the sorted list has arrived
        if(current_p < n && at[current_p] <= time) {
            idx = current_p;
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
                sprintf(temp, " | P%d", pid[idx]);
                strcat(gantt, temp);
            }
        }

        // --- 2. MATHEMATICAL / PROCESS LOGIC (State Updates Only) ---
        if(idx == -1) {
            time++; // CPU is idle, wait for the next process to arrive
        } 
        else {
            wt[idx] = time - at[idx];
            
            time += bt[idx]; // Process runs to completion
            
            ct[idx] = time;
            tat[idx] = ct[idx] - at[idx];

            avg_wt += wt[idx];
            avg_tat += tat[idx];

            completed++;
            current_p++; // Move pointer to the next process in the sorted list
        }

        last = idx; // Remember the state for the next loop
    }

    // Append the final completion time to close out the Gantt chart
    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    printf("\nProcess\tAT\tBT\tWT\tTAT\n");

    for(i=0; i<n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], wt[i], tat[i]);

    printf("\nGantt Chart:\n%s\n", gantt);

    printf("\nAverage WT = %.2f", avg_wt/n);
    printf("\nAverage TAT = %.2f\n", avg_tat/n);

    return 0;
}