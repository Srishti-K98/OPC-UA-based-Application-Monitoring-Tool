#include "latency.h" 

void processCommand(const char* command, char* ipAddress); 

int main(int argc, char* argv[]) 
{ 
    if (argc < 2)  
    { 
        printf("Invalid number of arguments.\n"); 
        printf("Usage: %s start server\n", argv[0]); 
        printf("       %s start client <ip_address>\n", argv[0]); 
        return 1; 
    } 
    if (strcmp(argv[1], "start") == 0)  
    { 
        if (argc < 3)  
        { 
            printf("Invalid number of arguments.\n"); 
            printf("Usage: %s start server\n", argv[0]); 
            printf("       %s start client <ip_address>\n", argv[0]); 
            return 1; 
        } 
        if (strcmp(argv[2], "server") == 0)  
        { 
            startserver();                                                                        //start server                                                    
        }  
        else if (strcmp(argv[2], "client") == 0)  
        { 
            if (argc < 4)  
            { 
                printf("Invalid number of arguments.\n"); 
                printf("Usage: %s start client <address>\n", argv[0]); 
                return 1; 
            } 
            char* address = strstr(argv[3], "://"); 
            if (address != NULL)  
            { 
                address += 3;  
            }  
            else  
            { 
                address = argv[3]; 
            } 
            startclient(argc, argv);                                                              //start client                                
        } 
        else  
        { 
            printf("Invalid command. Type '%s help' to see available commands.\n", argv[0]); 
            return 1; 
        } 
    }
    else if (strcmp(argv[1], "help") == 0)  
    { 
        printf("Usage: %s start server\n", argv[0]); 
        printf("       %s start client <ip_address>\n", argv[0]); 
        printf("       %s results\n", argv[0]); 
        printf("       %s help\n", argv[0]); 
    }  
    else  
    { 
        printf("Unknown command. Type '%s help' to see available commands.\n", argv[0]); 
        return 1; 
    }
    return 0; 
} 
