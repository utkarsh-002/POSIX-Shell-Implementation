#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<bits/stdc++.h>

using namespace std;

extern int nextFlag;
void fgExecute(vector<char*> argv) {

    int childPid = fork();
    
    if(childPid==0){
        if (execvp(argv[0], argv.data()) == -1){
            perror("\nexecvp");
            nextFlag=0;
        }
    }else if(childPid>0){
        int status;
        int res = waitpid(childPid, &status, WUNTRACED);
        if(res==-1){
                perror("\nwaitpid failed");
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
        cout<<"\n";
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