// #include <stdio.h>

// int main(){
//     int n,i,j,temp;
//     int bt[20],pr[20],wt[20],tat[20];

//     printf("Enter number of processes: ");
//     scanf("%d",&n);

//     printf("Enter burst time and priority:\n");
//     for(i=0;i<n;i++){
//         printf("P%d BT Priority: ",i+1);
//         scanf("%d%d",&bt[i],&pr[i]);
//     }

//     for(i=0;i<n;i++){
//         for(j=i+1;j<n;j++){
//             if(pr[i]>pr[j]){
//                 temp=pr[i]; pr[i]=pr[j]; pr[j]=temp;
//                 temp=bt[i]; bt[i]=bt[j]; bt[j]=temp;
//             }
//         }
//     }

//     wt[0]=0;

//     for(i=1;i<n;i++)
//         wt[i]=wt[i-1]+bt[i-1];

//     for(i=0;i<n;i++)
//         tat[i]=bt[i]+wt[i];

//     printf("\nProcess\tBT\tPR\tWT\tTAT\n");
//     for(i=0;i<n;i++)
//         printf("P%d\t%d\t%d\t%d\t%d\n",i+1,bt[i],pr[i],wt[i],tat[i]);
// }




// with the arrival time
#include <stdio.h>
#include <string.h>

int main(){

    int n, i;
    int at[20], bt[20], pr[20];
    int wt[20], tat[20], ct[20];
    int pid[20], done[20] = {0};

    // --- Added Gantt Chart Variables ---
    char gantt[1000] = "";
    char temp[50];
    int last = -2;   // tracks last running state (-2 initial, -1 idle, >=0 process index)

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Arrival Time, Burst Time and Priority:\n");

    for(i = 0; i < n; i++){
        pid[i] = i + 1;

        printf("P%d AT: ", i + 1);
        scanf("%d", &at[i]);

        printf("P%d BT: ", i + 1);
        scanf("%d", &bt[i]);

        printf("P%d Priority: ", i + 1);
        scanf("%d", &pr[i]);
    }

    int completed = 0, time = 0;

    // Initialize Gantt chart with the starting time
    sprintf(gantt, "%d", time);

    while(completed < n){

        int idx = -1;
        int highest = 9999;

        // Find the arrived process with the highest priority (lowest number = highest priority)
        for(i = 0; i < n; i++){
            if(at[i] <= time && done[i] == 0 && pr[i] < highest){
                highest = pr[i];
                idx = i;
            }
        }

        // --- GANTT CHART LOGIC (Formatting Only) ---
        // Triggered when switching states (Idle->Process, Process->Idle, or Process->Process)
        if(idx != last) {
            
            // Close the previous block with the current time (skip if it's the very first iteration)
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

        // --- MATHEMATICAL / PROCESS LOGIC (State Updates Only) ---
        if(idx == -1){
            time++;   // CPU idle, advance by 1 tick
        }
        else{
            wt[idx] = time - at[idx];

            time += bt[idx]; // Fast-forward time by the Burst Time

            ct[idx] = time;
            tat[idx] = ct[idx] - at[idx];

            done[idx] = 1;
            completed++;
        }
        
        last = idx; // Remember the state for the next loop
    }

    // Append the final completion time to close out the Gantt chart
    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    // --- PRINT RESULTS ---
    printf("\nProcess\tAT\tBT\tPR\tWT\tTAT\n");

    for(i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], pr[i], wt[i], tat[i]);
        
    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
}