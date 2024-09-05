#include <termios.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <iostream>
#include <dirent.h>
// #include "printHistory.cpp"

using namespace std;

string cmdList[] = {"ls", "echo", "cat", "search", "history", "cd", "pwd", "pinfo", "exit"};
vector<string> files;
vector<string> matches;
extern int upIdx;
extern vector<string> hisVec;
extern string inResult,histPath;
extern int inputLen,nextFlag;
extern void upArrow(vector<string> history,int& inputLen,int& upIdx,string& inResult,string outResult);
DIR* dptr;
struct dirent* dirStruct;

//Search function for auto complete
void autoComplete(string& input) {
    for (int i=0;i<10;i++) {
        if (cmdList[i].find(input) == 0)
            matches.push_back(cmdList[i]);
    }
    if(matches.size()>0)
        return;
    for (int i=0;i<files.size();i++) {
        if (files[i].find(input) == 0)
            matches.push_back(files[i]);
    }
}

void readHistory(){
    files.clear();

    //reading files and directories in current directory
    dptr = opendir(".");
    // dirStruct = readdir(dptr);
    while((dirStruct = readdir(dptr))!=nullptr){
        if (string(dirStruct->d_name) == "." || string(dirStruct->d_name) == "..")
            continue;
        if(find(files.begin(),files.end(),string(dirStruct->d_name))==files.end())
            files.push_back(string(dirStruct->d_name));
    }
    closedir(dptr);
}

//Function to read input character-by-character
void termInput(string outResult) {
    struct termios oggProp, modProp;
    queue<string> token;
    char c;
    string temp;
    string clr = "\033[2K\033[0G";
    int esc,brace,inLen;
    esc=brace=inLen=0;

    //Set history idx to last element in history vector at start of input
    upIdx = hisVec.size()-1;
/////////////////////////////////////////
    files.clear();

    //reading files and directories in current directory
    dptr = opendir(".");
    // dirStruct = readdir(dptr);
    while((dirStruct = readdir(dptr))!=nullptr){
        if (string(dirStruct->d_name) == "." || string(dirStruct->d_name) == "..")
            continue;
        if(find(files.begin(),files.end(),string(dirStruct->d_name))==files.end())
            files.push_back(string(dirStruct->d_name));
    }
    closedir(dptr);
///////////////////////////////////////
    inResult = "";
    tcgetattr(0, &oggProp);
    modProp = oggProp;

    modProp.c_lflag &= ~(ICANON | ECHO);
    modProp.c_cc[VMIN] = 1;
    modProp.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &modProp);
    
    while (read(0, &c, 1) > 0 && c != '\n') {

        //auto complete
        if(c =='\t'){
            matches.clear();
            // readHistory();
            autoComplete(temp);
            if(matches.size()==1){
                write(1,clr.c_str(),clr.size());
                write(1,outResult.c_str(),outResult.size());
                inResult+=matches[0];
                write(1,&inResult,inResult.length());
                temp.clear();
            }else if(matches.size()>1){
                write(1,"\n",1);
                for(int i=0;i<matches.size();i++){
                    write(1,&matches[i],matches[i].length());
                    write(1," ",1);
                }
                write(1,":",1);
                char n[10];
                read(0,n,10);
                write(1,n,strlen(n));
                if(atoi(n)<matches.size()){
                    inResult+=matches[atoi(n)-1];
                    temp.clear();
                    write(1,"\n",1);
                    write(1,outResult.c_str(),outResult.size());
                    write(1,&inResult,inResult.length());
                }else{
                    cout<<"\ninvalid input!!";
                    nextFlag=0;
                    return;
                }

            }
        }


        //Implement backspace
        if(c==127 && inLen>0){
            write(1,"\b \b",3);
            if(temp.size()>0)
                temp.pop_back();
            else
                inResult.pop_back();
            inLen--;
            continue;
        }else if(c==4){
            ofstream history(histPath);
            for(int i=0;i<hisVec.size();i++){
                history<<hisVec[i]<<"\n";
            }
            history.close();
            _exit(0);
        }

        //Implement up arrow for history
        if(c==27){
            read(0,&c,1);
            if(c=='['){
                read(0,&c,1);
                if(c == 'A'){
                    upArrow(hisVec,inputLen,upIdx,inResult,outResult);
                    inLen=inputLen;
                    esc=brace=0;
                    continue;
                }
            }
        }
        
        else if(c>=32 && c<=126){
            if(c == ' ' || c==';'){
                temp+=c;
                inResult+=temp;
                temp="";
            }else
                temp+=c;
            write(1,&c,1);
            inLen++;
        }
    }
    inResult+=temp;
    if(inLen>0)
        inputLen = inLen;
    tcsetattr(0, TCSANOW, &oggProp);

    //updating history
    if(inResult.size()>0){
        if(hisVec.size()<20)
            hisVec.push_back(inResult);
        else if(inResult.size()>0){
            hisVec.erase(hisVec.begin());
            hisVec.push_back(inResult);
        }
    }


}
