#include <bits/stdc++.h>

using namespace std;

queue<string> CreTokens(char command[]) {
    char *cmds;
    // strcpy(cmds,command);
    queue<string> tokens;
    cmds = strtok(command, ";");

    while (cmds != nullptr) {
        char* tokns = cmds;
        cmds = strtok(NULL, ";");
        tokns = strtok(tokns," ");
        while(tokns!=nullptr){
            tokens.push(tokns);
            tokns = strtok(NULL," ");
        }
        tokens.push(";");
    }
    // while(!tokens.empty()){
    //         cout<<tokens.front()<<"\n";
    //         tokens.pop();
    //     }";
    return tokens;
}