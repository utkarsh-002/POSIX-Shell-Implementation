#include <bits/stdc++.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

extern int nextFlag;

void pipe(queue<string>& tokens){

    vector<vector<char*> > argvs;
    vector<char*> argv;

    int stdInput = dup(STDIN_FILENO);
    int stdOut = dup(STDOUT_FILENO);

    while(!tokens.empty()){
        while(!tokens.empty() && tokens.front()!="|" && tokens.front() != ";"){
            argv.push_back(strdup(tokens.front().c_str()));
            tokens.pop();
        }
        argv.push_back(nullptr);
        argvs.push_back(argv);
        argv.clear(); 
        if(!tokens.empty() && (tokens.front() == "|" || tokens.front() == ";"))  tokens.pop();
    }

    
    int numPp = argvs.size() - 1;
    int pipeFd[2], ppFd = -1;

    for (int i = 0; i < argvs.size(); ++i) {
        if (i < numPp) {
            if (pipe(pipeFd) == -1) {
                perror("pipe");
                nextFlag = 0;
                return;
            }
        }

        int childPid = fork();

        if(childPid == 0){
            if(i > 0){ 
                    dup2(ppFd, STDIN_FILENO);
                    close(ppFd);
            }
            if(i < numPp){ 
                    dup2(pipeFd[1], STDOUT_FILENO);
                    close(pipeFd[0]);
                    close(pipeFd[1]);
            }

            if(execvp(argvs[i][0], argvs[i].data()) == -1){
                    perror("execvp");
                    nextFlag = 0;
                    exit(1);
            }
        }

        if (i > 0) close(ppFd);
        if (i < numPp) {
            close(pipeFd[1]);
            ppFd = pipeFd[0];
        }
    }

    close(ppFd);
    for (int i = 0; i < argvs.size(); ++i) {
        wait(nullptr);
    }

    dup2(stdInput, STDIN_FILENO);
    dup2(stdOut, STDOUT_FILENO);
    close(stdInput);
    close(stdOut);
}