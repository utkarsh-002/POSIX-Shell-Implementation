#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>

using namespace std;

#define inSize 1024
bool status = 1;
stack<string> fileList;
vector<string> echoStr;
vector<string> hisVec;
int upIdx,inputLen;
queue<string> cmdToks;
int nextFlag=0;
int printFlag =1;
char input[inSize], tempChar[inSize], homeDir[PATH_MAX], sysName[1026];
string curDirPath="~";
string prevDir="~";
string outResult, dirPath, strDir, absPath, prompt, histPath, inResult;

#include "creTokens.cpp"
#include "printHistory.cpp"
#include "termInput.cpp"
#include "changeDir.cpp"
#include "echo.cpp"
#include "getCurrDirPath.cpp"
#include "processInfo.cpp"
#include "search.cpp"
#include "listContents.cpp"
#include "fg_bgProcess.cpp"

void chooseCmds(){
    if(cmdToks.front()=="cd"){
            strDir = homeDir;
            dirPath = changeDir(cmdToks,curDirPath,strDir);
            
            // if(curDirPath==dirPath) 
            //     continue;

            if(dirPath == "Home"){
                write(1,"\n",1);
                write(1,absPath.c_str(),absPath.size());
                printFlag = 0;
                nextFlag=0;
            }else if(dirPath== "prev" ){
                string str = prevDir;
                str.erase(str.find('~'),1);
                strDir = strDir + str;
                if(chdir(strDir.c_str())<0)
                    perror("cd");
                else{
                    string tempStr = curDirPath;
                    curDirPath = prevDir;
                    prevDir = tempStr;
                    outResult=prompt + curDirPath +">";
                }
            }else if(curDirPath!=dirPath){
                prevDir = curDirPath;
                curDirPath = dirPath;
                outResult= prompt +  curDirPath +">";
            }
        }
        
        else if(cmdToks.front() == "echo"){
            string echoRes = echoFun(cmdToks,tempChar);
            write(1,"\n",1);
            write(1,echoRes.c_str(),echoRes.size());
        }else if(cmdToks.front() == "pwd"){
            getPwd(cmdToks,curDirPath);
        }else if(cmdToks.front() == "pinfo"){
            getProcessInfo(cmdToks);
        }else if(cmdToks.front() == "search"){
            searchItem(cmdToks,curDirPath);
        }else if(cmdToks.front() == "ls"){
            cout<<"\n";
            nextFlag=0;
            listContents(cmdToks,homeDir);
            // continue;
        }else if(cmdToks.front() == "history"){
            historyList(cmdToks,hisVec);
        }
}


int main(int argc,char* argv[]){
    
    gethostname(sysName, sizeof(sysName));

    uid_t uid = getuid();
    struct passwd *userDetail = getpwuid(uid);

    char *dotLocal = strstr(sysName, ".local");
    getcwd(homeDir,sizeof(homeDir));

    if(dotLocal)
        *dotLocal = '\0';

    prompt = userDetail->pw_name;
    prompt+="@";
    prompt+=sysName;
    prompt+=":";
    
    
    outResult = prompt+curDirPath+">";
    absPath = prompt +  homeDir +">";

    histPath = string(homeDir) + "/history.txt";
    ifstream history(histPath);
    if(history.is_open()){
        string line;
        while (getline(history, line)) { 
            hisVec.push_back(line);
            hisIdx++;
        }
    }
    hisIdx--;
    history.close(); 

    while(status){

        if(cmdToks.empty()){
            if(nextFlag){
                write(1,"\n",1);
            }
            nextFlag = 1;
            if(printFlag)  write(1,outResult.c_str(),outResult.size());
            printFlag=1;
            termInput(outResult);
            strcpy(input,inResult.c_str());
            strcpy(tempChar,input);
            cmdToks = CreTokens(input);
        }

        if(cmdToks.empty()) continue;

        if(cmdToks.front()==";")
            cmdToks.pop();
        
        if(cmdToks.front()=="exit" || cmdToks.front()=="4"){
            ofstream history(histPath);
            for(int i=0;i<hisVec.size();i++){
                history<<hisVec[i]<<"\n";
            }
            history.close();
            break;
        }

//added IO
        // if(inResult.find("&") != string::npos){
        //         fg_bg(cmdToks,1);
        // }else if(inResult.find(">") != string::npos || inResult.find(">>") != string::npos){
            
        // }

//
        chooseCmds();
        // else{
        //     fg_bg(cmdToks,0);
        // }
    }
    return 0;
}