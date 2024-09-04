#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <bits/stdc++.h>

using namespace std;
extern string inResult;
extern int nextFlag;

void IORedir(queue<string>& tokens) {

    vector<char*> argv;

    int stdInput = dup(STDIN_FILENO);
    int stdOut = dup(STDOUT_FILENO);

    if(inResult.find(">>")!=string::npos){
        
        if(inResult.find("<")!=string::npos){
            if(inResult.find("<") > inResult.find(">>")){
                string err = "\nIncorrect order of redirection tokens!!";
                write(1,err.c_str(),err.size());
            }else{
                
                while(!tokens.empty() && tokens.front()!="<"){
                    argv.push_back(strdup(tokens.front().c_str()));
                    tokens.pop();
                }
                argv.push_back(nullptr);

                tokens.pop();
                
                string filename = tokens.front();
                tokens.pop();
                
                int file;
                if((file = open(filename.c_str(), O_RDONLY)) == -1){
                    perror("\nfile");
                    nextFlag = 0;
                }
                dup2(file,STDIN_FILENO);

                tokens.pop();
                string filename2 = tokens.front();

                int file1;
                if((file1 = open(filename2.c_str(), O_CREAT | O_WRONLY | O_APPEND,0644)) == -1){
                    perror("\nfile1");
                    nextFlag = 0;
                }
                dup2(file1,STDOUT_FILENO);

                if(execvp(argv[0], argv.data()) == -1){
                    perror("\nexecvp");
                    nextFlag=0;
                }

                dup2(stdInput,STDIN_FILENO);
                dup2(stdOut,STDOUT_FILENO);
                close(file1);
                close(file);

                while(!tokens.empty())  tokens.pop();
            }
        }else{

            while(!tokens.empty() && tokens.front()!=">>"){
                argv.push_back(strdup(tokens.front().c_str()));
                tokens.pop();
            }
            argv.push_back(nullptr);
            tokens.pop();

            string filename = tokens.front();
            tokens.pop();
                
            int file;
            if((file = open(filename.c_str(), O_CREAT | O_WRONLY | O_APPEND,0644)) == -1){
                perror("\nfile");
                nextFlag = 0;
            }
            dup2(file,STDOUT_FILENO);

            if(execvp(argv[0], argv.data()) == -1){
                perror("\nexecvp");
                nextFlag=0;
            }

            dup2(stdOut,STDOUT_FILENO);
            close(file);
            while(!tokens.empty())  tokens.pop();
        }
    }else if(inResult.find(">")!=string::npos){
        if(inResult.find("<")!=string::npos){
            if(inResult.find("<") > inResult.find(">")){
                string err = "Incorrect order of redirection tokens!!";
                write(1,err.c_str(),err.size());
            }else{
                // Command before '<'
                while(!tokens.empty() && tokens.front()!="<"){
                    argv.push_back(strdup(tokens.front().c_str()));
                    tokens.pop();
                }
                argv.push_back(nullptr);

                tokens.pop();
                
                string filename = tokens.front();
                tokens.pop();
                
                int file;
                if((file = open(filename.c_str(), O_RDONLY)) == -1){
                    perror("\nfile");
                    nextFlag = 0;
                }
                dup2(file,STDIN_FILENO);

                // 2nd cammand
                tokens.pop();
                string filename2 = tokens.front();

                int file1;
                if((file1 = open(filename2.c_str(), O_CREAT | O_WRONLY | O_TRUNC,0644)) == -1){
                    perror("\nfile1");
                    nextFlag = 0;
                }
                dup2(file1,STDOUT_FILENO);

                if(execvp(argv[0], argv.data()) == -1){
                    perror("\nexecvp");
                    nextFlag=0;
                }

                dup2(stdInput,STDIN_FILENO);
                dup2(stdOut,STDOUT_FILENO);
                close(file1);
                close(file);

                while(!tokens.empty())  tokens.pop();

            }
        }else{
            // Command before '>>'
            while(!tokens.empty() && tokens.front()!=">"){
                argv.push_back(strdup(tokens.front().c_str()));
                tokens.pop();
            }
            argv.push_back(nullptr);

            tokens.pop();

            string filename = tokens.front();
            tokens.pop();
                
            int file;
            if((file = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC,0644)) == -1){
                perror("\nfile");
                nextFlag = 0;
            }
            dup2(file,STDOUT_FILENO);

            if(execvp(argv[0], argv.data()) == -1){
                perror("\nexecvp");
                nextFlag=0;
            }

            dup2(stdOut,STDOUT_FILENO);
            close(file);
            while(!tokens.empty())  tokens.pop();

        }

    }else{
        while(!tokens.empty() && tokens.front()!="<"){
            argv.push_back(strdup(tokens.front().c_str()));
            tokens.pop();
        }
        argv.push_back(nullptr);

        tokens.pop();
                
        string filename = tokens.front();
        tokens.pop();
                
        int file;
        if((file = open(filename.c_str(), O_RDONLY)) == -1){
            perror("\nfile");
            nextFlag = 0;
        }
        dup2(file,STDIN_FILENO);
        write(1,"\n",1);
        if(execvp(argv[0], argv.data()) == -1){
            perror("\nexecvp");
            nextFlag=0;
        }
        
        dup2(stdInput,STDIN_FILENO);
        close(file);

        while(!tokens.empty())  tokens.pop();
    }

    return;
}