#include "oslabs.h"

int abs(int value) {
    if (value < 0)
        return value * -1;
    else
        return value;
}


struct RCB NULLRCB = {
    0,0,0,0,0
};


struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int* queue_cnt,
    struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.address == 0 && current_request.arrival_timestamp == 0 &&
        current_request.cylinder == 0 && current_request.process_id == 0 &&
        current_request.request_id == 0) {
        return new_request;
    }

    else {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
    return NULLRCB;
}


struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int* queue_cnt) {
    if (*queue_cnt <= 0) {
        return NULLRCB;
    }
    else {
        int i = 0, smallest = 9999, pos;

        for (i; i < *queue_cnt; i++) {
            if (request_queue[i].arrival_timestamp < smallest) {
                smallest = request_queue[i].arrival_timestamp;
                pos = i;
            }

        }
        struct RCB ans = request_queue[pos];
        for (i = pos; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        *queue_cnt -= 1;
        return ans;

    }

}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int* queue_cnt, struct RCB current_request,
    struct RCB new_request, int timestamp) {
    if (current_request.address == 0 && current_request.arrival_timestamp == 0 &&
        current_request.cylinder == 0 && current_request.process_id == 0 &&
        current_request.request_id == 0) {
        return new_request;
    }

    else {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }

}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int* queue_cnt, int current_cylinder) {
    if (*queue_cnt <= 0) {
        return NULLRCB;
    }

    else {
        struct RCB ans_queue[QUEUEMAX];

        int i = 0, closest = abs(current_cylinder - request_queue[0].cylinder), anscount = 0, location;

        for (i; i < *queue_cnt; i++) {
            if (abs(current_cylinder - request_queue[i].cylinder) < closest) {
                closest = current_cylinder - request_queue[i].cylinder;
                location = i;
            }
        }

        struct RCB answer;
        for (i = 0; i < *queue_cnt; i++) {
            if (abs(current_cylinder - request_queue[i].cylinder) == closest) {
                ans_queue[anscount] = request_queue[i];
                anscount += 1;
            }
        }

        int smallest = 9999, pos;
        if (anscount > 0) {
            for (i = 0; i < anscount; i++) {
                if (ans_queue[i].arrival_timestamp < smallest) {
                    smallest = ans_queue[i].arrival_timestamp;
                    pos = i;
                }
            }
            answer = ans_queue[pos];
        }

        else {
            answer = request_queue[location];
        }

        for (i = 0; i < *queue_cnt; i++) {
            if (answer.request_id == request_queue[i].request_id)
                pos = i;
        }

        for (i = pos; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        *queue_cnt -= 1;
        return answer;
    }
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],
    int* queue_cnt, int current_cylinder, int scan_direction) {
    if (*queue_cnt <= 0) {
        return NULLRCB;
    }
    else {
        struct RCB locyl[QUEUEMAX], hicyl[QUEUEMAX], eqcyl[QUEUEMAX];
        int i = 0, locnt = 0, hicnt = 0, eqcnt = 0;
        for (i; i < *queue_cnt; i++) {
            if (request_queue[i].cylinder == current_cylinder) {
                eqcyl[eqcnt] = request_queue[i];
                eqcnt += 1;
            }
            if (request_queue[i].cylinder < current_cylinder) {
                locyl[locnt] = request_queue[i];
                locnt += 1;
            }
            if (request_queue[i].cylinder > current_cylinder) {
                hicyl[hicnt] = request_queue[i];
                hicnt += 1;
            }
        }

        if (eqcnt > 0) {
            int smallest = 9999, pos;
            for (i = 0; i < eqcnt; i++) {
                if (eqcyl[i].arrival_timestamp < smallest) {
                    pos = i;
                    smallest = eqcyl[i].arrival_timestamp;
                }
            }
            struct RCB answer = eqcyl[pos];

            for (i = 0; i < *queue_cnt; i++) {
                if (answer.request_id == request_queue[i].request_id)
                    pos = i;
            }

            for (i = pos; i < *queue_cnt - 1; i++) {
                request_queue[i] = request_queue[i + 1];
            }
            *queue_cnt -= 1;
            return answer;
        }

        if (scan_direction == 1) {
            if (hicnt > 0) {
                int hiclosest = abs(current_cylinder - hicyl[0].cylinder), pos;
                for (i = 0; i < hicnt; i++) {
                    if (abs(hicyl[i].cylinder - current_cylinder) <= hiclosest) {
                        hiclosest = abs(hicyl[i].cylinder - current_cylinder);
                        pos = i;
                    }
                }
                int j, newpos;
                for (j = 0; j < *queue_cnt; j++) {
                    if (request_queue[j].request_id == hicyl[pos].request_id) {
                        newpos = j;
                    }
                }
                struct RCB answer = request_queue[newpos];

                for (newpos; newpos < *queue_cnt - 1; newpos++) {
                    request_queue[newpos] = request_queue[newpos + 1];
                }
                *queue_cnt -= 1;
                return answer;
            }
            else {

                int closest = abs(current_cylinder - request_queue[0].cylinder), pos;
                int k;
                for (k = 0; k < *queue_cnt; k++) {
                    if (abs(request_queue[k].cylinder - current_cylinder) < closest) {
                        closest = abs(request_queue[k].cylinder - current_cylinder);
                        pos = k;
                    }
                }

                struct RCB answer = request_queue[pos];
                for (k = pos; k < *queue_cnt - 1; k++) {
                    request_queue[k] = request_queue[k + 1];
                }
                *queue_cnt -= 1;
                return answer;

            }
        }
        if (scan_direction == 0) {
            if (locnt > 0) {
                int loclosest = abs(current_cylinder - locyl[0].cylinder), pos;
                for (i = 0; i < locnt; i++) {
                    if (abs(locyl[i].cylinder - current_cylinder) <= loclosest) {
                        loclosest = abs(locyl[i].cylinder - current_cylinder);
                        pos = i;
                    }
                }
                int j, newpos;
                for (j = 0; j < *queue_cnt; j++) {
                    if (request_queue[j].request_id == locyl[pos].request_id) {
                        newpos = j;
                    }
                }
                struct RCB answer = request_queue[newpos];

                for (newpos; newpos < *queue_cnt - 1; newpos++) {
                    request_queue[newpos] = request_queue[newpos + 1];
                }
                *queue_cnt -= 1;
                return answer;
            }
            else if (hicnt > 0) {
                int hiclosest = abs(current_cylinder - hicyl[0].cylinder), pos;
                for (i = 0; i < hicnt; i++) {
                    if (abs(hicyl[i].cylinder - current_cylinder) <= hiclosest) {
                        hiclosest = abs(hicyl[i].cylinder - current_cylinder);
                        pos = i;
                    }
                }
                int j, newpos;
                for (j = 0; j < *queue_cnt; j++) {
                    if (request_queue[j].request_id == hicyl[pos].request_id) {
                        newpos = j;
                    }
                }
                struct RCB answer = request_queue[newpos];

                for (newpos; newpos < *queue_cnt - 1; newpos++) {
                    request_queue[newpos] = request_queue[newpos + 1];
                }
                *queue_cnt -= 1;
                return answer;


            }

        }
    }
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int* queue_cnt, struct RCB current_request,
    struct RCB new_request, int timestamp) {
    if (current_request.address == 0 && current_request.arrival_timestamp == 0 &&
        current_request.cylinder == 0 && current_request.process_id == 0 &&
        current_request.request_id == 0) {
        return new_request;
    }

    else {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}