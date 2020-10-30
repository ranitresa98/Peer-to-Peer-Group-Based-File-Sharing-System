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
#include <thread>
using namespace std;

struct arg_struct {
    int sock;
    int pt;
    string ip;
};



struct user_info
{
    string passwd;
    int login;
    string ip;
    int port;
user_info()
{
login=0;

}


};
unordered_map<string,struct user_info> users_list;
unordered_map<string,pair<string,int>> group_owner;
unordered_map<string,vector<pair<string,int>>> group_users;
unordered_map<string,vector<pair<string,int>>> pending_group_request;
vector<string> list_groups;
void * handle_conn(void* args)
{
	struct arg_struct *sp = (struct arg_struct *)args;
	int client_socket=sp->sock;
    int port=sp->pt;
    string ip=sp->ip;
	// free (args);

	char buffer1[256], buffer2[256];
       int flag;
            while(1){

                recv(client_socket, &flag, sizeof(flag), 0); 
                    cout<<"Peer "<<ip<<":"<<port<<"  "<<flag<<endl; 
                    strcpy(buffer1, "Tracker Listening"); 
                    send(client_socket, buffer1, 256, 0); 

                    switch(flag)
                    {
                        case 1:                ///////create_user
                            {char uname[50];
                              char pwd[50];
                              int ack=0;
                              memset(uname,0,sizeof(uname));
                              memset(pwd,0,sizeof(pwd));
                              recv(client_socket,&uname,sizeof(uname),0);
                             send(client_socket,&ack,sizeof(ack),0);
                              // cout<<"Username "<<":"<<uname<<endl; 
                               recv(client_socket,&pwd,sizeof(pwd),0);
                                send(client_socket,&ack,sizeof(ack),0);
                               // cout<<"Passwd "<<port<<":"<<pwd<<endl; 
                               fflush(stdout);

                               
                                users_list[uname].passwd=pwd;
                               
                                break;
                            }

                             case 2:        ////////////login
                            {char uname[50];
                              char pwd[50];
                              int ack=0;
                              memset(uname,0,sizeof(uname));
                              memset(pwd,0,sizeof(pwd));
                              recv(client_socket,&uname,sizeof(uname),0);
                             send(client_socket,&ack,sizeof(ack),0);
                              // cout<<"Username "<<":"<<uname<<endl; 
                               recv(client_socket,&pwd,sizeof(pwd),0);
                                send(client_socket,&ack,sizeof(ack),0);
                               // cout<<"Passwd "<<port<<":"<<pwd<<endl; 
                               fflush(stdout);

                               
                                if(users_list[uname].passwd==pwd)
                                {
                                    cout<<"logged in"<<endl;
                                    memset(buffer1,0,sizeof(buffer1));
                              
                                    strcpy(buffer1, "Swaggat hai tumara"); 
                                    send(client_socket, buffer1, 256, 0); 
                                    users_list[uname].login=1;
                                    users_list[uname].ip=ip;
                                    users_list[uname].port=port;
                                    // cout<<users_list[uname].ip<<endl;
                                    // cout<<users_list[uname].port<<endl;
                                }
                                else
                                {
                                    cout<<"wrong passwd"<<endl;
                                    memset(buffer1,0,sizeof(buffer1));
                                    strcpy(buffer1, "inavlid creditionals"); 
                                    send(client_socket, buffer1, 256, 0);
                                }
                               
                                break;
                            }



                     case 3:                ///////create_group
                            {
                              char group_id[50];
                              int ack=0;
                              memset(group_id,0,sizeof(group_id));
                              
                              recv(client_socket,&group_id,sizeof(group_id),0);
                             send(client_socket,&ack,sizeof(ack),0);
                              cout<<"Group "<<":"<<group_id<<endl; 
                                
                               fflush(stdout);

                               
                                group_owner[group_id]={ip,port};
                                group_users[group_id].push_back({ip,port});
                                list_groups.push_back(group_id);
                               memset(buffer1,0,sizeof(buffer1));
                               strcpy(buffer1, "Group created and joined");
                               send(client_socket, buffer1, 256, 0);
                                break;
                            }

                        case 4:                ///////join_group
                            {
                              char group_id[50];
                              int ack=0;
                              memset(group_id,0,sizeof(group_id));
                              
                              recv(client_socket,&group_id,sizeof(group_id),0);
                             send(client_socket,&ack,sizeof(ack),0);
                              cout<<"Group "<<":"<<group_id<<endl; 
                                
                               memset(buffer1,0,sizeof(buffer1));
                              
                                     
                                    

                               if(find (list_groups.begin(), list_groups.end(), group_id) !=list_groups.end())
                              { strcpy(buffer1, "Request Pending");
                                pending_group_request[group_id].push_back({ip,port});}
                                
                                else{strcpy(buffer1, "Invalid group id");}
                               send(client_socket, buffer1, 256, 0); 
                                break;
                            }


                            case 5:                ///////leave_group
                            {
                            
                            }

                                case 6:                ///////list_requests
                            {
                            
                                int ack;
                                int ack1;
                                if()
                                int length=list_groups.size();
                                 send(client_socket, &length, sizeof(length), 0);
                                recv(client_socket,&ack,sizeof(ack),0);
                                for (auto i=list_groups.begin();i!=list_groups.end();i++)
                                {
                                    string grp=*i;
                                    cout<<grp;
                                    char group[grp.length()];
                                    strcpy(group,grp.c_str());
                                    strcpy(buffer1, grp.c_str());
                                    send(client_socket, buffer1, 256, 0);
                                    recv(client_socket,&ack,sizeof(ack),0);
                                }
                               
                                cout<<"list of groups send"<<endl;


                            }
                            case 7:                ///////accept_request
                            {
                            
                            }

                            case 8:                ///////list_groups
                            {
                                int ack;
                                int length=list_groups.size();
                                 send(client_socket, &length, sizeof(length), 0);
                                recv(client_socket,&ack,sizeof(ack),0);
                                for (auto i=list_groups.begin();i!=list_groups.end();i++)
                                {
                                    string grp=*i;
                                    cout<<grp;
                                    char group[grp.length()];
                                    strcpy(group,grp.c_str());
                                    strcpy(buffer1, grp.c_str());
                                    send(client_socket, buffer1, 256, 0);
                                    recv(client_socket,&ack,sizeof(ack),0);
                                }
                               
                                cout<<"list of groups send"<<endl;
                            }

                        default:
                              {  break;}




                    }


                    }

      close(client_socket);
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
      
   
    pthread_t tid;
     // pthread_t tid[20];

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
         args.ip=inet_ntoa(peer_addr.sin_addr);
         // int i=0;
	    if(pthread_create(&tid, NULL, handle_conn, (void *)&args)!= 0 )
	    {
	    	cout<<"failed to create thread\n";
	    }  
	}  
     

  
   pthread_join(tid,NULL);
    return 0; 
} 