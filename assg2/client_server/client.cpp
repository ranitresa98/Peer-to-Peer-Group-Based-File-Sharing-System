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
  using namespace std;

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

int main(int argc, char const *argv[]) 
{ 
     
     if(argc!=3)
     {
        cout<<"not enough args"<<endl;
        return -1;
     }
     string ip_p=argv[1];
     vector<string> ip_port;
    //  stringstream s (ip_p);
    // string item;
    //  while (getline (s, item,':')) {
    //     ip_port.push_back (item);     
    //       }
     ip_port=split_string(ip_p,':');
   string ipp=ip_port[0];
     char ip[ipp.length()+1];
     strcpy(ip, ipp.c_str());
     int port=stoi(ip_port[1]);
    


    FILE *fp = fopen(argv[2],"r");
    if( !fp  ) {       
    perror("file not there") ;               
        return 1;
    }
    char track_ip1[16],track_ip2[16];
    char line[16];
   if( fgets(track_ip1,16,fp)!=NULL){};
   fgets(line,16,fp);
   int track_port1=atoi(line);
    if(fgets(track_ip2,16,fp) !=NULL){};
    fgets(line,16,fp);
    int track_port2=atoi(line);
    
    int client = socket(AF_INET, SOCK_STREAM, 0); 
    if (client < 0) 
     {perror("create"); exit(1);}
    else
        cout<<"Peer"<<endl; 
      
     

    struct sockaddr_in my_addr;     
    my_addr.sin_family = AF_INET; 
    my_addr.sin_port = htons(track_port1); 
    my_addr.sin_addr.s_addr = inet_addr(track_ip1); 
  
   
    socklen_t addr_size = sizeof my_addr; 
    int con = connect(client, (struct sockaddr*) &my_addr, sizeof( my_addr)); 
    if (con <0) 
         {perror("connect"); exit(1);}
    else
        cout<<"Connected"<<endl;
        char buffer1[256],buffer2[256];  
        char str1[50],str2[50],str3[50],str4[50];
        string command;
       int flag;
  while(1)
    {  
        command.empty();
         getline(cin,command);
         fflush(stdin);
            fflush(stdout);
  // cin>>buffer2;
  // fflush(stdin);
     // cout<<command;   
        // char command_array[command.length()+1];
        // strcpy(command_array,command.c_str());
        // send(client, command_array, 256, 0);

        vector<string> command_list=split_string(command,' ');
       
           // memset(str1,'\0',sizeof(str1));
           strcpy(str1,command_list[0].c_str());
           // cout<<str1<<endl;
           // cout<<str1<<"njj"<<endl;
        if(strcmp(str1,"create_user")==0)
        {            
                   flag=1;
            send(client,&flag,sizeof(flag),0);
            recv(client,buffer1,256,0);
            if(command_list.size() !=3)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            // cout<<command_list[1]<<endl;
            // cout<<command_list[2]<<endl;
             // memset(str2,'\0',sizeof(str2));
             //  memset(str3,'\0',sizeof(str3));
            strcpy(str2,command_list[1].c_str());
            strcpy(str3,command_list[2].c_str());
            // fflush(stdin);
            // fflush(stdout);
            int ack;
            // cout<<str2<<endl;cout<<str3<<endl;
        send(client, str2, strlen(str2), 0);  
        recv(client,&ack,sizeof(ack),0);
        send(client, str3, strlen(str3), 0);  
         recv(client,&ack,sizeof(ack), 0); 
         cout<<"Server : "<< buffer1<<endl;
        }

        else if(strcmp(str1,"login")==0)
        {            
                   flag=2;
            send(client,&flag,sizeof(flag),0);
            recv(client,buffer1,256,0);
            cout<<"Server : "<< buffer1<<endl;
            if(command_list.size() !=3)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            // cout<<command_list[1]<<endl;
            // cout<<command_list[2]<<endl;
             // memset(str2,'\0',sizeof(str2));
             //  memset(str3,'\0',sizeof(str3));
            strcpy(str2,command_list[1].c_str());
            strcpy(str3,command_list[2].c_str());
            // fflush(stdin);
            // fflush(stdout);
            int ack;
            // cout<<str2<<endl;cout<<str3<<endl;
        send(client, str2, strlen(str2), 0);  
        recv(client,&ack,sizeof(ack),0);
        send(client, str3, strlen(str3), 0);  
         recv(client,&ack,sizeof(ack), 0); 
         recv(client, buffer1, 256, 0); 
         cout<<"Server : "<< buffer1<<endl;
        }
        
        else if(strcmp(str1,"create_group")==0)
        {            
                   flag=3;
            send(client,&flag,sizeof(flag),0);
            recv(client,buffer1,256,0);
            if(command_list.size() !=2)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            // cout<<command_list[1]<<endl;
            // cout<<command_list[2]<<endl;
             // memset(str2,'\0',sizeof(str2));
             //  memset(str3,'\0',sizeof(str3));
            strcpy(str2,command_list[1].c_str());
           
            // fflush(stdin);
            // fflush(stdout);
            int ack;
            // cout<<str2<<endl;cout<<str3<<endl;
        send(client, str2, strlen(str2), 0);  
        recv(client,&ack,sizeof(ack),0);
        recv(client, buffer1, 256, 0); 
         cout<<"Server : "<< buffer1<<endl;
        }
 else if(strcmp(str1,"join_group")==0)
        {            
                   flag=4;
            send(client,&flag,sizeof(flag),0);
            recv(client,buffer1,256,0);
            if(command_list.size() !=2)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            // cout<<command_list[1]<<endl;
            // cout<<command_list[2]<<endl;
             // memset(str2,'\0',sizeof(str2));
             //  memset(str3,'\0',sizeof(str3));
            strcpy(str2,command_list[1].c_str());
           
            // fflush(stdin);
            // fflush(stdout);
            int ack;
            // cout<<str2<<endl;cout<<str3<<endl;
        send(client, str2, strlen(str2), 0);  
        recv(client,&ack,sizeof(ack),0);
        recv(client, buffer1, 256, 0); 
         cout<<"Server : "<< buffer1<<endl;
        }

else if(strcmp(str1,"list_groups")==0)
        {            
                   flag=8;
            send(client,&flag,sizeof(flag),0);
            recv(client,buffer1,256,0);
            if(command_list.size() !=1)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
        
int ack=0;
int length;
recv(client, &length, sizeof(length), 0); 
send(client, &ack,sizeof(ack), 0);  
     while(length--)
     {
         
        recv(client, buffer1, 256, 0); 
        cout<<buffer1<<endl;
        send(client, &ack,sizeof(ack), 0); 
        
     }  


        }

        else if(strcmp(str1,"list_requests")==0)
        {            
                   flag=6;
            send(client,&flag,sizeof(flag),0);
            recv(client,buffer1,256,0);
            if(command_list.size() !=1)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
        
              int ack=0;
              int length;
               recv(client, &length, sizeof(length), 0); 
                send(client, &ack,sizeof(ack), 0);  
             while(length--)
              {
         
                     recv(client, buffer1, 256, 0); 
                 cout<<buffer1<<endl;
                  send(client, &ack,sizeof(ack), 0); 
        
              }  


        }


        else if(strcmp(str1,"exit")==0)
        {break;}
         else
         {char buffer2[256]; 
            strcpy(buffer2,"jjj");
            send(client, buffer2, 256, 0);  
         recv(client, buffer1, 256, 0); 
         cout<<"Server : "<< buffer1<<endl;
         }

       
         } 


         close(client);

    return 0; 
} 
