#include <unistd.h>
#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

// bool errorHandler(queue<string>& tokens){
//     bool flag;
//     if(!tokens.empty() && tokens.front()==";"){
//         tokens.pop();
//         flag = true;
//     }
//     else if(!tokens.empty()){
//         string err = "\nTo many arguments!!";
//         write(1,err.c_str(),err.size());
//         flag = false;
//     }
//     return flag;
// }

string changeDir(queue<string>& cmdTokens,string curDirPath,string homeDir){
    // string sout = "enter path:";
    // write(1,&sout,sout.length());
    string path;
    struct dirent* dirStruct;
    string temp;
    cmdTokens.pop();
    while(!cmdTokens.empty() && cmdTokens.front()!=";"){
        temp = cmdTokens.front();
        cmdTokens.pop();
        if(temp==".") return curDirPath;
        if(temp == ".."){
            if(chdir("..")<0){
                perror("\ncd");
            }else{
                    if(curDirPath == "~"){
                        cmdTokens.pop();
                        return "Home";
                    }else{
                        int lSlash = curDirPath.find_last_of('/');
                        if(lSlash<curDirPath.size())
                            curDirPath.erase(lSlash);
                        cmdTokens.pop();
                        return curDirPath;
                    }
            }
        }else if(temp == "-"){
            cmdTokens.pop();
            return "prev";
        }else if(temp == "~"){
            if(chdir(homeDir.c_str())<0)
                perror("\ncd");
            else{
                cmdTokens.pop();
                return "~";
            }
        }else{
            string str = curDirPath;
            str.erase(str.find('~'),1);
            path = homeDir + str + "/" + temp;
            // write(1,path.c_str(),path.size());
            if(chdir(path.c_str())<0){
                perror("\ncd");
            }
            else{
                cmdTokens.pop();
                return curDirPath+"/"+temp;
            }
            
        }
    }
    cmdTokens.pop();
    return curDirPath;
}