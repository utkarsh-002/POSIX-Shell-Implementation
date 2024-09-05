#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;
extern string inResult;
extern int nextFlag;

void IORedir(queue<string>& tokens) {

    vector<char*> argv;

    int stdInput = dup(STDIN_FILENO);
    int stdOut = dup(STDOUT_FILENO);
    string filename, filename2;

    if(inResult.find(">>") != string::npos) {
        if(inResult.find("<") != string::npos) {
            if(inResult.find("<") > inResult.find(">>")) {
                string err = "\nIncorrect order of redirection tokens!!";
                write(1, err.c_str(), err.size());
                return;
            } else {
                while(!tokens.empty() && tokens.front() != "<") {
                    argv.push_back(strdup(tokens.front().c_str()));
                    tokens.pop();
                }
                argv.push_back(nullptr);
                tokens.pop();
                
                filename = tokens.front();
                tokens.pop();
                
                int file = open(filename.c_str(), O_RDONLY);
                if(file == -1) {
                    perror("\nfile");
                    nextFlag = 0;
                    return;
                }
                dup2(file, STDIN_FILENO);

                tokens.pop();
                filename2 = tokens.front();

                int file1 = open(filename2.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
                if(file1 == -1) {
                    perror("\nfile1");
                    nextFlag = 0;
                    return;
                }
                dup2(file1, STDOUT_FILENO);

                pid_t pid = fork();
                if (pid == 0) { // Child process
                    if(execvp(argv[0], argv.data()) == -1) {
                        perror("\nexecvp");
                        nextFlag = 0;
                        exit(1);
                    }
                } else if (pid > 0) { // Parent process
                    waitpid(pid, nullptr, 0);
                } else {
                    perror("fork failed");
                    nextFlag = 0;
                }

                dup2(stdInput, STDIN_FILENO);
                dup2(stdOut, STDOUT_FILENO);
                close(file1);
                close(file);

                while(!tokens.empty()) tokens.pop();
            }
        } else {
            while(!tokens.empty() && tokens.front() != ">>") {
                argv.push_back(strdup(tokens.front().c_str()));
                tokens.pop();
            }
            argv.push_back(nullptr);
            tokens.pop();

            filename = tokens.front();
            tokens.pop();

            int file = open(filename.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
            if(file == -1) {
                perror("\nfile");
                nextFlag = 0;
                return;
            }
            dup2(file, STDOUT_FILENO);

            pid_t pid = fork();
            if (pid == 0) { // Child process
                if(execvp(argv[0], argv.data()) == -1) {
                    perror("\nexecvp");
                    nextFlag = 0;
                    exit(1);
                }
            } else if (pid > 0) { // Parent process
                waitpid(pid, nullptr, 0);
            } else {
                perror("fork failed");
                nextFlag = 0;
            }

            dup2(stdOut, STDOUT_FILENO);
            close(file);
            while(!tokens.empty()) tokens.pop();
        }
    } else if(inResult.find(">") != string::npos) {
        if(inResult.find("<") != string::npos) {
            if(inResult.find("<") > inResult.find(">")) {
                string err = "Incorrect order of redirection tokens!!";
                write(1, err.c_str(), err.size());
                return;
            } else {
                while(!tokens.empty() && tokens.front() != "<") {
                    argv.push_back(strdup(tokens.front().c_str()));
                    tokens.pop();
                }
                argv.push_back(nullptr);

                if(!tokens.empty() && tokens.front() == "<") tokens.pop();

                if(!tokens.empty() && tokens.front().find(">") == string::npos) {
                    filename = tokens.front();
                    tokens.pop();
                }

                int file = open(filename.c_str(), O_RDONLY);
                if(file == -1) {
                    perror("\nfile");
                    nextFlag = 0;
                    return;
                }
                dup2(file, STDIN_FILENO);

                if(!tokens.empty() && tokens.front() == ">") tokens.pop();

                if(!tokens.empty())
                    filename2 = tokens.front();

                int file1 = open(filename2.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if(file1 == -1) {
                    perror("\nfile1");
                    nextFlag = 0;
                    return;
                }
                dup2(file1, STDOUT_FILENO);

                pid_t pid = fork();
                if (pid == 0) { // Child process
                    if(execvp(argv[0], argv.data()) == -1) {
                        perror("\nexecvp");
                        nextFlag = 0;
                        exit(1);
                    }
                } else if (pid > 0) { // Parent process
                    waitpid(pid, nullptr, 0);
                } else {
                    perror("fork failed");
                    nextFlag = 0;
                }

                dup2(stdInput, STDIN_FILENO);
                dup2(stdOut, STDOUT_FILENO);
                close(file1);
                close(file);

                while(!tokens.empty()) tokens.pop();
            }
        } else {
            while(!tokens.empty() && tokens.front() != ">") {
                argv.push_back(strdup(tokens.front().c_str()));
                tokens.pop();
            }
            argv.push_back(nullptr);

            if(!tokens.empty() && tokens.front() == ">") tokens.pop();

            if(!tokens.empty()) {
                filename = tokens.front();
                tokens.pop();
            }

            int file = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if(file == -1) {
                perror("\nfile");
                nextFlag = 0;
                return;
            }
            dup2(file, STDOUT_FILENO);

            pid_t pid = fork();
            if (pid == 0) { // Child process
                if(execvp(argv[0], argv.data()) == -1) {
                    perror("\nexecvp");
                    nextFlag = 0;
                    exit(1);
                }
            } else if (pid > 0) { // Parent process
                waitpid(pid, nullptr, 0);
            } else {
                perror("fork failed");
                nextFlag = 0;
            }

            dup2(stdOut, STDOUT_FILENO);
            close(file);

            while(!tokens.empty()) tokens.pop();
        }
    } else {
        string str = "Hellow wello";
        write(1, str.c_str(), str.size());

        while(!tokens.empty() && tokens.front() != "<") {
            argv.push_back(strdup(tokens.front().c_str()));
            tokens.pop();
        }
        argv.push_back(nullptr);

        tokens.pop();

        string filename = tokens.front();
        tokens.pop();

        int file = open(filename.c_str(), O_RDONLY);
        if(file == -1) {
            perror("\nfile");
            nextFlag = 0;
            return;
        }
        dup2(file, STDIN_FILENO);

        pid_t pid = fork();
        if (pid == 0) { // Child process
            if(execvp(argv[0], argv.data()) == -1) {
                perror("\nexecvp");
                nextFlag = 0;
                exit(1);
            }
        } else if (pid > 0) { // Parent process
            waitpid(pid, nullptr, 0);
        } else {
            perror("fork failed");
            nextFlag = 0;
        }

        dup2(stdInput, STDIN_FILENO);
        close(file);

        while(!tokens.empty()) tokens.pop();
    }

    dup2(stdInput, STDIN_FILENO);
    dup2(stdOut, STDOUT_FILENO);
}
