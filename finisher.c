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

#include "lib/shared_memory.h"
#include "lib/argument_parser.h"


int main(int argc, char *argv[]) {
    int shmid = *parse_killer(argc, argv);

    // Attaches the shared memory segment associated with the shared
    // memory identifier shmid to the data segment of the calling
    // process.
    // https://www.mkssoftware.com/docs/man3/shmat.3.asp
    buffer_t* buffer = (buffer_t*) shmat(shmid, NULL, 0);

    if (buffer == (void *) -1) {
      printf("Error: Shared memory attach");
      exit(1);
    }

    // Detach and destroy shared memory for buffer
    shmdt(buffer);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Buffer was deleted.\n");

    return 0; 
}
