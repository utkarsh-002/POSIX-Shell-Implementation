#include <bits/stdc++.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>

using namespace std;

string echoFun(queue<string>& tokens,char* input){
    while(!tokens.empty() && tokens.front()!=";")
        tokens.pop();
    if(!tokens.empty()) 
        tokens.pop();
    string res="";
    for(int i=5;i<strlen(input);i++)
        res+=input[i];
    // write(1,res,strlen(res));
    return res;
}