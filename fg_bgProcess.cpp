#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<bits/stdc++.h>

using namespace std;

unsigned int sleep(unsigned int seconds);
extern int nextFlag,frdPid;
void fgExecute(vector<char*> argv) {

    pid_t childPid = fork();
    
    if(childPid==0){
        if (execvp(argv[0], argv.data()) == -1){
            perror("\nexecvp");
            nextFlag=0;
        }
    }else if(childPid>0){
        int status;
        frdPid = childPid;
        int res = waitpid(childPid, &status, 2);
        if(res <0){
            perror("wait");
            nextFlag=0;
        }
        
    }
    else
    {
        printf("Some error has occurred");
    }
    return;
}

void bgExecute(vector<char*> argv) {

    int childPid = fork();

    if(childPid==0){
        if (execvp(argv[0], argv.data()) == -1){
            perror("\nexecvp");
            nextFlag=0;
        }
    }else if(childPid>0){
        frdPid = childPid;
        write(1,"\n",1);
        string pid = to_string(childPid);
        write(1,pid.c_str(),pid.size());
    }
    return;
}

void fg_bg(queue<string>& tokens,int flag){
    vector<char*> argv;

    while(!tokens.empty() && tokens.front()!="&" && tokens.front()!=";"){
        argv.push_back(strdup(tokens.front().c_str()));
        tokens.pop();
    }
    argv.push_back(nullptr);

    while(!tokens.empty())  tokens.pop();

    if(flag==0) 
        fgExecute(argv);
    else if(flag==1)
        bgExecute(argv);
    return;
}