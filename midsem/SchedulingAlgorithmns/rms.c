#include <stdio.h>
#include <math.h>
#include <string.h>

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
    int exec[10], period[10], rt[10];
    int hyper;

    float U = 0;

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

        rt[i] = 0; // Starts at 0, will be replenished at time=0

        // Utilization calculation
        U += (float)exec[i] / period[i];
    }

    // Hyperperiod calculation (LCM of all periods)
    hyper = period[0];
    for(i = 1; i < n; i++)
        hyper = lcm(hyper, period[i]);

    printf("\nHyperperiod = %d\n", hyper);

    // Utilization bound calculation: n * (2^(1/n) - 1)
    float bound = n * (pow(2, 1.0 / n) - 1);

    printf("CPU Utilization = %.3f\n", U);
    printf("RMS Bound = %.3f\n", bound);

    if(U <= bound)
        printf("Schedulable under RMS\n");
    else
        printf("May not be schedulable (Requires deeper exact analysis)\n");

    // Initialize Gantt chart with the starting time
    sprintf(gantt, "%d", time);

    // Run the simulation for exactly one Hyperperiod
    while(time < hyper){

        // --- 0. REPLENISHMENT LOGIC ---
        // If the current time is a multiple of a task's period, it "arrives" again
        for(i = 0; i < n; i++){
            if(time % period[i] == 0)
                rt[i] = exec[i];
        }

        // --- SELECTION LOGIC ---
        int idx = -1;
        int min_period = 9999; // RMS prioritizes the shortest period

        for(i = 0; i < n; i++){
            if(rt[i] > 0 && period[i] < min_period){
                min_period = period[i];
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
                sprintf(temp, " | T%d", idx + 1); // using 'T' for Task instead of 'P'
                strcat(gantt, temp);
            }
        }

        // --- 2. MATHEMATICAL / PROCESS LOGIC (State Updates Only) ---
        if(idx == -1){
            time++; // CPU Idle
        }
        else{
            rt[idx]--; // Execute task for 1 unit
            time++;
        }
        
        last = idx; // Remember the state for the next loop
    }

    // Append the final completion time (which will be the Hyperperiod)
    sprintf(temp, " | %d", time);
    strcat(gantt, temp);

    printf("\nGantt Chart:\n%s\n", gantt);

    return 0;
}