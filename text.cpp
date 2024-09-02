#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <bits/stdc++.h>

using namespace std;

void IORedir(queue<string>& tokens,int flag) {
    
    
    int file;
    if((file = open("exam.txt", O_CREAT | O_WRONLY)) == -1)
        perror("file");
    
    printf("the file decriptor is %d",file);
    dup2(file,STDOUT_FILENO);
    close(file);

    return 0;
}