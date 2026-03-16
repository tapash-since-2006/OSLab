// #include <stdio.h>

// int main(){
//     int n,i,time=0,remain;
//     int bt[20],rt[20],wt[20],tat[20],smallest;

//     printf("Enter number of processes: ");
//     scanf("%d",&n);

//     for(i=0;i<n;i++){
//         printf("P%d BT: ",i+1);
//         scanf("%d",&bt[i]);
//         rt[i]=bt[i];
//     }

//     remain=n;

//     while(remain>0){
//         smallest=-1;

//         for(i=0;i<n;i++){
//             if(rt[i]>0 && (smallest==-1 || rt[i]<rt[smallest]))
//                 smallest=i;
//         }

//         rt[smallest]--;
//         time++;

//         if(rt[smallest]==0){
//             remain--;
//             tat[smallest]=time;
//             wt[smallest]=tat[smallest]-bt[smallest];
//         }
//     }

//     printf("\nProcess\tWT\tTAT\n");
//     for(i=0;i<n;i++)
//         printf("P%d\t%d\t%d\n",i+1,wt[i],tat[i]);
// }


// #include<stdio.h>

// int main(){
//   int n,i;
//   int at[20], bt[20], rt[20];
//   int wt[20], tat[20], ct[20];
//   int pid[20];

//   int completed=0;
//   int time=0;

//   printf("Enter number of processes: ");
//   scanf("%d",&n);

//   printf("\nEnter Arrival Time and Burst Time:\n");

//   for(int i=0;i<n;i++){
//     pid[i]=i+1;

//     printf("P%d AT: ", i+1);
//     scanf("%d", &at[i]);

//     printf("P%d BT: ", i+1);
//     scanf("%d",&bt[i]);

//     rt[i]=bt[i];
//   }

//   while(completed<n){
//     int idx=-1;
//     int min=9999;

//     for(int i=0;i<n;i++){
//       if(at[i]<=time && rt[i]>0 && rt[i]<min){
//         min=rt[i];
//         idx=i;
//       }
//     }

//     if(idx==-1){
//       time++;
//     }else{
//       rt[idx]--;
//       time++;

//       if(rt[idx]==0){
//         ct[idx]=time;
//         tat[idx]=ct[idx]-at[idx];
//         wt[idx]=tat[idx]-bt[idx];
//         completed++;
//       }
//     }
//   }

//   printf("\nProcess\tAT\tBT\tWT\tTAT\n");

//     for(i=0;i<n;i++)
//         printf("P%d\t%d\t%d\t%d\t%d\n",
//         pid[i],at[i],bt[i],wt[i],tat[i]);

//     return 0;
// }



#include <stdio.h>
#include <string.h>

int main() {
    int n, i;
    int at[20], bt[20], rt[20];
    int wt[20], tat[20], ct[20];
    int pid[20];

    int completed = 0;
    int time = 0;

    char gantt[1000] = "";
    char temp[50];

    int last = -2;   // -2 initial state, -1 idle, >=0 process index

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Arrival Time and Burst Time:\n");

    for(i = 0; i < n; i++) {
        pid[i] = i + 1;

        printf("P%d AT: ", i + 1);
        scanf("%d", &at[i]);

        printf("P%d BT: ", i + 1);
        scanf("%d", &bt[i]);

        rt[i] = bt[i]; // Copy burst time to remaining time
    }

    // Initialize Gantt chart with the starting time (0)
    sprintf(gantt, "%d", time);

    while(completed < n) {
        int idx = -1;
        int min = 9999;

        // Find the process with the shortest remaining time that has arrived
        for(i = 0; i < n; i++) {
            if(at[i] <= time && rt[i] > 0 && rt[i] < min) {
                min = rt[i];
                idx = i;
            }
        }

        // --- GANTT CHART LOGIC ---
        // If the executing process changes (or goes to/from idle)
        if(idx != last) {
            // Close the previous block with the current time (skip if it's the very first tick)
            if(last != -2) {
                sprintf(temp, " | %d", time);
                strcat(gantt, temp);
            }
            
            // Open the new block
            if(idx == -1) {
                strcat(gantt, " | Idle");
            } else {
                sprintf(temp, " | P%d", pid[idx]);
                strcat(gantt, temp);
            }
        }

        // --- PROCESS EXECUTION LOGIC ---
        if(idx == -1) {
            // No process is ready, just advance time
            time++;
        } else {
            // Execute the chosen process for 1 unit of time
            rt[idx]--;
            time++;
            
            // If the process just finished
            if(rt[idx] == 0) {
                ct[idx] = time;
                tat[idx] = ct[idx] - at[idx];
                wt[idx] = tat[idx] - bt[idx];
                completed++;
            }
        }
        
        last = idx; // Remember the process that just executed
    }

    // Append the very final completion time to close out the Gantt chart
    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    // --- PRINT RESULTS ---
    printf("\nProcess\tAT\tBT\tWT\tTAT\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], wt[i], tat[i]);
    }

    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
}