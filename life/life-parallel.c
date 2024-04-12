#define _XOPEN_SOURCE 700

#include "life.h"
#include <pthread.h>
#include <stdlib.h>

typedef struct
{
    int start;
    int end;
    int steps;
    LifeBoard *nextState;
    LifeBoard *curState;
    pthread_barrier_t *barrier;
    int threadID;
} Threads;

void *threadLife(void *info)
{
    Threads *threadInfo = (Threads *)info;

    for (int step = 0; step < threadInfo->steps; step += 1)
    {

        /* We use the range [1, width - 1) here instead of
         * [0, width) because we fix the edges to be all 0s.
         */
        for (int y = threadInfo->start; y < threadInfo->end; y += 1)
        {
            // possible issue on line below
            for (int x = 1; x < threadInfo->curState->width - 1; x += 1)
            {

                /* For each cell, examine a 3x3 "window" of cells around it,
                 * and count the number of live (true) cells in the window. */
                int live_in_window = 0;
                for (int y_offset = -1; y_offset <= 1; y_offset += 1)
                {
                    for (int x_offset = -1; x_offset <= 1; x_offset += 1)
                    {
                        // possible issue on line below
                        if (x + x_offset <= 0 || y + y_offset <= 0 || x + x_offset >= threadInfo->curState->width || y + y_offset >= threadInfo->curState->height)
                        {
                            continue;
                        }
                        if (LB_get(threadInfo->curState, x + x_offset, y + y_offset))
                        {
                            live_in_window += 1;
                        }
                    }
                }
                /* Cells with 3 live neighbors remain or become live.
                   Live cells with 2 live neighbors remain live. */
                if (x <= 0 || y <= 0 || x >= threadInfo->curState->width || y >= threadInfo->curState->height)
                {
                    continue;
                }
                LB_set(threadInfo->nextState, x, y,
                       live_in_window == 3 /* dead cell with 3 neighbors or live cell with 2 */ ||
                           (live_in_window == 4 && LB_get(threadInfo->curState, x, y)) /* live cell with 3 neighbors */
                );
            }
        }
        pthread_barrier_wait(threadInfo->barrier);
        /* now that we computed next_state, make it the current state */
        if(threadInfo->threadID == 0)
        LB_swap(threadInfo->curState, threadInfo->nextState);
        // LB_display(threadInfo->nextState,stdout);
        pthread_barrier_wait(threadInfo->barrier);
    }
    return NULL;
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps)
{
    // pthread_t ThreadList[threads];
    // Threads ThreadArg[threads];
    /* YOUR CODE HERE */
    LifeBoard *next_state = LB_new(state->width, state->height);
    pthread_barrier_t barrier1;
    pthread_t *ThreadList = (pthread_t *)malloc(threads * sizeof(pthread_t));
    Threads *ThreadArg = (Threads *)malloc(threads * sizeof(Threads));

    int sectionHeight = ((state->height-2) / threads);
    for (int i = 0; i < threads; i++)
    {
        ThreadArg[i].start = sectionHeight * i;
        ThreadArg[i].end = sectionHeight * (i + 1);
        if (i == (threads - 1))
        {
            ThreadArg[i].end = state->height - 1;
        }

        ThreadArg[i].steps = steps;
        ThreadArg[i].nextState = next_state;
        ThreadArg[i].curState = state;
        ThreadArg[i].threadID = i;
        ThreadArg[i].barrier = &barrier1;

        pthread_barrier_init(&barrier1, NULL, threads);

        // // Create thread
        // if (pthread_create(&ThreadList[i], NULL, threadLife, &ThreadArg[i]) != 0) {
        //     fprintf(stderr, "Error creating thread %d\n", i);
        //     exit(1);
        // }
        pthread_create(&ThreadList[i], NULL, threadLife, &ThreadArg[i]);
    }
    for (int i = 0; i < threads; i++)
    {
        pthread_join(ThreadList[i], NULL);
    }

    LB_del(next_state);
    pthread_barrier_destroy(&barrier1);
    free(ThreadList);
    free(ThreadArg);
}
