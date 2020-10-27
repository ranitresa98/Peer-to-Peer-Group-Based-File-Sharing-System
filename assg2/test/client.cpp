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

vector<string> seperate_space(string command)
{
    vector<string> command_v;
    istringstream ss(command); 
    do { 
       
        string word; 
      command_v.push_back(word);
        
    } while (ss); 
        return command_v;
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
     stringstream s (ip_p);
    string item;
     while (getline (s, item,':')) {
        ip_port.push_back (item);     
          }
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
        cout<<"Client"<<endl; 
      
     

    struct sockaddr_in my_addr;     
    my_addr.sin_family = AF_INET; 
    my_addr.sin_port = htons(track_port1); 
    my_addr.sin_addr.s_addr = inet_addr(track_ip1); 
  
    // struct sockaddr_in my_addr1;
    // my_addr1.sin_family = AF_INET; 
    // my_addr1.sin_addr.s_addr = INADDR_ANY; 
    // my_addr1.sin_port = htons(port); 
    // my_addr1.sin_addr.s_addr = inet_addr(ip);
    // if (bind(client, (struct sockaddr*) &my_addr1, sizeof(struct sockaddr_in)) <0) 
    //     {perror("bind"); exit(1);}
    // else
    //     cout<<"Binded"<<endl; 
      
    socklen_t addr_size = sizeof my_addr; 
    int con = connect(client, (struct sockaddr*) &my_addr, sizeof( my_addr)); 
    if (con <0) 
         {perror("connect"); exit(1);}
    else
        cout<<"Connected"<<endl;
        char buffer1[256];  
        char str1[50],str2[50],str3[50],str4[50];
        string command;
       
  while(1)
    {  
        command.empty();
         getline(cin,command);
  
        
        // char command_array[command.length()+1];
        // strcpy(command_array,command.c_str());
        // send(client, command_array, 256, 0); 
        vector<string> command_list=seperate_space(command);
       
           
           strcpy(str1,command_list[0].c_str());
           // cout<<str1<<"njj"<<endl;
        if(strcmp(str1,"create_user")==0)
        {            

            if(command_list.size() !=3)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            strcpy(str2,command_list[1].c_str());
            strcpy(str3,command_list[2].c_str());
        send(client, str2, strlen(str2), 0);  
        send(client, str3, strlen(str3), 0);  
         recv(client, buffer1, 256, 0); 
         cout<<"Server : "<< buffer1<<endl;
        }
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
