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
#include <stdlib.h>
#include <unistd.h>

#define HASH_SIZE 400//20*20 i.e. 10mb
#define PB push_back

using namespace std;

struct arg_struct {
    int sock;
    int pt;
    string ip;
};
struct arg_struct_for_accept {
    string group_id;
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

struct file_info
{
string hash;
int size;
int chunks;

};
struct download_info
{
string group_id;
string filename;
string status;
};

unordered_map<int,int> port_mapper; //port to actual wala
unordered_map<string,struct user_info> users_list; //username->details
unordered_map<int,string> user_port_id;   //port->username
unordered_map<string,int> group_owner;      //group_name ->port
unordered_map<string,vector<int>> group_users;   //group_anme ->users port
unordered_map<string,vector<int>> pending_group_request;  //group_name ->requuset [orts]
vector<string> list_groups;                  //group_anmes
unordered_map<string, unordered_set<string> > group_files;     //group ->filenames
map<pair<string,string>, struct file_info > file_info_mapp;    //file name->info
map<pair<string,string>, vector<int> > file_peer;   //group:filename->ports of those who have it

unordered_map<int,vector<struct download_info>> download_file_list;  //port ->download_details



void * inform_user_added(void* args)
{
    struct arg_struct_for_accept *sp = (struct arg_struct_for_accept *)args;
    string ipp=sp->ip;
    char * ip;
     ip=new char[ipp.length()+1];
     strcpy(ip, ipp.c_str());
     string group_id=sp->group_id;
     
int client = socket(AF_INET, SOCK_STREAM, 0); 
    if (client < 0) 
     {perror("create"); }
  struct sockaddr_in server_addr;     
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(sp->pt); 
 server_addr.sin_addr.s_addr = inet_addr(ip);

    cout<<sp->pt<<endl;
 int con = connect(client, (struct sockaddr*) &server_addr, sizeof( server_addr));
 if(con<0)
    {perror("connect");}
 int flag=0,ack;
send(client,&flag,sizeof(flag),0);
 char buffer1[256];
 read(client,&ack,sizeof(ack));

string mzg="Joined group"+group_id;
 strcpy(buffer1, mzg.c_str()); 
 send(client, buffer1, 256, 0); 

  close(client);
 pthread_exit(NULL);

}

vector<string> split_string(string command, char delimeter)
{
    vector<string> tokens;
    string token="";
    for(int i=0;i<command.length();i++)
    {
        char ch=command[i];
        
         if(ch==delimeter)
        {
            tokens.push_back(token);
            token="";
        }
        else{
            token += ch;
        }
    }
    tokens.push_back(token);
    return tokens;
}
void * handle_conn(void* args)
{
	struct arg_struct *sp = (struct arg_struct *)args;
	int client_socket=sp->sock;
    int port=sp->pt;
    string ip=sp->ip;
int ack;
int real_port;
read(client_socket,&real_port,sizeof(real_port)); 
send(client_socket,&ack,sizeof(ack),0);
port_mapper[port]=real_port;

	char buffer1[256], buffer2[256];
       int flag;
            while(1){

                cout<<endl<<endl;
                read(client_socket, &flag, sizeof(flag)); 
                    cout<<"Peer "<<ip<<":"<<real_port<<"  "<<flag<<endl; 
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
                              read(client_socket,&uname,sizeof(uname));
                             send(client_socket,&ack,sizeof(ack),0);
                               read(client_socket,&pwd,sizeof(pwd));
                                send(client_socket,&ack,sizeof(ack),0);
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
                              read(client_socket,&uname,sizeof(uname));
                             send(client_socket,&ack,sizeof(ack),0);
                               read(client_socket,&pwd,sizeof(pwd));
                                send(client_socket,&ack,sizeof(ack),0);
                               fflush(stdout);

                               
                                if(users_list[uname].passwd==pwd)
                                {
                                    
                                    memset(buffer1,0,sizeof(buffer1));
                                    
                                    if(users_list[uname].login==1)
                                    {cout<<"already logged in"<<endl;
                                        ack=2;
                                        send(client_socket, &ack,sizeof(ack),0);  
                                    }

                                    else
                                    {cout<<"logged in"<<endl;
                                        ack=1;
                                    send(client_socket, &ack,sizeof(ack),0); 
                                    users_list[uname].login=1;
                                    users_list[uname].ip=ip;
                                    users_list[uname].port=port;
                                    user_port_id[port]=uname;
                                      }
                                }
                                else
                                {
                                    cout<<"wrong passwd"<<endl;
                                    ack=0;
                                    send(client_socket, &ack,sizeof(ack),0); 
                                }
                               
                                break;
                            }



                     case 3:                ///////create_group
                            {
                              char group_id[50];
                              int ack=0;
                              memset(group_id,0,sizeof(group_id));
                              
                              read(client_socket,&group_id,sizeof(group_id));
                             send(client_socket,&ack,sizeof(ack),0);
                              cout<<"Create Group "<<":"<<group_id<<endl; 
                                
                               fflush(stdout);

                               
                                group_owner[group_id]=port;
                                group_users[group_id].PB(port);
                                list_groups.PB(group_id);
                               memset(buffer1,0,sizeof(buffer1));
                               strcpy(buffer1, "Group created and joined");
                               send(client_socket, buffer1, 256, 0);
                                break;
                            }

                        case 4:                ///////join_group
                            {
                              char group_id[50];
                              memset(group_id,0,sizeof(group_id));                              
                              read(client_socket,&group_id,sizeof(group_id));                             
                              cout<<"Join Group "<<":"<<group_id<<endl;                                 
                               memset(buffer1,0,sizeof(buffer1));

                               if(find (list_groups.begin(), list_groups.end(), group_id) !=list_groups.end())
                              { strcpy(buffer1, "Request Pending");
                                pending_group_request[group_id].PB(port);
                                cout<<"list:"<<pending_group_request[group_id].size()<<endl;
                              }                                
                            else{
                                    strcpy(buffer1, "Invalid group id");
                                }
                            send(client_socket, buffer1, 256, 0); 
                            break;
                               }


                            case 5:                ///////leave_group
                            {
                            
                            char group_id[50];
                              memset(group_id,0,sizeof(group_id));
                              read(client_socket,&group_id,sizeof(group_id));
                              cout<<"Leave Group "<<":"<<group_id<<endl; 
                               memset(buffer1,0,sizeof(buffer1));
                                    
                                auto i=find (list_groups.begin(), list_groups.end(), group_id);
                               if(i !=list_groups.end())
                              {
                                
                               group_users[group_id].erase(remove(group_users[group_id].begin(), group_users[group_id].end(), port),group_users[group_id].end());
                                
                               for(auto j=group_files[group_id].begin();j!=group_files[group_id].end();j++)
                               {
                                for(auto k=file_peer[{group_id,*j}].begin();k!=file_peer[{group_id,*j}].end();k++)
                                    {
                                    file_peer[{group_id,*j}].erase(remove(file_peer[{group_id,*j}].begin(), file_peer[{group_id,*j}].end(), real_port),file_peer[{group_id,*j}].end());
                                   }
                               }
                                strcpy(buffer1, "Group leaved");
                                
                                cout<<"Group leaved";
                              }
                                
                                else{
                                    strcpy(buffer1, "Invalid group id");
                                    cout<<"Invalid group id";
                                }
                               send(client_socket, buffer1, 256, 0); 
                                break;
                               }


                                case 6:                ///////list_requests
                            {
                            
                                char group_id[50];
                              int ack=0;
                              memset(group_id,0,sizeof(group_id));
                              
                              read(client_socket,&group_id,sizeof(group_id));
                             
                              cout<<"List requests "<<":"<<group_id<<endl; 

                               
                               if( group_owner[group_id]!=port)
                               {
                                send(client_socket,&ack,sizeof(ack),0);
                              }
                                else
                               {
                                ack=1;
                                send(client_socket,&ack,sizeof(ack),0);
                              
                                read(client_socket,&ack,sizeof(ack));
                                int length=pending_group_request[group_id].size();
                                 send(client_socket, &length, sizeof(length),0);
                               read(client_socket,&ack,sizeof(ack));
                                for (auto i=pending_group_request[group_id].begin();i!=pending_group_request[group_id].end();i++)
                                {
                                    int req_port=*i;
                                    string req_user=user_port_id[req_port];
                                    cout<<req_user;
                                    strcpy(buffer1, req_user.c_str());
                                    send(client_socket, buffer1, 256, 0);
                                    read(client_socket,&ack,sizeof(ack));
                                }
                               
                                cout<<"list of requests send"<<endl;
                                
                                    }
                                    break;
                            }
                            case 7:                ///////accept_request
                            {
                             char group_id[50];
                              int ack=0;
                              memset(group_id,0,sizeof(group_id));
                             read(client_socket,&group_id,sizeof(group_id));
                                                        
                               if( group_owner[group_id]!=port)
                               {
                                cout<<"Not owner";
                                send(client_socket,&ack,sizeof(ack),0);

                               }
                                else
                               {
                                cout<<"Owner"<<endl;
                                ack=1;
                                send(client_socket,&ack,sizeof(ack),0);
                                char uname[50];
                                read(client_socket,&uname,sizeof(uname));
                                int user_port=users_list[uname].port;
                                
                                group_users[group_id].PB(user_port);
                                pending_group_request[group_id].erase(remove(pending_group_request[group_id].begin(), pending_group_request[group_id].end(), user_port),pending_group_request[group_id].end());
                                int user_real_port=port_mapper[user_port];


                                pthread_t tid1;
                                  struct arg_struct_for_accept arg;
       
                                arg.ip=ip;
                                 arg.pt=user_real_port;
                                 arg.group_id=group_id;
                                 
                                 if(pthread_create(&tid1, NULL, inform_user_added, (void *)&arg)!= 0 )
                                  {
                                        cout<<"failed to create thread\n";
                                   } 
                                   
                                
                                  pthread_join(tid1,NULL);
                                   
                                    cout<<"User added"<<endl;

                                strcpy(buffer1, "User added");
                                send(client_socket, buffer1, 256, 0);
                               }

                                break;
                            }

                            case 8:                ///////list_groups
                            {
                                int ack;
                                int length=list_groups.size();
                                 send(client_socket, &length, sizeof(length), 0);
                                read(client_socket,&ack,sizeof(ack));
                                for (auto i=list_groups.begin();i!=list_groups.end();i++)
                                {
                                    string grp=*i;
                                    cout<<grp<<endl;
                                    
                                    strcpy(buffer1, grp.c_str());
                                    send(client_socket, buffer1, 256, 0);
                                    read(client_socket,&ack,sizeof(ack));
                                }
                               
                                cout<<"list of groups send"<<endl;
                                break;
                            }

                            case 9:                ///////list_files
                            {
                                cout<<"List file"<<endl;
                                 char group_id[50];
                                    memset(group_id,0,sizeof(group_id));
                                    read(client_socket,&group_id,sizeof(group_id));
                                    int length=group_files[group_id].size();
                                 send(client_socket, &length, sizeof(length), 0);
                                read(client_socket,&ack,sizeof(ack));
                                for (auto i=group_files[group_id].begin();i!=group_files[group_id].end();i++)
                                {
                                    string grp=*i;
                                    cout<<grp<<endl;
                                    
                                    strcpy(buffer1, grp.c_str());
                                    send(client_socket, buffer1, 256, 0);
                                    read(client_socket,&ack,sizeof(ack));
                                }

                                break;
                            }
                            case 10:                ///////upload_file
                            {
                                cout<<"Upload file"<<endl;
                                    int ack=0;
                                    char group_id[50];
                                    memset(group_id,0,sizeof(group_id));
                                    read(client_socket,&group_id,sizeof(group_id));
                                  fflush(stdout);


                                auto i=find (group_users[group_id].begin(), group_users[group_id].end(), port);
                               if(i ==group_users[group_id].end())
                              {
                                ack=0;
                                send(client_socket,&ack,sizeof(ack),0);
                              }
                              else
                              {
                                ack=1;
                                send(client_socket,&ack,sizeof(ack),0);
                               read(client_socket,&ack,sizeof(ack));
                                if(ack==1)
                                {
                                    cout<<"file exists"<<endl;
                                    int file_size;
                                
                                read(client_socket,&file_size,sizeof(file_size));
                                send(client_socket,&ack,sizeof(ack),0);
                                cout<<"size"<<file_size<<endl;
                                    fflush(stdout);

                                char filename[100];                                
                                read(client_socket,&filename,sizeof(filename));
                                send(client_socket,&ack,sizeof(ack),0);
                                 cout<<"filepath "<<filename <<endl;
                                  fflush(stdout);
                               
                                
                                file_info_mapp[{group_id,filename}].size=file_size;
                                  

                                int no_of_chunks;
                                read(client_socket,&no_of_chunks,sizeof(no_of_chunks));
                                send(client_socket,&ack,sizeof(ack),0);
                               cout<<"chunks"<<no_of_chunks<<endl;
                                file_info_mapp[{group_id,filename}].chunks=no_of_chunks;
                                fflush(stdout);




                                char hashf[HASH_SIZE];
                                read(client_socket,&hashf,sizeof(hashf));
                                cout<<"hash:"<<hashf<<endl;
                                cout<<"hash length:"<<sizeof(hashf)<<endl;
                                file_info_mapp[{group_id,filename}].hash=hashf;
                                
                                group_files[group_id].insert(filename);
                                file_peer[{group_id,filename}].PB(real_port);
                                   

                                strcpy(buffer1, "File uploaded");
                                send(client_socket, buffer1, 256, 0);
                                

                               cout<<"Fileuploaded";
                                 }
                              }
                                break;
                            
                            }


                            case 11:          //download_files
                            {
                                    cout<<"Download file"<<endl;
                                    int ack=0;
                                    char group_id[50];
                                    memset(group_id,0,sizeof(group_id));
                                    read(client_socket,&group_id,sizeof(group_id));
                                    send(client_socket,&ack,sizeof(ack),0);
                                    cout<<"group"<<group_id<<endl;
                                    
                                    char filename[100];                                
                                    read(client_socket,&filename,sizeof(filename));
                                    cout<<"filename "<<filename<<endl;

                                    int length=file_peer[{group_id,filename}].size();
                                    send(client_socket, &length, sizeof(length), 0); 
                                    read(client_socket,&ack,sizeof(ack));
                                    cout<<"length"<<length<<endl;

                                    if(length!=0)
                                {        
                                  for (auto i=file_peer[{group_id,filename}].begin();i!=file_peer[{group_id,filename}].end();i++)
                                    {
                                      int ports=*i;
                                       cout<<ports<<endl;
                                        send(client_socket, &ports, sizeof(ports), 0);
                                       read(client_socket,&ack,sizeof(ack));
                                    }
                                    int chunk=file_info_mapp[{group_id,filename}].chunks;
                                     send(client_socket, &chunk, sizeof(chunk), 0);
                                     read(client_socket,&ack,sizeof(ack));
                                     cout<<"chunk"<<chunk<<endl;

                                      int file_size=file_info_mapp[{group_id,filename}].size;
                                     send(client_socket, &file_size, sizeof(file_size), 0);
                                     read(client_socket,&ack,sizeof(ack));
                                     cout<<"file_size"<<chunk<<endl;

                                     string hashh=file_info_mapp[{group_id,filename}].hash;
                                     char hashf[hashh.length()+1];
                                     strcpy(hashf,hashh.c_str());
                                    
                                     send(client_socket, &hashf, sizeof(hashf), 0);
                                    cout<<"hash"<<hashf<<endl;


                                    download_info a;
                                    a.group_id=group_id;
                                    a.filename=filename;
                                    a.status="D";
                                    download_file_list[port].push_back(a);




                                    read(client_socket,&ack,sizeof(ack));

                                    a.status="C";
                                    download_file_list[port].pop_back();
                                    download_file_list[port].push_back(a);
                                    file_peer[{group_id,filename}].PB(real_port);
                                     cout<<"Downloaded"<<endl;
                                } 

                                break;
                            }

                               case 12:                ///////logout
                            {
                              
                             
                               read(client_socket,&ack,sizeof(ack));

                                strcpy(buffer1, "Logged out");
                                send(client_socket,&buffer1,sizeof(buffer1),0);
                                
                               fflush(stdout);
                                cout<<"Logged out"<<endl;
                               
                                users_list[user_port_id[port]].login=0;
                               
                                break;
                            }


                                case 13:                ///////Show_downloads:
                            {

                               int length=download_file_list[port].size();
                               send(client_socket, &length, sizeof(length), 0); 
                                    read(client_socket,&ack,sizeof(ack));
                                
                                    if(length!=0)
                                {        
                                  for (auto i=download_file_list[port].begin();i!=download_file_list[port].end();i++)
                                    {
                                      string gid=i->group_id;
                                    string filee=i->filename;
                                    string st=i->status;
                                    string gid_filee="["+st+"]["+gid+"]"+filee;
                                    cout<<gid_filee<<endl;
                                    strcpy(buffer1, gid_filee.c_str());
                                    send(client_socket, buffer1, 256, 0);
                                       read(client_socket,&ack,sizeof(ack));
                                    }
                                
                               }
                                break;
                            }








                                 case 14:                ///////stop share
                            {
                              
                             
                               char group_id[50];
                                    memset(group_id,0,sizeof(group_id));
                                    read(client_socket,&group_id,sizeof(group_id));
                                    send(client_socket,&ack,sizeof(ack),0);
                                    cout<<"group"<<group_id<<endl;
                                    
                                    char filename[100];   
                                    memset(filename,0,sizeof(filename));                             
                                    read(client_socket,&filename,sizeof(filename));
                                    cout<<"filename "<<filename<<endl;
                                    
                                auto i=find (file_peer[{group_id,filename}].begin(), file_peer[{group_id,filename}].end(), real_port);
                               if(i !=file_peer[{group_id,filename}].end())
                              {
                                
                               file_peer[{group_id,filename}].erase(remove(file_peer[{group_id,filename}].begin(), file_peer[{group_id,filename}].end(), real_port),file_peer[{group_id,filename}].end());
                               if(file_peer[{group_id,filename}].size()==0)
                                        {group_files[group_id].erase(filename);}
                                strcpy(buffer1, "Sharing stopped");
                                cout<<"Sharing stopped"<<endl;
                              }
                                
                                else{
                                    strcpy(buffer1, "Invalid group id or file name");
                                    cout<<"Invalid group id or file name"<<endl;
                                }


                                    send(client_socket,buffer1,sizeof(buffer1),0);


                               
                                break;
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
      
   
   
    pthread_t tid[1000];
    
    struct sockaddr_in peer_addr;
    socklen_t addrlen;
    int new_sock;
    addrlen= sizeof(peer_addr);

  struct arg_struct args[1000];
  int i=0;

while(1)
{
        new_sock = accept(server,(struct sockaddr*) &peer_addr, (socklen_t*) &addrlen);
	      
         args[i].sock = new_sock;
        
     
    args[i].ip=inet_ntoa(peer_addr.sin_addr);
    args[i].pt=(ntohs(peer_addr.sin_port));
         
	    if(pthread_create(&tid[i], NULL, handle_conn, (void *)&args[i])!= 0 )
	    {
	    	cout<<"failed to create thread\n";
	    }  
        i++;
	}  
     

  pthread_exit(NULL);
    return 0; 
} 