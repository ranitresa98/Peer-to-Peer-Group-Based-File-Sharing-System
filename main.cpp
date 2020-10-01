#include <unistd.h> 
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <dirent.h>
#include <termios.h>
#include <time.h>
#include <sys/ioctl.h>
#include<sys/wait.h> 
#define clear_scr() cout<<"\x1b[2J";
using namespace std;

string root;
char * current_path;
struct termios initial_settings, new_settings;
int rows,cols;
int file_start,file_end;
int cursor_position=1;
vector<string> dir_list;
stack<string> back_trace;
stack<string> forward_trace;


void non_canonical();
void restore();
void moveTo(int row, int col);
void print_list_dirs();
void print_dirs();
void normal_navigation();
void getWindowSize();
void command_mode();
void command_processing(string command);
string path_form(string path);
void make_dir(vector<string> commands_with_args);
void make_file(vector<string> commands_with_args);
void change_directory(vector<string> commands_with_args);
void remove_directory(vector<string> commands_with_args);
void remove_file(vector<string> commands_with_args);
void remove_directories(string path_dir);
void rename_file(vector<string> commands_with_args);
void move_file(vector<string> commands_with_args);
void copy_file(vector<string> commands_with_args);


int main()
{
  non_canonical();
   char path[1024];
  
  if (getcwd(path, sizeof(path)) != NULL) {
       
   } else {
       perror("pwd");
       // exit(1);
   }
  
   root=path;
   current_path=path;

 print_list_dirs();

   normal_navigation();
 
 restore();
return 0;

}


void getWindowSize() {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
   perror("small screen");
  } else {
    cols = ws.ws_col;
    rows = ws.ws_row;
   
  }
  
}


void print_list_dirs()
{

DIR* dir;
struct dirent *direntries;

  
dir = opendir(current_path);
if (dir == NULL) {
      perror ("no directory entry");
      // exit(1);
}

dir_list.clear();
while ((direntries=readdir(dir)) != NULL) {
  dir_list.push_back(direntries->d_name);
    }
    if(closedir(dir)==-1){
       perror ("close dir");
      // exit(1);
    }

  file_start=0;file_end=dir_list.size();  
  print_dirs();
cursor_position=1;
    moveTo(1,1);
     
}



void print_dirs()
{
  clear_scr();
  getWindowSize();
  moveTo(1,1);
string dirname;
struct stat dirinfo;

 
 if(rows<dir_list.size() )
 {
    file_end=file_start+rows-1;
 }
 else
  {file_start=0;file_end=dir_list.size(); }
 sort(dir_list.begin(), dir_list.end());

 for(int i=file_start;i<file_end && i<dir_list.size();i++)
 {
  dirname = string(current_path) + "/" + string(dir_list[i]);
      char dirname_array[dirname.length()+1]; 
      strcpy(dirname_array, dirname.c_str()); 
      if(stat (dirname_array, &dirinfo)<0)
      {perror(dirname_array);}

    char dir_name[dir_list[i].length()+1]; 
      strcpy(dir_name, dir_list[i].c_str());
       
    // cout<<setw(110)<<left<<dir_name<<"\t";
    // char file_type='-';
    // file_type=(S_ISDIR(dirinfo.st_mode))? 'd' : file_type;

    char file_type='s';
    file_type=(S_ISREG(dirinfo.st_mode)) ? '-' : file_type;
    file_type=(S_ISDIR(dirinfo.st_mode))? 'd' : file_type;
    file_type=(S_ISCHR(dirinfo.st_mode)) ? 'c' : file_type;
    file_type=(S_ISFIFO(dirinfo.st_mode))? 'p' : file_type;
    file_type=(S_ISLNK(dirinfo.st_mode))? 'l' : file_type;
    file_type=(S_ISBLK(dirinfo.st_mode))? 'b' : file_type;
     
    
    cout<<file_type;
    printf( (dirinfo.st_mode & S_IRUSR) ? "r" : "-");
    printf( (dirinfo.st_mode & S_IWUSR) ? "w" : "-");
    printf( (dirinfo.st_mode & S_IXUSR) ? "x" : "-");
    printf( (dirinfo.st_mode & S_IRGRP) ? "r" : "-");
    printf( (dirinfo.st_mode & S_IWGRP) ? "w" : "-");
    printf( (dirinfo.st_mode & S_IXGRP) ? "x" : "-");
    printf( (dirinfo.st_mode & S_IROTH) ? "r" : "-");
    printf( (dirinfo.st_mode & S_IWOTH) ? "w" : "-");
    printf( (dirinfo.st_mode & S_IXOTH) ? "x" : "-");
    cout<<"\t"<<setw(10)<<left<<dirinfo.st_size;
      
         char *time_m= ctime(&dirinfo.st_mtime);
     time_m[strlen(time_m)-1] = '\0';
      cout<<"\t"<<setw(25)<<left<<time_m;
cout<<"\t"<<dir_name<<endl;
  
      // cout<<"\t"<<setw(20)<<left<<time_m<<endl;
 
    }
   }



   void moveTo(int row, int col) {
cout<<"\x1b["<<row<<";"<<col<<"H";
}



void non_canonical()
{
  tcgetattr(0,&initial_settings);
   atexit(restore);
new_settings = initial_settings;
new_settings.c_lflag &= ~ICANON;
new_settings.c_lflag &= ~ECHO;

if(tcsetattr(0, TCSANOW, &new_settings) != 0) {
perror ("non_canonical");
}
}

void restore()
{
 clear_scr();
 tcsetattr(0,TCSANOW,&initial_settings);
}





  void normal_navigation()
  { moveTo(rows,1);
 cout<<"\x1b[K";  
    cout<<" NORMAl MODE"<<current_path;
    moveTo(cursor_position,65);  
    char ch;
    while(ch=cin.get()){
      
      print_dirs();
        if(ch==':') 
    { 
     command_mode();   
     print_list_dirs();    
    }
    
     if (ch == 27) // up down left right
     {
      ch = cin.get();
      if (ch == '[') // up down left right
     {
      ch= cin.get();
    
         if(ch=='A') //up
        {
          if(cursor_position>1 /*&& file_start==0*/ )
         { 
          
          cursor_position--;
          cout<<"\x1b[1A";
          }
        
        }
        
          else if(ch=='B')//down
          { 
            
          if(cursor_position<(rows-1) && cursor_position<dir_list.size() /*&& file_end==dir_list.size()*/ )
         { 
          
          cursor_position++;
            cout<<"\x1b[1B";
          }
       
          }
          
        else if(ch=='C')//right
          {
            if(!forward_trace.empty())
            { back_trace.push(string(current_path));
              string path=forward_trace.top();
              forward_trace.pop();
              strcpy(current_path, path.c_str());
              print_list_dirs();
          }
          
          }
        
        else if(ch=='D')//left
          {
           if(!back_trace.empty())
            { forward_trace.push(string(current_path));
              string path=back_trace.top();
              back_trace.pop();
              strcpy(current_path, path.c_str());
              print_list_dirs();
          }
          }     
      }
    }
     else if(ch=='h')//home
         {
          back_trace.push(current_path);
          forward_trace=stack<string>();
       strcpy(current_path, root.c_str());
       print_list_dirs();
             
       }
       
    else   if(ch==127)//backspace
       { 
          back_trace.push(current_path);
          string str=current_path;
           size_t found = str.find_last_of("/\\");
          string parent=str.substr(0,found);
          strcpy(current_path, parent.c_str()); 
          print_list_dirs();
          forward_trace=stack<string>();
       }
       


     else  if(ch==10)//enter 
      {
 
      string dirname;
      struct stat dirinfo; 
      string dir_name=string(dir_list[cursor_position+file_start-1]);
      if(dir_name==".")
      {continue;}
     else if(dir_name=="..")
    {
      string str=current_path;
      size_t found = str.find_last_of("/\\");
          string parent=str.substr(0,found);
          back_trace.push(current_path);
          strcpy(current_path, parent.c_str()); 
          print_list_dirs();
    }
     else  { dirname = string(current_path) + "/" + dir_name;
           char child_path[dirname.length()+1]; 
      strcpy(child_path, dirname.c_str()); 
      if(stat (child_path, &dirinfo)<0)
      {perror(child_path);}
      if(S_ISDIR(dirinfo.st_mode))
        { 
          back_trace.push(current_path);
           strcpy(current_path, dirname.c_str());
          print_list_dirs();
                 
        }
        else if(S_ISREG(dirinfo.st_mode)){
        pid_t pid = fork(); 
          if (pid == 0) {

            execl("/usr/bin/vi", "vi", dirname.c_str(), NULL);
    
      exit(0);
      
    }
    else{
      wait(NULL);
    }
    
    }
    }
  }

   else if(ch=='k'){ 
       if(file_start>0)
        {
          file_start--;
          file_end--;
          print_dirs();
          moveTo(cursor_position,1);
        } 
         }
    else  if(ch=='l'){ 
         if(file_end!=dir_list.size()  )
        {
          file_start++;
          file_end++;
          print_dirs();
          moveTo(cursor_position,1);
        }
      }
    moveTo(rows,1);
    cout<<"\x1b[K";
    cout<<" NORMAl MODE "<<current_path<<(int)ch;
    moveTo(cursor_position,65);  
  }
}










void command_mode()
{
  string command="";
  moveTo(rows,1);
 cout<<"\x1b[K";  
    cout<< " COMMAND MODE :" ;
// char ch[4];
    // while(read(STDIN_FILENO, &ch, 3)>=0){
    char ch;
    while(ch=cin.get()){
      print_list_dirs();
      
    
    
        //     if((int)ch[0]==27)  //esc
    if(ch==27)            //esc
    { 
     //  if(ch[1]='[' && (ch[2]=='A' || ch[2]=='B' || ch[2]=='C' || ch[2]=='D') ) continue;
     // // ch = cin.get();
     // //  ch= cin.get();
     //  if(ch=='A' || ch=='B' || ch=='C' || ch=='D')  continue;
     normal_navigation();       
    }
    
    else  if(ch==127)//backspace
       {
        if(!command.empty()) 
        command.pop_back();
       }
  else if(ch==10) //enter
{
if(!command.empty()) 
{command_processing(command);
command="";
}}
else{
command+=ch;
}

moveTo(rows,1);
//cout<<"\x1b[K"; 

    cout<<" COMMAND MODE :"<<command;
}
}



void command_processing(string command)
{
vector<string> commands_with_args;
stringstream s (command);
    string item;

    while (getline (s, item,' ')) {
        commands_with_args.push_back (item);
          }
if(commands_with_args[0]=="copy")
{copy_file(commands_with_args);return;}
else if(commands_with_args[0]=="move")
{move_file(commands_with_args);return;}
else if(commands_with_args[0]=="rename")
{rename_file(commands_with_args);return;}
else if(commands_with_args[0]=="create_file")
{make_file(commands_with_args);return;}
else if(commands_with_args[0]=="create_dir")
{make_dir(commands_with_args);return;}
else if(commands_with_args[0]=="delete_file")
{remove_file(commands_with_args);return;}
else if(commands_with_args[0]=="delete_dir")
{remove_directory(commands_with_args);return;}
else if(commands_with_args[0]=="goto")
{change_directory(commands_with_args);return;}
else if(commands_with_args[0]=="search")
{}
else
  {moveTo(rows,1);  cout<<":wrong command";}
}



string path_form(string path)
{
string final_path;
if(path[0]=='~' || path[0]=='/' )
{final_path=root+ "/"; path=path.substr(1);}
else if(path[0]=='.')
  {final_path=string(current_path)+ "/";path=path.substr(1);
if(path[0]=='.')
  { string str=current_path;
    size_t found = str.find_last_of("/\\");
     final_path=str.substr(0,found);
      path=path.substr(1); }
}
else
  {final_path=string(current_path)+ "/";}
if(path[0]=='/')
{path=path.substr(1);}

final_path+=path;
return final_path;

}


void make_dir(vector<string> commands_with_args)
{
if (commands_with_args.size()!=3)
{moveTo(rows,1);
cout<<"\x1b[K"; 
    cout<<"invalid no of arguments";
    return;}
string path=path_form(commands_with_args[2]);
path+="/";
path+=commands_with_args[1];

char path_dir[path.length()+1]; 
      strcpy(path_dir, path.c_str());
if( mkdir(path_dir, S_IRWXU | S_IRWXG | S_IXOTH )<0)
  {perror("cant make directory");}

}



void make_file(vector<string> commands_with_args)
{
if (commands_with_args.size()!=3)
{moveTo(rows,1);
cout<<"\x1b[K"; 
    cout<<"invalid no of arguments";
    return;}
string path=path_form(commands_with_args[2]);
path+="/";
path+=commands_with_args[1];

char path_dir[path.length()+1]; 
      strcpy(path_dir, path.c_str());
int fd=open(path_dir, O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IROTH|S_IWGRP);
if( fd<0)
  {perror("cant make file");}
if(close(fd)<0){perror("cant close file");}
}

void change_directory(vector<string> commands_with_args)
{
  if (commands_with_args.size()!=2)
{moveTo(rows,1);
cout<<"\x1b[K"; 
    cout<<"invalid no of arguments";
    return;}
string path=path_form(commands_with_args[1]);


back_trace.push(current_path);
           strcpy(current_path, path.c_str());
          print_list_dirs();

}


void remove_directories(char * path_dir)
{struct stat dirinfo; 
  DIR* dir;
struct dirent *direntries;
// char path_dirs[path_dir.length()+1]; 
//       strcpy(path_dirs, path_dir.c_str());
  dir = opendir(path_dir);
if (dir == NULL) {
      perror ("no directory entry");
}
while ((direntries=readdir(dir)) != NULL) {
  if(string(direntries->d_name)=="." || string(direntries->d_name)=="..")
    continue;
 string dirname = string(path_dir) + "/" + string(direntries->d_name);
  char path[dirname.length()+1]; 
      strcpy(path, dirname.c_str()); 
      if(stat (path, &dirinfo)<0)
      {perror(path);}
      if(S_ISDIR(dirinfo.st_mode))
        { 
          remove_directories(path);
        }
        else if(S_ISREG(dirinfo.st_mode)){
        if( remove(path)<0)
  {perror(" can't delete file");
}
}
    }
    if(closedir(dir)==-1){
       perror ("close dir");
    }
    if( rmdir(path_dir)<0)
  {perror(" can't delete directory");
}
}
void remove_directory(vector<string> commands_with_args)
{
  if (commands_with_args.size()!=2)
{moveTo(rows,1);
cout<<"\x1b[K"; 
    cout<<"invalid no of arguments";
    return;}
string path=path_form(commands_with_args[1]);
char path_dir[path.length()+1]; 
      strcpy(path_dir, path.c_str());
      remove_directories(path_dir);
}
void remove_file(vector<string> commands_with_args)
{
  if (commands_with_args.size()!=2)
{moveTo(rows,1);
cout<<"\x1b[K"; 
    cout<<"invalid no of arguments";
    return;}
string path=path_form(commands_with_args[1]);
char path_dir[path.length()+1]; 
      strcpy(path_dir, path.c_str());
if( remove(path_dir)<0)
  {perror(" can't delete file");
}


}


void rename_file(vector<string> commands_with_args)
{
if (commands_with_args.size()!=3)
{moveTo(rows,1);
cout<<"\x1b[K"; 
    cout<<"invalid no of arguments";
    return;}
string from_path=path_form(commands_with_args[1]);
string to_path=path_form(commands_with_args[2]);

char from_path_dir[from_path.length()+1]; 
 strcpy(from_path_dir, from_path.c_str());
 char to_path_dir[to_path.length()+1]; 
 strcpy(to_path_dir, to_path.c_str());
if(rename(from_path_dir, to_path_dir) <0)
  {perror("rename");}

}

void move_file(vector<string> commands_with_args)
{
  int n=commands_with_args.size();
if (n<3)
{moveTo(rows,1);
cout<<"\x1b[K"; 
    cout<<"invalid no of arguments";
    return;}




 
 for(int i=1;i<n-1;i++)
 {

   string from_path=path_form(commands_with_args[i]);
  char from_path_dir[from_path.length()+1]; 
 strcpy(from_path_dir, from_path.c_str());

string to_path=path_form(commands_with_args[n-1]);
 size_t found = from_path.find_last_of("/\\");
  string to_name=from_path.substr(found+1);
to_path=to_path+"/"+to_name;
char to_path_dir[to_path.length()+1]; 
 strcpy(to_path_dir, to_path.c_str());

 if(link(from_path_dir, to_path_dir)==-1)
  {perror("move copy");}
if(remove(from_path_dir)==-1)
  {perror("move delte");}

}

}




void copy_file(vector<string> commands_with_args)
{
  int n=commands_with_args.size();
if (n<3)
{moveTo(rows,1);
cout<<"\x1b[K"; 
    cout<<"invalid no of arguments";
    return;}

 for(int i=1;i<n-1;i++)
 {

   string from_path=path_form(commands_with_args[i]);
  char from_path_dir[from_path.length()+1]; 
 strcpy(from_path_dir, from_path.c_str());

string to_path=path_form(commands_with_args[n-1]);
 size_t found = from_path.find_last_of("/\\");
  string to_name=from_path.substr(found+1);
to_path=to_path+"/"+to_name;
char to_path_dir[to_path.length()+1]; 
 strcpy(to_path_dir, to_path.c_str());

 if(symlink(from_path_dir, to_path_dir)==-1)
  {perror("move copy");}


}

}