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

UA_Server* server = NULL;     
UA_Client* client = NULL;     
UA_Variant value;  
struct timespec ts;
char* address;

FILE *file;

//function declarations 
int startserver();
int startclient(char *argv[]); 