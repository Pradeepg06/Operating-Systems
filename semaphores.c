#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // for sleep()

#define BUFFER_SIZE 5 // size of the circular buffer
#define PRODUCE_LIMIT 20 // total items to produce (for demonstration)

// Shared buffer and associated indices for circular buffer
int buffer[BUFFER_SIZE];
int in = 0; // points to the next free slot for the producer
int out = 0; // points to the next full slot for the consumer

// Semaphores
sem_t empty; // counts empty slots in the buffer
sem_t full; // counts full slots in the buffer

// Mutex to protect buffer access (critical section)
pthread_mutex_t mutex;

void* producer(void* arg) {
    int item;
    for (int i = 0; i < PRODUCE_LIMIT; i++) {
        // Produce an item (here we simply use the loop index)
        item = i;

        // Wait if there are no empty slots
        sem_wait(&empty);

        // Lock the critical section before writing to the buffer
        pthread_mutex_lock(&mutex);

        // Place item in buffer and update index using modulo arithmetic
        buffer[in] = item;
        printf("Producer: produced item %d at index %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        // Unlock the critical section
        pthread_mutex_unlock(&mutex);

        // Signal that there is a new full slot in the buffer
        sem_post(&full);

        sleep(1); // Simulate time taken to produce the item
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int item;
    for (int i = 0; i < PRODUCE_LIMIT; i++) {
        // Wait until there is an item to consume
        sem_wait(&full);

        // Lock the critical section before reading from the buffer
        pthread_mutex_lock(&mutex);

        // Remove an item from the buffer and update index
        item = buffer[out];
        printf("Consumer: consumed item %d from index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        // Unlock the critical section
        pthread_mutex_unlock(&mutex);

        // Signal that there is a new empty slot in the buffer
        sem_post(&empty);

        sleep(2); // Simulate time taken to process the consumed item
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t tid_producer, tid_consumer;

    // Initialize the semaphores:
    // 'empty' is initially set to BUFFER_SIZE (all slots empty)
    // 'full' is initially set to 0 (no full slots)
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Create the producer and consumer threads
    pthread_create(&tid_producer, NULL, producer, NULL);
    pthread_create(&tid_consumer, NULL, consumer, NULL);

    // Wait for both threads to finish
    pthread_join(tid_producer, NULL);
    pthread_join(tid_consumer, NULL);

    // Clean up resources
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("All items produced and consumed.\n");
    return 0;
}
