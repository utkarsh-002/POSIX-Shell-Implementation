#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
// #include <vector>
// #include <string>
// #include <cstring>
// #include <ctime>
// #include <iomanip>
#include<bits/stdc++.h>

using namespace std;

void listDetails(const string name,const struct stat detail) {
    struct passwd *pwd = getpwuid(detail.st_uid);
    struct group  *grp = getgrgid(detail.st_gid);
    struct tm *time = localtime(&detail.st_mtime);
    char timebuf[80];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", time);

    cout << ((S_ISDIR(detail.st_mode)) ? "d" : "-");
    cout << ((detail.st_mode & S_IRUSR) ? "r" : "-");
    cout << ((detail.st_mode & S_IWUSR) ? "w" : "-");
    cout << ((detail.st_mode & S_IXUSR) ? "x" : "-");
    cout << ((detail.st_mode & S_IRGRP) ? "r" : "-");
    cout << ((detail.st_mode & S_IWGRP) ? "w" : "-");
    cout << ((detail.st_mode & S_IXGRP) ? "x" : "-");
    cout << ((detail.st_mode & S_IROTH) ? "r" : "-");
    cout << ((detail.st_mode & S_IWOTH) ? "w" : "-");
    cout << ((detail.st_mode & S_IXOTH) ? "x" : "-");
    cout << " " << setw(3) << detail.st_nlink;

    cout << " " << setw(8) << (pwd ? pwd->pw_name : "");
    cout << " " << setw(8) << (grp ? grp->gr_name : "");

    cout << " " << setw(8) << detail.st_size;

    cout << " " << timebuf;

    cout << " " << name << endl;
}

void iterateDocs(string path,int shwHidden,int shwDetail){
    DIR *dptr = opendir(path.c_str());
    if (dptr == nullptr) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dptr)) != nullptr) {

        if (!shwHidden && entry->d_name[0] == '.') 
            continue;

        string fullPath = path + "/" + entry->d_name;
        struct stat fileDetail;
        if (stat(fullPath.c_str(), &fileDetail) < 0) {
            perror("stat");
            continue;
        }

        if (shwDetail) {
            listDetails(entry->d_name, fileDetail);
        } else {
            cout << entry->d_name << "\n";
        }
    }
    closedir(dptr);
}

void listContents(queue<string>& tokens,string homeDir){
    vector<string> flags;
    string tempStr;
    tokens.pop();
    while(!tokens.empty() && tokens.front()!=";"){
        if(tokens.front()!="-"){
            tempStr+=tokens.front();
        }else{
            if(tempStr.size()>0){
                flags.push_back(tempStr);
                tempStr.clear();
            }
            tempStr = tokens.front();
        }
        tokens.pop();
    }
    if(tempStr.size()>0)
        flags.push_back(tempStr);
    if(!tokens.empty()) 
        tokens.pop();

    char currDir[PATH_MAX];
    getcwd(currDir,sizeof(currDir));

    if(flags.size()==0){
        iterateDocs(".",0,0);
    }else if(flags.size()==1){
        if(flags[0]==".")
            iterateDocs(".",0,0);
        else if(flags[0]=="..")
            iterateDocs("..",0,0);
        else if(flags[0]=="~")
            iterateDocs(homeDir,0,0);
        else if(flags[0]=="-a")
            iterateDocs(".",1,0);
        else if(flags[0]=="-l")
            iterateDocs(".",0,1);
        else if(flags[0]=="-al" || flags[0]=="la")
            iterateDocs(".",1,1);
        else{
            if(flags[0][0]!='-'){
                string path = string(currDir) + "/" + flags[0];
                iterateDocs(path,0,0);
            }else{
                cout<<"\nInvalid Directory/File_name!!";
            }
        }
    }else if(flags.size()==2){
        if((flags[0]=="-l" && flags[0]=="-a") || (flags[0]=="-a" && flags[0]=="-l"))
            iterateDocs(".",1,1);
    }else
        cout<<"Too many arguments!!";
    
}