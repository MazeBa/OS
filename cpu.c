#include "oslabs.h"
#include <stdio.h>
struct PCB NULLPCB = { 0,0,0,0,0,0,0 };

int qAmt = 0;
int min(int a, int b) {
    if (a <= b)
        return a;
    else
        return b;
}

void printStruct(struct PCB temp) {
    printf("PID: %d \n arrival: %d \n total burst: %d \n exec-start: %d \n exec-end: %d \n remain-burst: %d \n priority: %d ",
        (int)temp.process_id, (int)temp.arrival_timestamp, (int)temp.total_bursttime, (int)temp.execution_starttime,
        (int)temp.execution_endtime, (int)temp.remaining_bursttime, (int)temp.process_priority);
}

struct PCB handle_process_arrival_pp(struct PCB ready_q[QUEUEMAX], int* queueCount, struct PCB currentProc, struct PCB newProc, int timestamp) {

    if (currentProc.process_id == 0 && currentProc.arrival_timestamp == 0 && currentProc.execution_endtime == 0
        && currentProc.execution_starttime == 0 && currentProc.process_priority == 0 && currentProc.remaining_bursttime == 0
        && currentProc.total_bursttime == 0)
    {
        newProc.execution_starttime = timestamp;
        newProc.execution_endtime = timestamp + newProc.total_bursttime;
        newProc.remaining_bursttime = newProc.total_bursttime;
        return newProc;
    }

    else {
        if (newProc.process_priority >= currentProc.process_priority) {
            newProc.execution_starttime = 0;
            newProc.execution_endtime = 0;
            newProc.remaining_bursttime = newProc.total_bursttime;
            int temp = *queueCount;

            ready_q[temp] = newProc;
            temp++;
            *queueCount = temp;
            return currentProc;
        }

        else {
            newProc.execution_starttime = timestamp;
            newProc.execution_endtime = timestamp + newProc.total_bursttime;
            newProc.remaining_bursttime = newProc.total_bursttime;
            currentProc.execution_endtime = 0;
            currentProc.remaining_bursttime -= 1;
            ready_q[*queueCount] = currentProc;
            *queueCount += 1;
            return newProc;
        }
    }


}


struct PCB  handle_process_completion_pp(struct PCB readyQ[QUEUEMAX], int* queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        return NULLPCB;
    }
    else {
        int priorityPos = 0;
        int prioritylvl = 9999;
        int i;
        struct PCB ans = NULLPCB;
        for (i = 0; i < *queue_cnt; i++) {
            if (readyQ[i].process_priority < prioritylvl && readyQ[i].process_priority >0) {
                priorityPos = i;
                prioritylvl = readyQ[i].process_priority;
                ans = readyQ[i];
            }

        }
        ans.execution_starttime = timestamp;
        ans.execution_endtime = timestamp + ans.remaining_bursttime;

        for (i = priorityPos; i < *queue_cnt - 1; i++) {
            readyQ[i] = readyQ[i + 1];
        }

        readyQ[*queue_cnt - 1] = NULLPCB;
        *queue_cnt -= 1;
        return ans;
    }

}



struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, struct PCB current_process,
    struct PCB new_process, int time_stamp)
{
    if (current_process.process_id == 0 && current_process.arrival_timestamp == 0 && current_process.execution_endtime == 0
        && current_process.execution_starttime == 0 && current_process.process_priority == 0 && current_process.remaining_bursttime == 0
        && current_process.total_bursttime == 0)
    {
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }

    else {
        if (current_process.remaining_bursttime < new_process.total_bursttime) {
            new_process.execution_endtime = 0;
            new_process.execution_starttime = 0;
            new_process.remaining_bursttime = new_process.total_bursttime;
            ready_queue[*queue_cnt] = new_process;
            *queue_cnt += 1;
            return current_process;
        }
        else {
            current_process.execution_starttime = 0;
            current_process.execution_endtime = 0;
            current_process.remaining_bursttime -= 1;
            ready_queue[*queue_cnt] = current_process;
            *queue_cnt += 1;
            new_process.execution_starttime = time_stamp;
            new_process.execution_endtime = time_stamp + new_process.total_bursttime;
            new_process.remaining_bursttime = new_process.total_bursttime;
            return new_process;
        }

    }

}


struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, int timestamp) {
    if (*queue_cnt == 0)
        return NULLPCB;

    else {
        int quIndex, i, lowestBurst = 99999;
        for (i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].remaining_bursttime < lowestBurst) {
                lowestBurst = ready_queue[i].remaining_bursttime;
                quIndex = i;
            }

        }
        struct PCB ans = ready_queue[quIndex];
        for (i = quIndex; i < *queue_cnt - 1; i++) {
            ready_queue[i] = ready_queue[i + 1];
        }
        ready_queue[*queue_cnt - 1] = NULLPCB;
        ans.execution_starttime = timestamp;
        ans.execution_endtime = timestamp + ans.remaining_bursttime;
        *queue_cnt -= 1;
        return ans;
    }
}


struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, struct PCB current_process,
    struct PCB new_process, int timestamp, int time_quantum) {
    if (current_process.process_id == 0 && current_process.arrival_timestamp == 0 && current_process.execution_endtime == 0
        && current_process.execution_starttime == 0 && current_process.process_priority == 0 && current_process.remaining_bursttime == 0
        && current_process.total_bursttime == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + min(new_process.total_bursttime, time_quantum);
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    else {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;
        return current_process;
    }
}


struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, int time_stamp, int time_quantum) {
    if (*queue_cnt == 0)
        return NULLPCB;
    else {
        int arrivTime = 99999, quIndex, i;
        for (i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].arrival_timestamp < arrivTime) {
                arrivTime = ready_queue[i].arrival_timestamp;
                quIndex = i;
            }
        }
        struct PCB ans = ready_queue[quIndex];
        for (i = quIndex; i < *queue_cnt - 1; i++) {
            ready_queue[i] = ready_queue[i + 1];
        }
        ready_queue[*queue_cnt - 1] = NULLPCB;
        ans.execution_starttime = time_stamp;
        *queue_cnt -= 1;
        ans.execution_endtime = time_stamp + min(time_quantum, ans.remaining_bursttime);
        return ans;
    }
}