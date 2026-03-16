// #include <stdio.h>

// int main(){
//     int n,i,quantum,remain,time=0;
//     int bt[20],rt[20],wt[20],tat[20];

//     printf("Enter number of processes: ");
//     scanf("%d",&n);

//     printf("Enter burst times:\n");
//     for(i=0;i<n;i++){
//         printf("P%d: ",i+1);
//         scanf("%d",&bt[i]);
//         rt[i]=bt[i];
//     }

//     printf("Enter time quantum: ");
//     scanf("%d",&quantum);

//     remain=n;

//     while(remain>0){
//         for(i=0;i<n;i++){
//             if(rt[i]>0){
//                 if(rt[i]>quantum){
//                     time+=quantum;
//                     rt[i]-=quantum;
//                 }
//                 else{
//                     time+=rt[i];
//                     wt[i]=time-bt[i];
//                     rt[i]=0;
//                     remain--;
//                 }
//             }
//         }
//     }

//     for(i=0;i<n;i++)
//         tat[i]=bt[i]+wt[i];

//     printf("\nProcess\tWT\tTAT\n");
//     for(i=0;i<n;i++)
//         printf("P%d\t%d\t%d\n",i+1,wt[i],tat[i]);
// }




//arrival time also 
#include <stdio.h>
#include <string.h>

int main() {
    int n, i, quantum;
    int at[20], bt[20], rt[20];
    int wt[20], tat[20], ct[20];
    int pid[20];

    // --- Added Gantt Chart Variables ---
    char gantt[1000] = "";
    char temp[50];
    int last = -2;   // tracks last running state (-2 initial, -1 idle, >=0 process index)

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Arrival Time and Burst Time:\n");
    for(i = 0; i < n; i++) {
        pid[i] = i + 1;

        printf("P%d AT: ", i + 1);
        scanf("%d", &at[i]);

        printf("P%d BT: ", i + 1);
        scanf("%d", &bt[i]);

        rt[i] = bt[i];
    }

    printf("\nEnter Time Quantum: ");
    scanf("%d", &quantum);

    int time = 0, completed = 0;
    int current_i = 0; // Persistent pointer to track whose turn it is next

    // Initialize Gantt chart with the starting time
    sprintf(gantt, "%d", time);

    while(completed < n) {
        int idx = -1;

        // Circular scan to find the next ready process
        for(int step = 0; step < n; step++) {
            if(at[current_i] <= time && rt[current_i] > 0) {
                idx = current_i;
                current_i = (current_i + 1) % n; // Move to the next process for the future
                break; // Stop looking, we found our next process
            }
            // If not ready, check the next one in the circle
            current_i = (current_i + 1) % n; 
        }

        // --- 1. GANTT CHART LOGIC (Formatting Only) ---
        // (If you want the Gantt chart to draw a line every single quantum even if 
        // the same process runs twice in a row, change this to: if(1) { ... )
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
            time++; // CPU idle
        } 
        else {
            if(rt[idx] > quantum) {
                time += quantum;
                rt[idx] -= quantum;
            } else {
                time += rt[idx];
                rt[idx] = 0;

                ct[idx] = time;
                tat[idx] = ct[idx] - at[idx];
                wt[idx] = tat[idx] - bt[idx];

                completed++;
            }
        }

        last = idx; // Remember the state for the next loop
    }

    // Append the final completion time to close out the Gantt chart
    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    // --- PRINT RESULTS ---
    printf("\nProcess\tAT\tBT\tWT\tTAT\n");
    for(i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], wt[i], tat[i]);

    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
}