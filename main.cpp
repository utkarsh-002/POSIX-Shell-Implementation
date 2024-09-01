#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>

using namespace std;

#define inSize 1024
bool status = 1;
string inResult;
stack<string> fileList;
vector<string> echoStr;
vector<string> hisVec;
string histPath;
int upIdx,inputLen;
int nextFlag=0;
int printFlag =1;

#include "creTokens.cpp"
#include "printHistory.cpp"
#include "termInput.cpp"
#include "changeDir.cpp"
#include "echo.cpp"
#include "getCurrDirPath.cpp"
#include "processInfo.cpp"
#include "search.cpp"
#include "listContents.cpp"

int main(int argc,char* argv[]){
    char sysName[1026];
    gethostname(sysName, sizeof(sysName));

    uid_t uid = getuid();
    struct passwd *userDetail = getpwuid(uid);

    char *dotLocal = strstr(sysName, ".local");
    char homeDir[PATH_MAX];
    getcwd(homeDir,sizeof(homeDir));

    if(dotLocal)
        *dotLocal = '\0';

    queue<string> cmdToks;
    char input[inSize];
    char tempChar[inSize];
    string prompt = userDetail->pw_name;
    prompt+="@";
    prompt+=sysName;
    prompt+=":";
    
    string curDirPath="~";
    string prevDir="~";
    string outResult = prompt+curDirPath+">";
    string absPath = prompt +  homeDir +">";
    // nextFlag=0;
    // printFlag=1;

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
            if(nextFlag)    write(1,"\n",1);
            nextFlag = 1;
            if(printFlag)  write(1,outResult.c_str(),outResult.size());
            printFlag=1;
            termInput(outResult);
            strcpy(input,inResult.c_str());
            strcpy(tempChar,input);
            cmdToks = CreTokens(input);
        }
        // while(!cmdToks.empty()){
        //     cout<<"\n"<<cmdToks.front();
        //     cmdToks.pop();
        // }

        if(cmdToks.empty()) continue;
        
        if(cmdToks.front()=="exit" || cmdToks.front()=="4"){
            ofstream history(histPath);
            for(int i=0;i<hisVec.size();i++){
                history<<hisVec[i]<<"\n";
            }
            history.close();
            break;
        }

        if(cmdToks.front()=="cd"){
            string strDir = homeDir;
            string dirPath = changeDir(cmdToks,curDirPath,strDir);
            
            if(curDirPath==dirPath) continue;

            if(dirPath == "Home"){
                write(1,"\n",1);
                write(1,absPath.c_str(),absPath.size());
                printFlag = 0;
                nextFlag=0;
            }else if(dirPath=="prev"){
                string str = prevDir;
                str.erase(str.find('~'),1);
                strDir = strDir + str;
                // write(1,strDir.c_str(),strDir.size());
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
                // write(1,dirPath.c_str(),dirPath.size());
                curDirPath = dirPath;
                outResult= prompt +  curDirPath +">";
            }
        }
        
        else if(cmdToks.front() == "echo"){
            // write(1,tempChar,strlen(tempChar));
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
            listContents(cmdToks,homeDir);
        }else if(cmdToks.front() == "history"){
            historyList(cmdToks,hisVec);
        }else if(cmdToks.front()!= "cd" && cmdToks.front()!= "ls" && cmdToks.front()!= "echo" && cmdToks.front()!= "pwd" && cmdToks.front()!= "search"){
            // write(1,cmdToks.front().c_str(),cmdToks.front().size());
            string err1 = "Invalid input!!";
            while(!cmdToks.empty() && cmdToks.front()!=";")
                cmdToks.pop();
            if(!cmdToks.empty())    cmdToks.pop();
            // write(1,err1.c_str(),err1.size());
            cout<<"\n"<<err1;
        }
    }
    return 0;
}