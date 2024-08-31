#include <unistd.h>
#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

void getPwd(queue<string>& tokens,string curDirPath){
    char path[PATH_MAX];
    int cnt=0;
    while(!tokens.empty() && tokens.front()!=";"){
        cnt++;
        tokens.pop();
    }
    tokens.pop();
    if(cnt>1){
        string err = "\npwd: too many arguments";
        write(1,err.c_str(),err.size());
    }else{
        if(getcwd(path,sizeof(path))!=NULL){
            write(1,"\n",1);
            write(1,path,strlen(path));
        }else{
            perror("pwd() error");
        }
    }
}