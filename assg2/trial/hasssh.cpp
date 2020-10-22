
// g++ hasssh.cpp -Wall -lcrypto -o hasssh
#include <bits/stdc++.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
int main()
{
	string patth="main.cpp";
  char path[patth.length()+1]; 
 strcpy(path, patth.c_str());
char copy_content[512*1024];

int nread;
int fd = open(path, O_RDONLY); 

struct stat s;
string mtorrent="";
 if( fstat (fd, &s)==-1)
 	{perror("fopen");}
 size_t size = s.st_size;
mtorrent+=to_string(size);
size_t found = patth.find_last_of("/");  //get last /
string file_name=patth.substr(found+1);   //get file name
mtorrent+=file_name;


unsigned char hash[SHA_DIGEST_LENGTH];

while((nread = read(fd,copy_content,sizeof(copy_content))) > 0) 
{
// 	string ans=(string)copy_content;
// cout<<ans.substr(0, nread);
// cout<<"--------------------------";

SHA1((const unsigned char *)copy_content,nread, hash);
char t[2*SHA_DIGEST_LENGTH];
 for (int i=0; i<SHA_DIGEST_LENGTH; i++)
	{ snprintf(t+2*i,  sizeof(t)-2*(i), "%02x", hash[i]);
	    }
// cout<<t;
//      cout<<endl;
     mtorrent+=(string)t;


   
	
}
cout<<mtorrent<<endl;

if(close(fd)<0){perror("cant close file");}

size_t found_last = file_name.find_last_of(".");  //get last /
string target_file=patth.substr(0,found_last);
target_file+=".mtorrent";

char target_file_path[target_file.length()+1]; 
 strcpy(target_file_path, target_file.c_str());
char mtorrent_arr[mtorrent.length()+1]; 
 strcpy(mtorrent_arr, mtorrent.c_str());

fd = open(target_file_path, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
write(fd,mtorrent_arr,sizeof(mtorrent));
if(close(fd)<0){perror("cant close file");}

return 0;
}