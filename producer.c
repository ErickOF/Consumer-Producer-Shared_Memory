#ifndef PROYECTO1_PRODUCER_H
#define PROYECTO1_PRODUCER_H


// C program for finding the largest integer 
// among three numbers using command line arguments 
#include <stdio.h> 
#include <unistd.h>

#include "lib/argument_parser.h"
#include "lib/semaphores.h"

// Taking argument as command line 
int main(int argc, char *argv[]) 
{ 
    // Get the attachment parametters
    // [0]:shmid [1]:avg time sleep in seconds
    int* parameters = parse_producer(argc, argv);
    // Stats variables
    unsigned int num_messages = 0;
    double acc_waiting_time = 0;
    double acc_sem_waiting_time = 0;
    double acc_kernel_time = 0;
    // Producer control and identification
    short isSending = TRUE;
    short self_id;

    // Shared memory buffer
    buffer_t* buffer;
    // Get the semaphores used to access the data
    // semaphores[0]: mutex, 
    // semaphores[1]: empty_spaces, 
    // semaphores[2]: available_msgs
    sem_t* semaphores;

    // Initialize buffer, semaphores and client id
    start_client(*parameters, TRUE, buffer, semaphores, &self_id);

    // Only send when the buffer is active
    while(isSending){
        // Wait for empty spaces
        sem_wait(semaphores + 1);
        // Wait for mutex
        sem_wait(semaphores);

        //Check if the buffer is still active
        isSending = buffer->isActive;

        // If it is then send the msg
        if(isSending){

            // TODO : BUILD MESSAGES CORRECTLY
            // Create msg
            message_t msg = {.producer_id=self_id, .data=self_id, 
                            .date=10, .time=11};
            // Send the msg
            send_msg(msg, buffer);
            // Increment the counter
            ++num_messages;

        }
        // If the buffer is down 
        else{
            // Decrease the producer counter
            --(buffer->producers);
            printf("Setected unactive buffer, detaching...\n");

        }

        // Release mutex
        sem_post(semaphores);
        // Update available
        sem_post(semaphores + 2);

        // Sleep time 
        sleep(*(parameters + 1));


    }

    // Detach from shared memory
    shmdt(buffer);
    // TODO HANDLE PRINTS OVER HERE
    printf("Detached...\n");

} 




#endif  // PROYECTO1_PRODUCER_H