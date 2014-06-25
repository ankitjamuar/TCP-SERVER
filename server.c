/*
     Name: C socket server
     Author: Ankit Sinha
     Description:  Handles multiple clients using threads.
     Modified Date: 25-06-2014
     Created on: 21-06-2014
     Compile command:  gcc server.c -lpthread -o server -std=c99
*/
 
#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread


//TO save socket descriptior of incoming client
    int _client[50];
    int c_no = 0;
    int _TAGPLUG = 0;
 
//the thread function
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;

         
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 2626 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 5);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;
  
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {

        _client[c_no] = client_sock;
        c_no++;

        char *client_ip = inet_ntoa(client.sin_addr);

        puts("Connection accepted");
         
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    int my_c_no = c_no;
     
    //Send some messages to the client
    message = "_LOGIN_START";
    write(sock , message , strlen(message));
     
    //printf("%d\n",c_no );
    printf("%d\n",sock );

     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //end of string marker
    client_message[read_size] = '\0';

     //if(strstr(client_message, "#") != NULL){
            
        
             if( (strcmp( client_message, "T")) == 0  ){
                //write(sock, client_message , strlen(client_message));

                _TAGPLUG = sock;
                message = "_OK@";
                write(sock, message , strlen(message));
                puts("Passord Validated");
            }

    // }

    if(_TAGPLUG != 0){
        write(_TAGPLUG, client_message , strlen(client_message));
        puts("Sent only to TagPlug");
    }


    
    //Send the message back to client
   // for(int z = 0; z< c_no; z++){
      //if(my_c_no != z){
         
      //}

   // }
      
    
    //clear the message buffer
    memset(client_message, 0, 2000);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
       // c_no--;
    }
    else if(read_size == -1)
    {
        perror("recv failed");
        printf("%d\n",sock );
    }
    //free(socket_desc);
         
    return 0;
} 