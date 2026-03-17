// #include <stdio.h>

// int main(){
//     int n,i,time=0,remain;
//     int bt[20],rt[20],pr[20],wt[20],tat[20],smallest;

//     printf("Enter number of processes: ");
//     scanf("%d",&n);

//     for(i=0;i<n;i++){
//         printf("P%d BT Priority: ",i+1);
//         scanf("%d%d",&bt[i],&pr[i]);
//         rt[i]=bt[i];
//     }

//     remain=n;

//     while(remain>0){
//         smallest=-1;

//         for(i=0;i<n;i++){
//             if(rt[i]>0 && (smallest==-1 || pr[i]<pr[smallest]))
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



// // with the arrival time


// #include <stdio.h>
// #include <string.h>

// int main(){

//     int n, i;
//     int at[20], bt[20], pr[20];
//     int rt[20], ct[20], wt[20], tat[20];
//     int pid[20];

//     // --- Added Gantt Chart Variables ---
//     char gantt[1000] = "";
//     char temp[50];
//     int last = -2;   // tracks last running state (-2 initial, -1 idle, >=0 process index)

//     printf("Enter number of processes: ");
//     scanf("%d", &n);

//     printf("\nEnter Arrival Time, Burst Time and Priority:\n");

//     for(i = 0; i < n; i++){
//         pid[i] = i + 1;

//         printf("P%d AT: ", i + 1);
//         scanf("%d", &at[i]);

//         printf("P%d BT: ", i + 1);
//         scanf("%d", &bt[i]);

//         printf("P%d Priority: ", i + 1);
//         scanf("%d", &pr[i]);

//         rt[i] = bt[i]; // Initialize remaining time
//     }

//     int completed = 0, time = 0;

//     // Initialize Gantt chart with the starting time
//     sprintf(gantt, "%d", time);

//     while(completed < n){

//         int idx = -1;
//         int highest = 9999;

//         // Find the arrived process with the highest priority (lowest number) and remaining time > 0
//         for(i = 0; i < n; i++){
//             if(at[i] <= time && rt[i] > 0 && pr[i] < highest){
//                 highest = pr[i];
//                 idx = i;
//             }
//         }

//         // --- GANTT CHART LOGIC (Formatting Only) ---
//         // Triggered when switching states (Idle->Process, Process->Idle, or Process A->Process B)
//         if(idx != last) {
            
//             // Close the previous block with the current time (skip if it's the very first iteration)
//             if(last != -2) {
//                 sprintf(temp, " | %d", time);
//                 strcat(gantt, temp);
//             }
            
//             // Open the new block
//             if(idx == -1) {
//                 strcat(gantt, " | Idle");
//             } else {
//                 sprintf(temp, " | P%d", pid[idx]);
//                 strcat(gantt, temp);
//             }
//         }

//         // --- MATHEMATICAL / PROCESS LOGIC (State Updates Only) ---
//         if(idx == -1){
//             time++;   // CPU idle, advance by 1 unit
//         }
//         else{
//             rt[idx]--;   // Execute for 1 unit
//             time++;

//             // If the process just finished
//             if(rt[idx] == 0){
//                 ct[idx] = time;
//                 tat[idx] = ct[idx] - at[idx];
//                 wt[idx] = tat[idx] - bt[idx];

//                 completed++;
//             }
//         }

//         last = idx; // Remember the state for the next tick
//     }

//     // Append the final completion time to close out the Gantt chart
//     sprintf(temp, " | %d", time);
//     strcat(gantt, temp);

//     // --- PRINT RESULTS ---
//     printf("\nProcess\tAT\tBT\tPR\tWT\tTAT\n");

//     for(i = 0; i < n; i++)
//         printf("P%d\t%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], pr[i], wt[i], tat[i]);

//     printf("\nGantt Chart:\n%s\n", gantt);

//     return 0;
// }


#include<stdio.h>
#include<string.h>

int main(){
    int n,i;
    int at[20], bt[20], rt[20];
    int tat[20], ct[20], wt[20];
    int pid[20], pri[20];

    int completed=0;
    int time=0;

    char gantt[200]="";
    char temp[20];

    int last=-2; 

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("\nEnter Arrival Time and Burst Time and The Priority(Lower the more Priority):\n");

    for(int i=0;i<n;i++){
        pid[i]=i+1;

        printf("P%d AT :", i+1);
        scanf("%d", &at[i]);

        printf("P%d BT :", i+1);
        scanf("%d", &bt[i]);

        printf("Enter the priority of P%d", i+1);
        scanf("%d", &pri[i]);

        rt[i]=bt[i];
    }

    sprintf(gantt, "%d", time);

    while(completed<n){
        int idx=-1;
        int min=9999; // lower the value more the priority in this case

        for(int i=0;i<n;i++){
            if(at[i]<=time && rt[i]>0 && pri[i]<min){
                idx=i;
                min=pri[i];
            }
        }

            //GANTT LOGIC
            if(last!=idx){
                if(last!=-2){//close it
                    sprintf(temp, " | %d", time);
                    strcat(gantt, temp);
                }

                //put the process or the idle
                if(idx==-1){
                    strcat(gantt, " | Idle");
                }else{
                    sprintf(temp, " | P%d", pid[idx]);
                    strcat(gantt, temp);
                }
            }

            // mathematical logic
            if(idx==-1){
                time++;
            }else{
                rt[idx]--;
                time++;

                if(rt[idx]==0){
                    ct[idx]=time;
                    tat[idx]=ct[idx]-at[idx];
                    wt[idx]=tat[idx]-bt[idx];
                    completed++;
                }
            }
            last=idx;
        }

        sprintf(temp, " | %d", time);
        strcat(gantt,temp);

        printf("\nProcess\tAT\tBT\tWT\tTAT\n");
    for(i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n", pid[i], at[i], bt[i], wt[i], tat[i]);
    }

    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
    }