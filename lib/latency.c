#include "latency.h" 

UA_Boolean running = true;
UA_Boolean terminateRequested = false;

void signalHandler(int sig) 
{
    running = false;
    terminateRequested = true;
}


static UA_StatusCode
helloWorldMethodCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) 
{
    UA_String outputStr = UA_STRING_NULL;
    
    // Get the current time using clock_gettime
    struct timespec currentTime;
    if (clock_gettime(CLOCK_REALTIME, &currentTime) == 0) {
        char timestamp[30];
        snprintf(timestamp, sizeof(timestamp), "%lld%09ld",
                 (long long)currentTime.tv_sec, currentTime.tv_nsec);
        outputStr = UA_STRING_ALLOC(timestamp);
    } else {
        // Failed to get the current time
        outputStr = UA_STRING_ALLOC("Error: Failed to get current time");
    }
    
    UA_Variant_setScalarCopy(output, &outputStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_String_clear(&outputStr);
    
    //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Current time was called");
    
    return UA_STATUSCODE_GOOD;
}

static void
addHelloWorldMethod(UA_Server *server) 
{
    UA_Argument inputArgument;
    UA_Argument_init(&inputArgument);
    inputArgument.description = UA_LOCALIZEDTEXT("en-US", "Empty");
    inputArgument.name = UA_STRING("MyInput");
    inputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    inputArgument.valueRank = UA_VALUERANK_SCALAR;
    // Set inputArgument to NULL since no input is required
    inputArgument.arrayDimensionsSize = 0;
    inputArgument.arrayDimensions = NULL;

    UA_Argument outputArgument;
    UA_Argument_init(&outputArgument);
    outputArgument.description = UA_LOCALIZEDTEXT("en-US", "A String");
    outputArgument.name = UA_STRING("MyOutput");
    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
    outputArgument.valueRank = UA_VALUERANK_SCALAR;

    UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
    helloAttr.description = UA_LOCALIZEDTEXT("en-US","Say `Current time`");
    helloAttr.displayName = UA_LOCALIZEDTEXT("en-US","Current Time");
    helloAttr.executable = true;
    helloAttr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_NUMERIC(1,62541),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(1, "current t ime"),
                            helloAttr, &helloWorldMethodCallback,
                            0, NULL, 1, &outputArgument, NULL, NULL);
}

int startserver() 
{ 
    signal(SIGINT, signalHandler); 

    server = UA_Server_new();
    addHelloWorldMethod(server);

    UA_StatusCode retval = UA_Server_runUntilInterrupt(server);
    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE; 
} 

int startclient(char *argv[]) 
{ 
    UA_Boolean running = true;

    const char* serverAddress = argv[3];
    client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));

    // Connect to the server
    UA_StatusCode retval = UA_Client_connect(client, serverAddress);
    if (retval != UA_STATUSCODE_GOOD) {
        printf("Failed to connect to the server\n");
        UA_Client_delete(client);
        return -1;
    }

    size_t outputSize;
    char timestamp[30];
    UA_Variant *ptr_output;
    struct timespec clientTime;

    // Create the method call in a loop to calculate latency
    while (running && !terminateRequested)
    {
        sleep(1);
        file = fopen("MyOutput.csv", "a"); 
    if (file == NULL) 
    {
        printf("Error opening the file.\n");
        return 1;
    }
        clock_gettime(CLOCK_REALTIME, &clientTime);

        UA_StatusCode retval = UA_Client_call(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                              UA_NODEID_NUMERIC(1, 62541), 0, NULL, &outputSize, &ptr_output);

        printf("Client method call time: ");
        printf("%ld%09ld\n", clientTime.tv_sec, clientTime.tv_nsec);

        if (retval == UA_STATUSCODE_GOOD) {
            for (size_t i = 0; i < outputSize; i++) {
                UA_String* uas = (UA_String*)ptr_output[i].data;
                memcpy(&timestamp, uas->data, uas->length);
            }

            int64_t serverTimeInt;
            char c;
            int scanned = sscanf(&timestamp, "%" SCNd64 "%c", &serverTimeInt, &c);
            printf("serverTimeInt: ");
            printf("%" PRId64 "\n", serverTimeInt);

            int64_t clientTimeInt = clientTime.tv_sec * 1000000000 + clientTime.tv_nsec;
            printf("clientTimeInt: ");
            printf("%" PRId64 "\n", clientTimeInt);

            int64_t latency = serverTimeInt - clientTimeInt;
            printf("latency: ");
            printf("%" PRId64 "\n", latency);

            clock_gettime(CLOCK_REALTIME, &ts);
    
            time_t current_time = ts.tv_sec;  // Get the current time in seconds
            struct tm* timeinfo;
            timeinfo = localtime(&current_time);

            // Create a char array to store the ISO 8601 formatted time
            char time_string[21]; 

            // Use strftime to format the time according to ISO 8601
            strftime(time_string, sizeof(time_string), "%Y-%m-%dT%H:%M:%SZ", timeinfo);
            
            fprintf(file, "%s,", time_string);
            fprintf(file, "%" PRId64 "\n", latency);

            fclose(file);   
        }
        else 
        {
            printf("Method call was unsuccessful, and %x returned values available.\n", retval);
        }
    }
    UA_Client_disconnect(client);
    UA_Client_delete(client);
    return 0;
}