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
    // for(int i=5;i<strlen(input);i++)
    //     res+=input[i];
    int i=5;
    bool evencnt =1;
    int len = strlen(input);

    while(i < len && input[i]== ' ')  i++;

    while(i < len){
        if(input[i] == '"'){
            evencnt = !evencnt;
            i++;
            continue;
        }

        // while(i<strlen(input) && input[i] != '"'){
        //     if(input[i] == ' ' && res.back() == ' ' && evencnt)    continue;
        //     res+=input[i];
        //     i++;
        // }

        if (input[i] == ' ' && (res.empty() || res.back() == ' ') && evencnt) {
            i++;
            continue;
        }

        res += input[i];
        i++;
    }
    
    return res;
}