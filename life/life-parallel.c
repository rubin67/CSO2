#define _XOPEN_SOURCE 700

#include "life.h"
#include <pthread.h>
#include <stdlib.h>

pthread_barrier_t barrier1;

typedef struct
{
    int start;
    int end;
    int steps;
    LifeBoard *nextState;
    LifeBoard *curState;
    pthread_barrier_t *barrier;
} Threads;

void *threadLife(void *info)
{
    Threads *threadInfo = (Threads *)info;

    for (int step = 0; step < threadInfo->steps; step += 1)
    {

        /* We use the range [1, width - 1) here instead of
         * [0, width) because we fix the edges to be all 0s.
         */
        for (int y = 1; y < threadInfo[step].curState->height - 1; y += 1)
        {
            for (int x = 1; x < threadInfo[step].curState->width - 1; x += 1)
            {

                /* For each cell, examine a 3x3 "window" of cells around it,
                 * and count the number of live (true) cells in the window. */
                int live_in_window = 0;
                for (int y_offset = -1; y_offset <= 1; y_offset += 1)
                    for (int x_offset = -1; x_offset <= 1; x_offset += 1)
                        if (LB_get(threadInfo[step].curState, x + x_offset, y + y_offset))
                            live_in_window += 1;

                /* Cells with 3 live neighbors remain or become live.
                   Live cells with 2 live neighbors remain live. */
                LB_set(threadInfo[step].nextState, x, y,
                       live_in_window == 3 /* dead cell with 3 neighbors or live cell with 2 */ ||
                           (live_in_window == 4 && LB_get(threadInfo[step].curState, x, y)) /* live cell with 3 neighbors */
                );
            }
        }
        pthread_barrier_wait(&barrier1);
        /* now that we computed next_state, make it the current state */
        LifeBoard *temp = threadInfo[step].curState;
        threadInfo[step].curState = threadInfo[step].nextState;
        threadInfo[step].nextState = temp;
    }
    return NULL;
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps)
{
    /* YOUR CODE HERE */
    LifeBoard *next_state = LB_new(state->width, state->height);

    pthread_t ThreadList[threads];
    Threads ThreadArg[threads];

    int index = 0;
    // column/threads per thread  0 to value
    for (int i = 0; i < threads; i++)
    {
        ThreadArg[i].start = index;
        index += (state->height) / threads;
        ThreadArg[i].end = index;
        index++;
        ThreadArg[i].steps = steps;
        ThreadArg[i].nextState = next_state;
        ThreadArg[i].curState = state;

        ThreadArg[i].barrier = &barrier1;
        pthread_barrier_init(&barrier1, NULL, threads);

        pthread_create(&ThreadList[i], NULL, threadLife, &ThreadArg[i]);
    }
    for (int i = 0; i < threads; i++)
    {
        pthread_join(ThreadList[i], NULL);
    }
    if (steps % 2 != 0)
    {
        LB_swap(state, next_state);
    }

    LB_del(next_state);
    pthread_barrier_destroy(&barrier1);
}
