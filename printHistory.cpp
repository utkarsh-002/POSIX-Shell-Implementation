#include <iostream>
#include <unistd.h>
#include<bits/stdc++.h>
#include <unistd.h>

using namespace std;

int hisIdx;

void upArrow(vector<string> history,int& inputLen,int& upIdx){
    int strLen = inputLen;
    if(upIdx !=0){
        while(strLen>0){
            write(1,"\b \b",3);
            strLen--;
        }
        inputLen = history[upIdx].size();
        write(1,history[upIdx].c_str(),inputLen);
        upIdx--;
    }

}

void historyList(queue<string>& tokens,vector<string> hisVec){
    tokens.pop();
    int num = 0;
    if(!tokens.empty() && tokens.front()!=";"){
       try{
        num = stoi(tokens.front());
        tokens.pop();
       }catch(const invalid_argument& error){
        cout<<"Invalid argument: "<<error.what();
        return;
       }
    }
    cout<<num;

    if(!tokens.empty() && tokens.front()!=";"){
        cout<<"Too many arguments!!\n";
        while (!tokens.empty()) {
            tokens.pop();
        }
    }else if(!tokens.empty())
        tokens.pop();

    int numHistory = hisVec.size();
    if(num>0)
        for(int i = (numHistory-num-1)> 0 ? numHistory-num-1 : 0;i<numHistory;i++)
            cout<<"\n"<<hisVec[i];
    else{
        for(int i = (numHistory-10)> 0 ? numHistory-10 : 0;i<numHistory;i++)
            cout<<"\n"<<hisVec[i];
    }

}