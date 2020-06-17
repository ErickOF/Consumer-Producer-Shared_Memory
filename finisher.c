// The <sys/ipc.h> header is used by three mechanisms for interprocess
// communication (IPC): messages, semaphores and shared memory. All use
// a common structure type, ipc_perm to pass information used in
// determining permission to perform an IPC operation.
// https://pubs.opengroup.org/onlinepubs/007908799/xsh/sysipc.h.html
#include <sys/ipc.h>
// XSI shared memory facility.
// https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/shm.h.html
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


int main() {
    // Test data
    char* buffer_name = "Ericks buffer\0";

    int data_name_size = strlen(buffer_name) + 5;
    char* data_name = (char*) calloc(data_name_size, sizeof(char));
    strcpy(data_name, buffer_name);
    strcat(data_name, "_data\0");

    short flags = 0644;

    // Generate a key based on buffer name
    key_t key = (key_t) generate_uid(buffer_name);

    // Returns the shared memory identifier associated with key.
    // int shmget(key_t key, size_t size, int shmflg);
    // https://www.mkssoftware.com/docs/man3/shmget.3.asp
    int shmid = shmget(key, sizeof(buffer_t), flags | IPC_CREAT);

    if (shmid == -1) {
        printf("Error: shmget\n");
        exit(1);
    }

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    buffer_t* buffer = (buffer_t*) shmat(shmid, NULL, 0);

    if (buffer == (void *) -1) {
      printf("Error: Shared memory attach");
      exit(1);
    }

    char* data = (char*) shmat(shmid, NULL, 0);

    // Detach and destroy shared memory for data
    shmdt(data);
    shmctl(buffer->id_data, IPC_RMID, NULL);

    // Detach and destroy shared memory for data
    shmdt(buffer);
    shmctl(shmid, IPC_RMID, NULL);

    // Free memory after be used
    free(data_name);

    return 0; 
}
