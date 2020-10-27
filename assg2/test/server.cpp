#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <pthread.h>   
struct arg_struct {
    int sock;
    int pt;
};
using namespace std;

vector<string> seperate_space(string command)
{
    
     vector<string> command_v;
     stringstream s (command);
    string item;
     while (getline (s, item,':')) {
        command_v.push_back (item);     
          }
          return command_v;
}
void * handle_conn(void* args)
{
	struct arg_struct *sp = (struct arg_struct *)args;
	int client_socket=sp->sock;
    int port=sp->pt;
	// free (args);

	char buffer1[256], buffer2[256];
       char req_client[1000];
            while(1){recv(client_socket, req_client, 1000, 0); 
                       cout<<"Client "<<port<<":"<<req_client<<endl; 
                        strcpy(buffer1, "main zinda hu"); 
                        send(client_socket, buffer1, 256, 0); 
                        }

        pthread_exit(NULL);
        
}

int main(int argc, char const *argv[]) 
{ 
     if(argc!=3 )
     {
        cout<<"not enough args"<<endl;
        return 1;
     }

     


    
    FILE *fp = fopen(argv[1],"r");
    if( !fp  ) {       
    perror("file not there") ;               
        return 1;
    }
    char ip1[16],ip2[16];
    char line[16];
   if( fgets(ip1,16,fp)!=NULL){};
   fgets(line,16,fp);
   int port1=atoi(line);
    if(fgets(ip2,16,fp) !=NULL){};
    fgets(line,16,fp);
    int port2=atoi(line);

     // char ip[ipp.length()+1];
     // strcpy(ip, ipp.c_str());
     // string portt1=argv[2];
     
     // int port2=stoi(portt2);



    int server = socket(AF_INET, SOCK_STREAM, 0); 
    if (server < 0) 
       {perror("socket"); 
            exit(1);}
    else
       cout<<"tracker"<<endl; 
          
    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET; 
    my_addr.sin_addr.s_addr = INADDR_ANY; 
    my_addr.sin_addr.s_addr = inet_addr(ip1); 
    my_addr.sin_port = htons(port1); 

    if (bind(server, (struct sockaddr*) &my_addr, sizeof(my_addr)) <0) 
        
         {perror("bind"); exit(1);} 
    else
       cout<<"binded"<<endl;
          
    if (listen(server, 10) < 0) 
        {perror("listen"); exit(1);}  
    else
       cout<<"sunn raha hu  main"<<endl;
      
    socklen_t addr_size; 
    addr_size = sizeof(struct sockaddr_in); 
      
   
    // pthread_t tid[15];
     pthread_t tid;


    struct sockaddr_in peer_addr;
    socklen_t addrlen;
    int new_sock;
    addrlen= sizeof(peer_addr);


     while(new_sock = accept(server,(struct sockaddr*) &peer_addr, (socklen_t*) &addrlen))
     { 	
	   struct arg_struct args;
         args.sock = new_sock;
       
        char ip_c[INET_ADDRSTRLEN]; 
        inet_ntop(AF_INET, &(peer_addr.sin_addr), ip_c, INET_ADDRSTRLEN); 
	     args.pt = ntohs(peer_addr.sin_port);
         // int i=0;
	    if(pthread_create(&tid, NULL, handle_conn, (void *)&args)!= 0 )
	    {
	    	cout<<"failed to create thread\n";
	    } 
	    
	} close(server);   

    return 0; 
} 