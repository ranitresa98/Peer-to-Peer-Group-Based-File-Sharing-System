#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/sha.h>

using namespace std;

int main()
{
	char * path="a.txt";
 int fd = open (path, O_RDONLY);
 struct stat s;
 if( fstat (fd, &s)==-1)
 	{perror("fopen");}
 size_t size = s.st_size;
char * file_content =(char *)  mmap (NULL, size, PROT_READ , MAP_PRIVATE, fd, 0);
for (int i = 0; i < size; i++)
{
        cout<<file_content[i];
} 
	

	return 0;
}

// for (int i=0; i<SHA_DIGEST_LENGTH; i++)
//  	{printf("%02x", hash[i]);}

// char data[] = "Hello, world!";
// size_t length = strlen(data);

// unsigned char hash[SHA_DIGEST_LENGTH];
// char t[2*SHA_DIGEST_LENGTH];

   
// SHA1((const unsigned char *)data, length, hash);
// for (int i=0; i<SHA_DIGEST_LENGTH; i++)
	 // snprintf(t+2*i, sizeof(t)-2*(i), "%02x", hash);
  //   printf("%s",t);