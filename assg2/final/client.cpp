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

#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
 #define chunk_size 512*1024
#define piece_size 512



  using namespace std;   
int port;
char * ip;
int logged_in=0;
char track_ip1[16],track_ip2[16];
int track_port1,track_port2;


unordered_map<string,string> filename_path; //file name to path





struct down_details
{
    int port;
    int start;
    string filename;
    string file_out;
    int file_size;
    string hash;
};


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


void * service_server(void* args)
{
 int serv=*((int*)args);
    cout<<"peer server"<<endl;


   
// cout<<"server lock enter"<<endl;
    int flag;int ack;

read(serv,&flag,sizeof(flag));
    send(serv,&ack,sizeof(ack),0);

    fflush(stdout);
    
    //  cout<<"client_server flag send"<<endl;
    // cout<<"server flag"<<flag<<endl;
    if(flag==0)
    {   char buffer1[256];
        read(serv,&buffer1,sizeof(buffer1));
        cout<<":"<<buffer1<<endl;
        cout<<endl<<endl;
        // cout<<"client_server flag0 buffer send"<<endl;
   
    close(serv);
    pthread_exit(NULL);
    }

    else

   {
    // cout<<"server enter"<<endl;
        fflush(stdout);
    
       char filename[100];
       memset(filename,0,sizeof(filename));

       
       read(serv,&filename,sizeof(filename));
       send(serv,&ack,sizeof(ack),0);
       // cout<<"client_server filename  send"<<endl;
       

       // cout<<"filename:"<<filename<<endl;
           string filepath=filename_path[filename];
           char  file_path[filepath.length()+1];
           strcpy(file_path,filepath.c_str());
           // cout<<"filepath:"<<file_path<<endl;

           int start;
           
            read(serv,&start,sizeof(start));
            send(serv,&ack,sizeof(ack),0);
            // cout<<"client_server start  send"<<endl;
           // cout<<"start"<<start<<endl;

            int file_size;
           
           read(serv,&file_size,sizeof(file_size));
            send(serv,&ack,sizeof(ack),0);
            // cout<<"client_server file_size  send"<<endl;
            // cout<<"file_size"<<file_size<<endl;


            char hash[400];
            memset(hash,0,sizeof(hash));
             read(serv,&hash,sizeof(hash));
        send(serv,&ack,sizeof(ack),0);
            // cout<<"hash server_addr"<<hash<<endl;
// cout<<"client_server hash  send"<<endl;
   

             FILE* fp=fopen(file_path,"r");
            
              if (fp==NULL) {perror("open"); exit(-1);}
              // else{cout<<"file opened"<<endl;}
             char buffer[piece_size];

             fseek(fp, start, SEEK_SET);
             int len=file_size;
             cout<<"uploading"<<len<<endl;
             
             int i=0;
             while(len>0)
             {   
                int sizee=min(len,piece_size);
                memset(buffer,0,sizee);
                int n=fread(&buffer,sizeof(char),sizee,fp);
                 send(serv,buffer,n,0);
               read(serv,&ack,sizeof(ack));
                len-=n;
                // cout<<"client_server fread  send"<<n<<ack<<endl;
                
                i++;
                }

                // cout<<"i"<<i<<endl;
                 
            fclose(fp);
            char buffer1[256];
  read(serv,buffer1,sizeof(buffer1));
  // cout<<buffer1<<endl<<endl;
  // cout<<"client_server buffer1  send"<<endl;
  fflush(stdout);
    
            
           cout<<"done"<<endl;
           cout<<endl<<endl;
    close(serv);
pthread_exit(NULL);
       }


}



void * download(void* args)
{
    struct down_details *sp = (struct down_details *)args;
     int client = socket(AF_INET, SOCK_STREAM, 0); 
    if (client < 0) 
     {perror("create"); exit(1);}
    
        cout<<endl;
        cout<<"Peer Client"<<endl; 
     

    struct sockaddr_in server_addr;     
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(sp->port); 
    server_addr.sin_addr.s_addr = inet_addr(ip); 
  
   // cout<<"port"<<sp->port<<endl;
    
    int con = connect(client, (struct sockaddr*) &server_addr, sizeof( server_addr)); 
    if (con <0) 
         {perror("connect"); exit(1);}
     cout<<"connected"<<endl;
     int flag=1,ack=1;

    fflush(stdout);
   
     // cout<<"client lock enter"<<endl;
     send(client,&flag,sizeof(flag),0);
     read(client,&ack,sizeof(ack));
//      cout<<"client_cliuent  flag  send"<<endl;
// cout<<"client flag"<<flag<<endl;

     fflush(stdout);
     // cout<<"client enter"<<endl;
     string file_name=sp->filename;
     char  filename[file_name.length()+1];
     strcpy(filename,file_name.c_str());
     // char buffer1[256];
     //    strcpy(buffer1, "ffffg");
     // cout<<"filename :"<<filename<<endl;
     
     send(client,&filename,sizeof(filename),0);
    read(client,&ack,sizeof(ack));
    // cout<<"client_cliuent  filename  send"<<endl;
    
    // cout<<"filename :"<<filename<<endl;
    fflush(stdout);


    int start=sp->start;
    send(client,&start,sizeof(start),0);
    read(client,&ack,sizeof(ack));
   // cout<<"start"<<start<<endl;
   //  // cout<<"client_cliuent  start  send"<<endl;

    int file_size=sp->file_size;
    send(client,&file_size,sizeof(file_size),0);
    read(client,&ack,sizeof(ack));
    // cout<<"file_size"<<file_size<<endl;
    // // cout<<"client_cliuent  file_size  send"<<endl;

   string hashh=sp->hash;
   // cout<<"hash sp->"<<sp->hash<<endl;
   //  cout<<"hash str"<<hashh<<endl;

     char hashf[hashh.length()+1];
     strcpy(hashf,hashh.c_str());
     send(client,&hashf,sizeof(hashf),0);
    read(client,&ack,sizeof(ack));
    // cout<<"hash "<<hashf<<endl;
// cout<<"client_cliuent  hash  send"<<endl;

     
     string dest=sp->file_out;
     char dest_path[50];
     strcpy(dest_path,dest.c_str());

// cout<<"dest  "<<dest_path<<endl;
     FILE* fp=fopen(dest_path,"w+");
     if (fp==NULL) {perror("open"); exit(-1);}
       // else{cout<<"file opened"<<endl;}
    
     char buffer[piece_size];

      fseek(fp, start, SEEK_SET);
      int len=file_size;
      cout<<"downloading"<<len<<endl;

                SHA_CTX ctx;
                SHA1_Init(&ctx);
                
       int i=0;
       while(len>0)
         {

            int n=read(client,buffer,sizeof(buffer));
            fwrite(&buffer,sizeof(char),n,fp);
            send(client,&ack,sizeof(ack),0);
            // cout<<"buffer"<<buffer<<endl;
             len-=n;
             SHA1_Update(&ctx, buffer, n);
             memset(buffer,'\0',sizeof(buffer));
             // cout<<"client_cliuent  fwrite  send"<<n<<ack<<endl;
             // cout<<n<<endl;
           i++;

          }

          // cout<<"i"<<i<<endl;
        fclose(fp);
         // cout<<"downloaded 1"<<endl; 
 
 unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1_Final(hash, &ctx);
        char t[2*SHA_DIGEST_LENGTH];
        for (int i=0; i<SHA_DIGEST_LENGTH; i++)
                    { snprintf(t+2*i,  sizeof(t)-2*(i), "%02x", hash[i]);
                        }

                
string mtorrent="";
            string temp=(string)t;
                     mtorrent=temp.substr(0,20);
    cout<<"downloaded "<<endl; 
    // cout<<hashh<<" "<<mtorrent<<" "<<start<<endl;
    if( (hashh.compare(mtorrent)) == 0)
    {
        sp->start=1;
        // cout<<"hash matches "<<endl;
    }
    char buffer1[256];
                strcpy(buffer1, "ffffg");
                 send(client,buffer1,sizeof(buffer1),0);
                 // cout<<"client_client buffer1  send"<<endl;
fflush(stdout);
   
  close(client);
pthread_exit(NULL);
}



void * server_side(void* args)
{
int client = socket(AF_INET, SOCK_STREAM, 0); 
    if (client < 0) 
     {perror("create"); exit(1);}
    else
        cout<<"Peer Server"<<endl;

     struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET; 
    my_addr.sin_addr.s_addr = INADDR_ANY; 
    my_addr.sin_port = htons(port); 
    my_addr.sin_addr.s_addr = inet_addr(ip); 

     if (bind(client, (struct sockaddr*) &my_addr, sizeof(struct sockaddr_in)) == 0) 
        cout<<"Binded Correctly"<<endl; 
    else
        cout<<"Unable to bind"<<endl;
   if (listen(client, 10) < 0) 
        {perror("listen"); exit(1);}  
    
      
struct sockaddr_in peer_addr;
    socklen_t addrlen;
    int new_sock[1000];
    addrlen= sizeof(peer_addr);
pthread_t tid[1000];
int i=0;
while(1)
{
        new_sock[i] = accept(client,(struct sockaddr*) &peer_addr, (socklen_t*) &addrlen);


             
         if(pthread_create(&tid[i],NULL,service_server,&new_sock[i])!=0)
	    {
	    	cout<<"Failed to create server request service thread\n";
	    }
         i++;
          

 }
close(client);
   // pthread_join(tid,NULL);
pthread_exit(NULL);

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
  
     ip_port=split_string(ip_p,':');
   string ipp=ip_port[0];
      ip=new char[ipp.length()+1];
     strcpy(ip, ipp.c_str());
     port=stoi(ip_port[1]);
    


    FILE *fp = fopen(argv[2],"r");
    if( !fp  ) {       
    perror("file not there") ;               
        return 1;
    }
    
    char line[16];
   if( fgets(track_ip1,16,fp)!=NULL){};
   fgets(line,16,fp);
    track_port1=atoi(line);
    if(fgets(track_ip2,16,fp) !=NULL){};
    fgets(line,16,fp);
     track_port2=atoi(line);
    
    

   
     
     


    pthread_t tid1;
    if(pthread_create(&tid1, NULL, server_side, NULL)!= 0) 
    {
        cout<<"Failed to create server thread"<<endl;
    } 




    int client = socket(AF_INET, SOCK_STREAM, 0); 
    if (client < 0) 
     {perror("create"); exit(1);}
    else
        cout<<"Peer Client"<<endl; 
     

    struct sockaddr_in server_addr;     
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(track_port1); 
    server_addr.sin_addr.s_addr = inet_addr(track_ip1); 
  
   
    
    int con = connect(client, (struct sockaddr*) &server_addr, sizeof( server_addr)); 
    if (con <0) 
         {perror("connect"); exit(1);}
    else
       { cout<<"Connected"<<endl;}
        char buffer1[256],buffer2[256];  
        char str1[100],str2[100],str3[100],str4[100];
        string command;
       int flag;
       int ack;
        send(client,&port,sizeof(port),0);
        read(client,&ack,sizeof(ack));

  while(1)
    {  
        fflush(stdin);
         fflush(stdout);
         cout<<endl<<endl;
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
       
           memset(str1,0,sizeof(str1));
           strcpy(str1,command_list[0].c_str());
           // cout<<str1<<endl;
           // cout<<str1<<"njj"<<endl;









        if(strcmp(str1,"create_user")==0)
        {            
                   flag=1;
            
            if(command_list.size() !=3)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
            // cout<<command_list[1]<<endl;
            // cout<<command_list[2]<<endl;
             memset(str2,0,sizeof(str2));
              memset(str3,0,sizeof(str3));
            strcpy(str2,command_list[1].c_str());
            strcpy(str3,command_list[2].c_str());
            // fflush(stdin);
            // fflush(stdout);
            
            // cout<<str2<<endl;cout<<str3<<endl;
        send(client, str2, strlen(str2), 0);  
        read(client,&ack,sizeof(ack));
        send(client, str3, strlen(str3), 0);  
         read(client,&ack,sizeof(ack)); 
         cout<<"Server : "<< buffer1<<endl;
        }











        else if(strcmp(str1,"login")==0)
        {            
                   flag=2;
            
            if(command_list.size() !=3)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
            cout<<"Server : "<< buffer1<<endl;
            // cout<<command_list[1]<<endl;
            // cout<<command_list[2]<<endl;
             memset(str2,0,sizeof(str2));
              memset(str3,0,sizeof(str3));
            strcpy(str2,command_list[1].c_str());
            strcpy(str3,command_list[2].c_str());
            // fflush(stdin);
            // fflush(stdout);
            
            // cout<<str2<<endl;cout<<str3<<endl;
        send(client, str2, strlen(str2), 0);  
        read(client,&ack,sizeof(ack));
        send(client, str3, strlen(str3), 0);  
         read(client,&ack,sizeof(ack)); 
         read(client,&ack,sizeof(ack));
        if(ack==0)
        {

            cout<<"Invalid user name or paasword"<<endl;
        }
        else if(ack==1)
        {
            logged_in=1;
            cout<<"logged in suceessfully"<<endl;
        }
         else
        {
            cout<<"Already logged in somewhere"<<endl;
        }


        }
        











        else if(strcmp(str1,"create_group")==0)
        {   
        if(logged_in)
        {         
                   flag=3;
            
            if(command_list.size() !=2)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
            // cout<<command_list[1]<<endl;
            // cout<<command_list[2]<<endl;
              memset(str2,0,sizeof(str2));
              memset(str3,0,sizeof(str3));
            strcpy(str2,command_list[1].c_str());
           
            // fflush(stdin);
            // fflush(stdout);
            
            // cout<<str2<<endl;cout<<str3<<endl;
        send(client, str2, strlen(str2), 0);  
        read(client,&ack,sizeof(ack));
        read(client, buffer1, 256); 
         cout<<"Server : "<< buffer1<<endl;

         }
         else{
            cout<<"login first"<<endl;
         }
        }














 else if(strcmp(str1,"join_group")==0)
        {            
              
              if(logged_in){     flag=4;
           
            if(command_list.size() !=2)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
             send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
            // cout<<command_list[1]<<endl;
            // cout<<command_list[2]<<endl;
             memset(str2,0,sizeof(str2));
              memset(str3,0,sizeof(str3));
            strcpy(str2,command_list[1].c_str());
           
            // fflush(stdin);
            // fflush(stdout);
           
            // cout<<str2<<endl;cout<<str3<<endl;
        send(client, str2, strlen(str2), 0);  
    
        read(client, buffer1, 256); 
         cout<<"Server : "<< buffer1<<endl;
         }

         else{
            cout<<"login first"<<endl;
         }
        }












else if(strcmp(str1,"list_groups")==0)
        {     
        if(logged_in)
        {       
                   flag=8;
            
            if(command_list.size() !=1)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
        send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
    ack=0;
    int length;
    read(client, &length, sizeof(length)); 
    send(client, &ack,sizeof(ack), 0);  
     while(length--)
     {
         
        read(client, buffer1, 256); 
        cout<<buffer1<<endl;
        send(client, &ack,sizeof(ack), 0); 
        
     }  
     cout<<"list of groups printed"<<endl;
    }
        else{
            cout<<"login first"<<endl;
         }

        }










        else if(strcmp(str1,"list_requests")==0)
        {            
               if(logged_in)
               {
                   flag=6;
               
            
            if(command_list.size() !=2)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
             ack=0;
              memset(str2,0,sizeof(str2));
        strcpy(str2,command_list[1].c_str());
        send(client, str2, strlen(str2), 0);  
        read(client,&ack,sizeof(ack));
        if(ack==0)
        {
            cout<<"You are not a owner"<<endl;
            continue;
        }
        else
        {
              int length=0;
              send(client, &ack,sizeof(ack), 0); 
               read(client, &length, sizeof(length)); 
               cout<<"Size:"<<length<<endl;
                send(client, &ack,sizeof(ack), 0);  
             while(length--)
              {
         
                read(client, buffer1, 256); 
                cout<<buffer1<<endl;
                send(client, &ack,sizeof(ack), 0); 
        
              }  
            cout<<"list of pending requests printed"<<endl;
            }

        }
        else{
            cout<<"login first"<<endl;
         }
        }







    else if(strcmp(str1,"leave_group")==0)
        {            
              
              if(logged_in){     flag=5;
           
            if(command_list.size() !=2)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
             send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
           
            memset(str2,0,sizeof(str2));
            strcpy(str2,command_list[1].c_str());
           
            
            // cout<<str2<<endl;cout<<str3<<endl;
        send(client, &str2, strlen(str2), 0);  
        
        read(client, buffer1, 256); 
         cout<<"Server : "<< buffer1<<endl;
         }

         else{
            cout<<"login first"<<endl;
         }
        }










        else if(strcmp(str1,"upload_file")==0)
        {
             if(logged_in){    
              flag=10;
                if(command_list.size() !=3)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
            fflush(stdout);
           cout<<buffer1<<endl;
           memset(str2,0,sizeof(str2));
           memset(str3,0,sizeof(str3));
                strcpy(str2,command_list[1].c_str());
                 strcpy(str3,command_list[2].c_str());
                send(client, str3, strlen(str3), 0);
                cout<<"group_id "<<str3<<endl;
                read(client,&ack,sizeof(ack));
                fflush(stdout);

                if(ack==0)
                {
                    cout<<"Not part of group";
                    continue;
                }    
                
                

                FILE* fp = fopen(str2, "r"); 
                   if (fp == NULL) { 
                        printf("File Not Found!\n"); 
                        ack=0;
                        send(client, &ack, sizeof(ack), 0);
                        continue;
                    } 
                  ack=1;
                   send(client, &ack, sizeof(ack), 0);
                  fflush(stdout);
                 
                    fseek(fp, 0L, SEEK_END); 
                    int file_size = ftell(fp); 
                   
                    fclose(fp);

                    // cout<<"size"<<file_size<<endl;
            send(client, &file_size, sizeof(file_size), 0);
                read(client,&ack,sizeof(ack));
                cout<<"size"<<file_size<<endl;
            fflush(stdout);


                // cout<<"filename "<<str2<<endl;
                cout<<"filepath "<<str2<<endl;
                fflush(stdout);
                vector<string> file;
                 file=split_string(str2,'/');
                 string filea=file[file.size()-1];
                   char filename[filea.length()+1];
                  strcpy(filename, filea.c_str());
                  send(client, &filename, sizeof(filename), 0);
                read(client,&ack,sizeof(ack));
                
                  cout<<"filename:"<<filename<<endl;
                  filename_path[filename]=str2;

//                 string path;
//                 path.assign(str2);
//                 string file_name;
//                 size_t found = path.find_last_of("/");  //get last /
//                 if(found !=string::npos)
//                 file_name=path.substr(found+1);
//                 else
//                 file_name=path;
//                 char filename[file_name.length()+1];
//                 strcpy(filename,file_name.c_str());  
                

                    char copy_content[chunk_size];  ///512kb
                      int nread;
                string mtorrent="";
                int chunks=0;
                unsigned char hash[SHA_DIGEST_LENGTH];
                int fd = open(str2, O_RDONLY);
                while((nread = read(fd,copy_content,sizeof(copy_content))) > 0) 
            {
         
                SHA1((const unsigned char *)copy_content,nread, hash);
                char t[2*SHA_DIGEST_LENGTH];
                 for (int i=0; i<SHA_DIGEST_LENGTH; i++)
                    { snprintf(t+2*i,  sizeof(t)-2*(i), "%02x", hash[i]);
                        }
                        string temp=(string)t;
                     mtorrent+=temp.substr(0,20);
                     chunks++;
    
            }
            if(close(fd)<0){perror("cant close file");}
            
            



              cout<<"chunks"<<chunks<<endl;
            send(client, &chunks, sizeof(chunks), 0);
             read(client,&ack,sizeof(ack));
        
             // cout<<"chunks"<<chunks<<endl;
            

                  // cout<<"hassh"<<mtorrent<<endl;
                char hassh[mtorrent.length()+1];
                strcpy(hassh,mtorrent.c_str());
                // cout<<"hassh"<<hassh<<endl;
                send(client, &hassh, sizeof(hassh), 0);
                cout<<"hassh"<<hassh<<endl;
                
               
                    


                read(client, &buffer1, 256);
                   cout<<buffer1<<endl;
                  

            }
            else{
            cout<<"login first"<<endl;
         }
        }




else if(strcmp(str1,"accept_request")==0)
        {            
               if(logged_in)
               {
                   flag=7;
               
            
            if(command_list.size() !=3)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
             ack=0;
             memset(str2,0,sizeof(str2));
        strcpy(str2,command_list[1].c_str());
        send(client, str2, strlen(str2), 0);  
        read(client,&ack,sizeof(ack));
        if(ack==0)
        {
            cout<<"You are not a owner"<<endl;
            continue;
        }
        else
        {
            memset(str3,0,sizeof(str3));
         strcpy(str3,command_list[2].c_str());
        send(client, str3, strlen(str3), 0);
        read(client, &buffer1, 256);
                   cout<<buffer1<<endl;


        }

        }
        else{
            cout<<"login first"<<endl;
         }
        }


else if(strcmp(str1,"list_files")==0)
        {            
               if(logged_in)
               {
                   flag=9;
               
            
            if(command_list.size() !=2)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
            
            memset(str2,0,sizeof(str2));
        strcpy(str2,command_list[1].c_str());
        send(client, str2, strlen(str2), 0);  
       
         
         ack=0;
    int length;
    read(client, &length, sizeof(length)); 
    send(client, &ack,sizeof(ack), 0);  
     while(length--)
     {
         
        read(client, buffer1, 256); 
        cout<<buffer1<<endl;
        send(client, &ack,sizeof(ack), 0); 
        
     }  
     cout<<"list of files printed"<<endl;

       

        }
        else{
            cout<<"login first"<<endl;
         }
        }









else if(strcmp(str1,"download_file")==0)
        {            
               if(logged_in)
               {
                   flag=11;
               
            
            if(command_list.size() !=4)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
           
           memset(str2,0,sizeof(str2));
           memset(str3,0,sizeof(str3));
           memset(str4,0,sizeof(str4)); 
        strcpy(str2,command_list[1].c_str());
        strcpy(str3,command_list[2].c_str());
        strcpy(str4,command_list[3].c_str());


        send(client, str2, strlen(str2), 0);          
         read(client,&ack,sizeof(ack));
         cout<<"group:"<<str2<<endl;


         send(client, str3, strlen(str3), 0);   
         cout<<"file:"<<str3<<endl;
         
    int length;
    read(client, &length, sizeof(length)); 
    send(client, &ack,sizeof(ack), 0); 
    cout<<length<<endl; 

    if(length==0)
    {
        cout<<"no peers available for download"<<endl;
        continue;
    }
    else
    {vector<int> peer_lists;
        cout<<"list of seeders printed"<<endl;
         while(length--)
         {
             int port_downloader;
            read(client, &port_downloader, sizeof(port_downloader)); 
            cout<<port_downloader<<endl;
            peer_lists.push_back(port_downloader);
            send(client, &ack,sizeof(ack), 0); 
            
         }  
         


         int no_of_chunks;
         read(client, &no_of_chunks, sizeof(no_of_chunks));
         send(client, &ack,sizeof(ack), 0); 
         cout<<"no_of_chunks"<<no_of_chunks<<endl;

         int filesize;
         read(client, &filesize, sizeof(filesize));
         send(client, &ack,sizeof(ack), 0); 
         cout<<"filesize"<<filesize<<endl;

         fflush(stdout);
         char hashf[400];
          read(client,&hashf,sizeof(hashf));
          
          cout<<"hash:"<<hashf<<endl;
fflush(stdout);

    cout<<"total no of peers"<<peer_lists.size()<<endl;
    length=peer_lists.size();
            
            pthread_t tid[no_of_chunks];

    string dest=(string)str4;
     dest=dest+"/"+(string)str3;
            //  struct down_details args;
            //  args.filename=str3;
            // args.file_out=str4;
            // args.hash=(string)hashf; 
            // cout<<"hash:"<<args.hash<<endl;

            struct down_details args[no_of_chunks];
             int j=0;
         for(int i=0;i<no_of_chunks;i++)
         {
            args[i].filename=str3;
            args[i].file_out=dest;
            string temp=(string)hashf;
            args[i].hash=temp.substr((i*20),20); 
            args[i].port=peer_lists[i%length];   
            cout<<"port "<<i%length<<" :"<<args[i].port<<endl;                    
            args[i].file_size=min(filesize,chunk_size);
            args[i].start=(i*chunk_size);
            
            filesize-=chunk_size;          
         if(pthread_create(&tid[i],NULL,download,(void*)&args[i])!=0)
        {
            cout<<"Failed to create server request service thread\n";
        }
         }
         
         for(int i=0;i<no_of_chunks;i++)
         {

                pthread_join(tid[i],NULL);
                // cout<<args[i].start<<endl;
                if(args[i].start==1)
                {j++;}
             // cout<<j<<endl;
         }
        
         if(j==no_of_chunks)
            {cout<<"hash verified"<<endl;}
    cout<<"Downloaded"<<endl;
    filename_path[str3]=dest;
          send(client, &ack,sizeof(ack), 0); 
    
            }
        }
        else{
            cout<<"login first"<<endl;
         }
        }

else if(strcmp(str1,"logout")==0)
        {   
         if(logged_in)
               {         
                   flag=12;
             if(command_list.size() !=1)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);
            send(client,&ack,sizeof(ack),0);
            read(client,buffer1,256);
            cout<<buffer1<<endl;
            logged_in=0;
            
    
            }else{
            cout<<"login first"<<endl;
         }

        }
  else if(strcmp(str1,"stop_share")==0)
        {   
         if(logged_in)
               {         
                   flag=14;
             if(command_list.size() !=3)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);

memset(str2,0,sizeof(str2));
memset(str3,0,sizeof(str3));
            strcpy(str2,command_list[1].c_str());
        strcpy(str3,command_list[2].c_str());
        

        send(client, str2, strlen(str2), 0);          
         read(client,&ack,sizeof(ack));
         cout<<"group_id"<<str2<<endl;
            send(client, str3, strlen(str3), 0); 
            cout<<"file name"<<str3<<endl;         
         read(client,buffer1,256);
         cout<<buffer1<<endl;
            
    
            }else{
            cout<<"login first"<<endl;
         }

        }







else if(strcmp(str1,"show_downloads")==0)
        {   
         if(logged_in)
               {         
                   flag=13;
             if(command_list.size() !=1)
            {
                cout<<"enter valid command"<<endl;
                continue;
            }
            send(client,&flag,sizeof(flag),0);
            read(client,buffer1,256);

            ack=0;
    int length;
    read(client, &length, sizeof(length)); 
    send(client, &ack,sizeof(ack), 0);  
     while(length--)
     {
         
        read(client, buffer1, 256); 
        cout<<buffer1<<endl;
        send(client, &ack,sizeof(ack), 0); 
        
     } 
     
    read(client, &length, sizeof(length)); 
    send(client, &ack,sizeof(ack), 0);  
     while(length--)
     {
         
        read(client, buffer1, 256); 
        cout<<buffer1<<endl;
        send(client, &ack,sizeof(ack), 0); 
        
     } 

            
    
            }else{
            cout<<"login first"<<endl;
         }

        }




        else if(strcmp(str1,"exit")==0)
        {break;}
         else
         {
         cout<<"invalid command"<<endl;
         }

       
         } 


         close(client);



    return 0; 

} 
