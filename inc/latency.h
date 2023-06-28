//headers for client.c 
#include <open62541/client_config_default.h> 
#include <open62541/client_highlevel.h> 

//headers for server.c 
#include <open62541/server.h> 
#include <open62541/server_config_default.h> 
#include <open62541/plugin/log_stdout.h> 
#include <signal.h> 

//headers for latency.c and cli.c 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <time.h> 
#include <string.h>

struct timespec serverConnectionTime, clientConnectionTime, latency; 
UA_Server* server = NULL;     
UA_Client* client = NULL;     
UA_Variant value;  
struct timespec ts;

/*UA_Boolean running = true; 
void signalHandler(int sig) 
{ 
    running = false; 
} 
*/
int shouldTerminate = 0;

// Signal handler function
void signalHandler(int signum) {
    if (signum == SIGINT) {
        printf("Received interrupt signal. Terminating...\n");
        shouldTerminate = 1;
    }
}

FILE *file;

//function declarations 
int startserver(); 
int startclient(int argc, char *argv[]); 
void calculateLatency();