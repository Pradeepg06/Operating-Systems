#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 2
#define MAX_ITERATIONS 3  // Number of times each philosopher eats

pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_t philosophers[NUM_PHILOSOPHERS];

void think(int id) {
    printf("Philosopher %d is thinking.\n", id);
    sleep(rand() % 3 + 1);  // Random think time
}

void eat(int id) {
    printf("Philosopher %d is eating.\n", id);
    sleep(rand() % 2 + 1);  // Random eat time
}

void* philosopher(void* num) {
    int id = *(int*)num;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    // Strategy to avoid deadlock: pick lower-numbered fork first
    int first = left < right ? left : right;
    int second = left < right ? right : left;

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        think(id);

        printf("Philosopher %d is hungry.\n", id);

        pthread_mutex_lock(&forks[first]);
        pthread_mutex_lock(&forks[second]);

        eat(id);

        pthread_mutex_unlock(&forks[second]);
        pthread_mutex_unlock(&forks[first]);
    }

    printf("Philosopher %d is done with dinner.\n", id);
    return NULL;
}

int main() {
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all philosophers to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("All philosophers are done with dinner.\n");
    return 0;
}
