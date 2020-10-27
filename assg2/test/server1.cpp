#include <bits/stdc++.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<sys/types.h> 
#include<sys/socket.h> 
#include<sys/un.h> 
#include<string.h> 
#include<netdb.h> 
#include<netinet/in.h> 
#include<arpa/inet.h> 
#include<string.h> 
#include<pthread.h>   

#define MAX_THREADS 15

using namespace std;
struct arg_struct {
    int sock;
    int pt;
};
queue< arg_struct > threadsss;

pthread_t tid[MAX_THREADS];
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;





void * handle_conn(void* args);
void * thread_init(void* args);

void * thread_init(void* args)
{
    while(true)
    { struct arg_struct client;
        pthread_mutex_lock(&mutex1);
         if(!threadsss.empty()){
        
        pthread_cond_wait(&condition_var,&mutex1);
        cout<<threadsss.empty()<<endl;
        cout<<"empty"<<endl;
         
         client=threadsss.front()
         pthread_mutex_unlock(&mutex1);
         }

         if(client)
         {cout<<client.pt<<endl;
         cout<<"front"<<endl;
         threadsss.pop();
          cout<<"pop"<<endl;
          cout<<"jjj"<<endl;
         handle_conn((void *)&client);}

          
    }
}

void * handle_conn(void* args)
{
    cout<<"conn"<<endl;
	struct arg_struct* sp = (struct arg_struct *)args;
	int client_socket=sp->sock;
    int port=sp->pt;
	// free (args);

	char buffer1[256], buffer2[256];
       
            while(1){recv(client_socket, buffer2, 256, 0); 
                       cout<<"Client :"<<port<<":"<<buffer2<<endl; 
                        strcpy(buffer1, "main zinda hu"); 
                        send(client_socket, buffer1, 256, 0); 
                        cout<<"Server :"<<buffer1<<endl;}
        pthread_exit(NULL);
        // close(client_socket);
}

int main(int argc, char const *argv[]) 
{ 
     if(argc!=3 )
     {
        cout<<"not enough args"<<endl;
        return -1;
     }

     string ipp=argv[1];
     char ip[ipp.length()+1];
     strcpy(ip, ipp.c_str());
     string portt=argv[2];
     int port=stoi(portt);
     
     for(int i=0;i<MAX_THREADS;)
     {
        pthread_create(&tid[i++], NULL, thread_init, NULL);
     }


    int server = socket(AF_INET, SOCK_STREAM, 0); 
    if (server < 0) 
        printf("Error in server creating\n"); 
    else
        printf("Server Created\n"); 
          
    struct sockaddr_in my_addr, peer_addr; 
    my_addr.sin_family = AF_INET; 
    my_addr.sin_addr.s_addr = INADDR_ANY; 
    my_addr.sin_addr.s_addr = inet_addr(ip); 
    my_addr.sin_port = htons(port); 
  
    if (bind(server, (struct sockaddr*) &my_addr, sizeof(my_addr)) == 0) 
        printf("Binded Correctly\n"); 
    else
        printf("Unable to bind\n"); 
          
    if (listen(server, 15) == 0) 
        printf("Listening ...\n"); 
    else
        printf("Unable to listen\n"); 
      
    socklen_t addr_size; 
    addr_size = sizeof(struct sockaddr_in); 
      
   
    
    int i=0;
    socklen_t addrlen;
    int newsock;
    addrlen= sizeof(peer_addr);
     newsock = accept(server,(struct sockaddr*) &peer_addr, (socklen_t*) &addrlen);
     if(newsock<0)
        {cout<<"failed to accept"<<endl;}
    else
        {cout<<"connected"<<endl;}
struct arg_struct  args;
         args.sock = newsock;
       
        char ip_c[INET_ADDRSTRLEN]; 
        inet_ntop(AF_INET, &(peer_addr.sin_addr), ip_c, INET_ADDRSTRLEN); 
         args.pt = ntohs(peer_addr.sin_port);
         cout<<args.pt<<endl;
         struct arg_struct client;
pthread_mutex_lock(&mutex1);
threadsss.push(args);
pthread_cond_signal(&condition_var);
pthread_mutex_unlock(&mutex1);     
	    
	
    return 0; 
} 