#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

void* producer(void* arg) {
    int item;
    for (int i = 0; i < 10; i++) {
        item = rand() % 100;  // produce an item
        sem_wait(&empty);     // decrease empty count
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer produced: %d at %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);      // increase full count

        sleep(1);             // simulate production time
    }
    return NULL;
}

void* consumer(void* arg) {
    int item;
    for (int i = 0; i < 10; i++) {
        sem_wait(&full);      // wait until full > 0
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        printf("Consumer consumed: %d from %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);     // increase empty count

        sleep(2);             // simulate consumption time
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}











### Key Concepts:
1. **Producer-Consumer Problem**:
   - The **producer** generates data (in this case, random integers) and places it in a buffer.
   - The **consumer** takes the data from the buffer and processes it.
   - The challenge is to synchronize the producer and consumer such that:
     - The producer doesn't add items to a full buffer.
     - The consumer doesn't attempt to consume from an empty buffer.
   
2. **Semaphores**:
   - Semaphores are used to control access to the buffer by the producer and consumer, ensuring synchronization between them.
   - Two semaphores are used:
     - `empty`: Tracks the number of empty spaces in the buffer (initialized to `BUFFER_SIZE`).
     - `full`: Tracks the number of filled items in the buffer (initialized to `0`).
   - The producer will **wait** (decrement) on the `empty` semaphore to ensure there is space in the buffer before producing, and then **signal** (increment) the `full` semaphore to indicate that the buffer has a new item.
   - The consumer will **wait** on the `full` semaphore to ensure there is something in the buffer to consume, and then **signal** the `empty` semaphore to indicate that the buffer now has an empty space.

3. **Mutexes**:
   - A **mutex** (short for "mutual exclusion") is used to ensure that only one thread (either the producer or consumer) accesses the buffer at a time, preventing data races when writing or reading from the buffer.
   - `pthread_mutex_lock()` is used to lock the mutex, and `pthread_mutex_unlock()` is used to unlock it. These operations prevent other threads from modifying the buffer simultaneously.

### Code Breakdown:

#### Global Variables:
- `buffer`: An array representing the shared buffer with a size defined by `BUFFER_SIZE`.
- `in`, `out`: Indices to keep track of where the producer should insert an item (`in`) and where the consumer should remove an item (`out`).
- `empty`: A semaphore initialized to `BUFFER_SIZE`, which represents the number of empty spaces in the buffer.
- `full`: A semaphore initialized to `0`, representing the number of items in the buffer.
- `mutex`: A mutex used to synchronize access to the shared buffer.

#### Producer Thread:
The `producer` function:
- It loops 10 times (producing 10 items).
- It produces a random item (an integer between 0 and 99).
- It waits on the `empty` semaphore to ensure there is space in the buffer.
- It locks the mutex to safely write to the buffer.
- It inserts the item into the buffer at the position indicated by `in`, prints a message, and then updates `in` (circularly, i.e., `(in + 1) % BUFFER_SIZE`).
- After inserting the item, it unlocks the mutex and signals the `full` semaphore to indicate that an item has been added to the buffer.
- Finally, it sleeps for 1 second to simulate the time it takes to produce an item.

#### Consumer Thread:
The `consumer` function:
- It loops 10 times (consuming 10 items).
- It waits on the `full` semaphore to ensure there is something to consume.
- It locks the mutex to safely read from the buffer.
- It consumes an item from the buffer at the position indicated by `out`, prints a message, and then updates `out` (circularly, i.e., `(out + 1) % BUFFER_SIZE`).
- After consuming the item, it unlocks the mutex and signals the `empty` semaphore to indicate that a space has become available in the buffer.
- Finally, it sleeps for 2 seconds to simulate the time it takes to consume an item.

#### Main Function:
- **Semaphores Initialization**: `sem_init(&empty, 0, BUFFER_SIZE)` initializes the `empty` semaphore with the value `BUFFER_SIZE` (i.e., there are initially `BUFFER_SIZE` empty slots in the buffer). `sem_init(&full, 0, 0)` initializes the `full` semaphore with value `0` (i.e., initially, the buffer is empty).
- **Mutex Initialization**: `pthread_mutex_init(&mutex, NULL)` initializes the mutex for protecting the shared buffer.
- **Thread Creation**: `pthread_create()` is used to create two threads: `prod` (producer) and `cons` (consumer).
- **Thread Joining**: `pthread_join()` waits for both threads to finish before the program continues.
- **Cleanup**: After the threads have finished, semaphores and the mutex are destroyed using `sem_destroy()` and `pthread_mutex_destroy()`.

### Thread Safety and Synchronization:
- **Semaphores** are used to manage the number of items in the buffer (through `empty` and `full`).
- The **mutex** ensures that only one thread (either producer or consumer) modifies the buffer at any given time, preventing race conditions and ensuring data integrity.
- The `sleep()` calls simulate the production and consumption times, allowing us to observe the interleaving of operations between the producer and consumer.

### Execution Flow:
1. The producer starts producing items and placing them into the buffer.
2. The consumer starts consuming items from the buffer.
3. The producer and consumer synchronize their actions using semaphores and the mutex.
4. The program continues until both threads have completed their work, with the producer producing 10 items and the consumer consuming 10 items.
