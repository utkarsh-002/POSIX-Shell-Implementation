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
#include "ioRedirection.cpp"
#include "pipe.cpp"

#define inSize 1024
bool status = 1;
stack<string> fileList;
vector<string> echoStr;
vector<string> hisVec;
int upIdx,inputLen,frdPid;
queue<string> cmdToks;
int nextFlag=0;
int printFlag =1;
char input[inSize], tempChar[inSize], homeDir[PATH_MAX], sysName[1026];
string curDirPath="~";
string prevDir="~";
string outResult, dirPath, strDir, absPath, prompt, histPath, inResult;