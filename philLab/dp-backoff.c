#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t philosopher[5];
pthread_mutex_t chopstick[5];

// Function to generate random number within a given range
int random_range(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

void *eat(void *arg) {
    int n = (int)(long)arg;
    int retries = 0;
    int base_backoff_time = 50; // Initial backoff time
    int backoff_multiplier = 2; // Multiplier for increasing backoff time

    while (retries < 3) { // Limit retries to avoid livelock
        // Attempt to acquire the first chopstick
        if (pthread_mutex_trylock(&chopstick[n]) == 0) {
            printf("Philosopher %d got chopstick %d\n", n, n);
            // Attempt to acquire the second chopstick
            if (pthread_mutex_trylock(&chopstick[(n + 1) % 5]) == 0) {
                printf("Philosopher %d got chopstick %d\n", n, (n + 1) % 5);
                printf("Philosopher %d is eating\n", n);
                sleep(1);
                // Release the chopsticks
                pthread_mutex_unlock(&chopstick[(n + 1) % 5]);
                printf("Philosopher %d set down chopstick %d\n", n, (n + 1) % 5);
            } else {
                printf("Philosopher %d failed to get chopstick %d\n", n, (n + 1) % 5);
                pthread_mutex_unlock(&chopstick[n]);
                retries++;
                int backoff_time = random_range(base_backoff_time, base_backoff_time * backoff_multiplier);
                usleep(backoff_time);
                base_backoff_time *= backoff_multiplier; // Increase backoff time
            }
        } else {
            printf("Philosopher %d failed to get chopstick %d\n", n, n);
            retries++;
            int backoff_time = random_range(base_backoff_time, base_backoff_time * backoff_multiplier);
            usleep(backoff_time);
            base_backoff_time *= backoff_multiplier; // Increase backoff time
        }
    }

    return NULL;
}

int main(int argc, const char *argv[]) {
    srand(time(NULL)); // Seed for random number generation

    // Initialize mutexes for chopsticks
    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&chopstick[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < 5; i++) {
        pthread_create(&philosopher[i], NULL, eat, (void *)(size_t)i);
    }

    // Join philosopher threads
    for (int i = 0; i < 5; i++) {
        pthread_join(philosopher[i], NULL);
    }

    // Destroy mutexes for chopsticks
    for (int i = 0; i < 5; i++) {
        pthread_mutex_destroy(&chopstick[i]);
    }

    return 0;
}
