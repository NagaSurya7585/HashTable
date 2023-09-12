#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
 
#define QUEUE_SIZE 10                   
#define NUM_receiverS 5                 
#define NUM_MESSAGES 10                 
#define MESSAGE_INTERVAL_US 200000      

// Structure to represent the shared queue
typedef struct {
    char* messages[QUEUE_SIZE];
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
    int sender; 
} SharedQueue;




// Function to initialize the shared queue
void initQueue(SharedQueue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
    queue->sender = 0; 
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
    pthread_cond_init(&queue->not_full, NULL);
}



// Function to add a message to the queue
void add_Mssg(SharedQueue* queue, const char* message) {
    pthread_mutex_lock(&queue->mutex);
    

    while (queue->count == QUEUE_SIZE || queue->sender) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }

    if (!queue->sender) { 
        queue->rear = (queue->rear + 1) % QUEUE_SIZE;
        queue->messages[queue->rear] = strdup(message);
        queue->count++;

      
        pthread_cond_signal(&queue->not_empty);
    }

    pthread_mutex_unlock(&queue->mutex);
}


// Function to remove and return a message from the queue
char* remove_Mssg(SharedQueue* queue) {
    pthread_mutex_lock(&queue->mutex);

    
    while (queue->count == 0 && !queue->sender) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }

    char* message = NULL;
    if (queue->count > 0) {
        message = queue->messages[queue->front];
        queue->front = (queue->front + 1) % QUEUE_SIZE;
        queue->count--;

        
        pthread_cond_signal(&queue->not_full);
    }

    pthread_mutex_unlock(&queue->mutex);

    return message;
}



// Function to simulate the sender adding messages
void* sender(void* arg) {
    SharedQueue* queue = (SharedQueue*)arg;
    for (int i = 0; i < NUM_MESSAGES; i++) {
        char message[20];
        snprintf(message, sizeof(message), "Message %d", i + 1);
        add_Mssg(queue, message);
        usleep(MESSAGE_INTERVAL_US);
    }

    
    pthread_mutex_lock(&queue->mutex);
    queue->sender = 1;
    pthread_mutex_unlock(&queue->mutex);


    pthread_cond_broadcast(&queue->not_empty);

    pthread_exit(NULL);
}



// Function to simulate the receivers consuming messages
void* receiver(void* arg) {
    SharedQueue* queue = (SharedQueue*)arg;
    while (1) {
        char* message = remove_Mssg(queue);
        if (!message) {
         
            break;
        }
        printf("receiver %ld received: %s\n", pthread_self(), message);
        free(message);
    }
    pthread_exit(NULL);
}


int main()
{
    SharedQueue queue;
    initQueue(&queue);

    pthread_t senderThread;
    pthread_create(&senderThread, NULL, sender, &queue);

    pthread_t receiverThreads[NUM_receiverS];
    for (int i = 0; i < NUM_receiverS; i++) {
        pthread_create(&receiverThreads[i], NULL, receiver, &queue);
    }

    pthread_join(senderThread, NULL);
    for (int i = 0; i < NUM_receiverS; i++) {
        pthread_join(receiverThreads[i], NULL);
    }

    pthread_mutex_destroy(&queue.mutex);
    pthread_cond_destroy(&queue.not_empty);
    pthread_cond_destroy(&queue.not_full);

    return 0;
}

// main initializes the shared queue, creates a sender thread, and multiple receiver threads. It joins the threads after they finish and performs cleanup by destroying mutexes and condition variables.
