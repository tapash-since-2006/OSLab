#include <stdio.h>
#include <string.h> // Added for string formatting

int gcd(int a, int b){
    while(b != 0){
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

int lcm(int a, int b){
    return (a * b) / gcd(a, b);
}

int main(){

    int n, i, time = 0;
    int exec[10], period[10];
    int rt[10], deadline[10];
    int hyper;

    // --- Added Gantt Chart Variables ---
    char gantt[1000] = "";
    char temp[50];
    int last = -2;

    printf("Enter number of tasks: ");
    scanf("%d", &n);

    printf("\nEnter Execution Time and Period\n");

    for(i = 0; i < n; i++){

        printf("Task %d Execution Time: ", i + 1);
        scanf("%d", &exec[i]);

        printf("Task %d Period: ", i + 1);
        scanf("%d", &period[i]);

        rt[i] = 0;
        deadline[i] = period[i]; // Initial deadline
    }

    // Hyperperiod calculation
    hyper = period[0];
    for(i = 1; i < n; i++)
        hyper = lcm(hyper, period[i]);

    printf("\nHyperperiod = %d\n", hyper);

    // Initialize Gantt chart with the starting time
    sprintf(gantt, "%d", time);

    while(time < hyper){

        // --- 0. REPLENISHMENT LOGIC ---
        // At the start of every period, reset remaining time and calculate the new absolute deadline
        for(i = 0; i < n; i++){
            if(time % period[i] == 0){
                rt[i] = exec[i];
                deadline[i] = time + period[i];
            }
        }

        // --- SELECTION LOGIC ---
        int idx = -1;
        int earliest = 99999; // EDF prioritizes the smallest absolute deadline

        for(i = 0; i < n; i++){
            if(rt[i] > 0 && deadline[i] < earliest){
                earliest = deadline[i];
                idx = i;
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
                sprintf(temp, " | T%d", idx + 1);
                strcat(gantt, temp);
            }
        }

        // --- 2. MATHEMATICAL / PROCESS LOGIC (State Updates Only) ---
        if(idx == -1){
            time++; // CPU Idle
        }
        else{
            rt[idx]--; // Execute for 1 unit
            time++;
        }
        
        last = idx; // Remember the state for the next tick
    }

    // Append the final completion time (Hyperperiod) to close out the Gantt chart
    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
}