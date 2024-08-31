#include <unistd.h>
#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/types.h>
#include <iostream>

using namespace std;
queue<string> dir;

bool searchFun(DIR* dptr,string name,int cnt){
    struct dirent* dirStruct;
    
    while((dirStruct = readdir(dptr))!=nullptr){
        if (string(dirStruct->d_name) == "." || string(dirStruct->d_name) == "..")
            continue;
        // cout<<"\n"<<dirStruct->d_name;
        if(string(dirStruct->d_name)==name){
            return true;
        }else{
            if(dirStruct->d_type == DT_DIR && cnt==1)
                dir.push(dirStruct->d_name);
        }
    }
    return false;
}

void searchItem(queue<string>& tokens,string currDirPath){
    struct dirent* dirStruct;
    DIR* dptr;
    tokens.pop();
    string name;
    int cnt=0;
    int childDir = 1;
    string currDir = ".";

    while(!tokens.empty() && tokens.front()!=";"){
        name = tokens.front();
        tokens.pop();
        cnt++;
    }
    if(cnt==1){
        if(!tokens.empty())
            tokens.pop();
    }else{
        cout<<"\nInvalid arguments!!\n";
        return;
    }
    
    dptr = opendir(currDir.c_str());
    
    if(searchFun(dptr,name,childDir)){
            cout<<"\nTrue\n";
            closedir(dptr);
            chdir(currDirPath.c_str());
            return;
    }
    while(!dir.empty()){
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        currDir = string(cwd) + "/" + dir.front();
        dir.pop();
        closedir(dptr);
        dptr = opendir(currDir.c_str());
        childDir--;
        if(searchFun(dptr,name,childDir)){
            cout<<"\nTrue\n";
            closedir(dptr);
            chdir(currDirPath.c_str());
            return;
        }
        // cout<<"\n"<<dir.front()<<"\n";
    }

    closedir(dptr);
    // while((dirStruct = readdir(dptr))!=nullptr){
    //     if (string(dirStruct->d_name) == "." || string(dirStruct->d_name) == "..")
    //         continue;
    //     if(string(dirStruct->d_name)==name){
    //         cout<<"\nTrue\n";
    //         return;
    //     }else{
    //         if(dirStruct->d_type == DT_DIR)
    //             dir.push(dirStruct->d_name);
    //     }
    // }
    cout<<"\nFalse\n";
}