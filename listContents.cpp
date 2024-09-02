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

extern int nextFlag;

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
    vector<string> dirPath;
    string tempStr,path;
    tokens.pop();
    char currDir[PATH_MAX];
    getcwd(currDir,sizeof(currDir));
    
    while(!tokens.empty() && tokens.front()!=";"){
        if(tokens.front().find("-")!=string::npos){
            flags.push_back(tokens.front());
        }else{
            dirPath.push_back(tokens.front());
        }
        tokens.pop();
    }

    if(flags.size()>0){
        if(find(flags.begin(),flags.end(),"-al")!=flags.end() || find(flags.begin(),flags.end(),"-la")!=flags.end()){
            if(dirPath.size()==0)
                iterateDocs(".",1,1);
            else{
                for(int i=0;i<dirPath.size();i++){
                    path = string(currDir) + "/" + dirPath[i];
                    string str = dirPath[i] + "\n";
                    if(dirPath.size()>1) write(1,str.c_str(),str.size());
                    write(1,dirPath[i].c_str(),dirPath[i].size());
                    iterateDocs(path,1,1);
                }
            }
        }else if(find(flags.begin(),flags.end(),"-l")!=flags.end() && find(flags.begin(),flags.end(),"-a")!=flags.end()){
              if(dirPath.size()==0)
                iterateDocs(".",1,1);
            else if(dirPath.size()==1){
                path = string(currDir) + "/" + dirPath[0];
                iterateDocs(path,1,1);
            }
            else{
                for(int i=0;i<dirPath.size();i++){
                    path = string(currDir) + "/" + dirPath[i];
                    string str = dirPath[i] + "\n";
                    write(1,str.c_str(),str.size());
                    iterateDocs(path,0,1);
                }
            }
        }else if(find(flags.begin(),flags.end(),"-l")!=flags.end() || find(flags.begin(),flags.end(),"-a")!=flags.end()){
            int flag1,flag2;
            flag1=flag2=0;
            if(find(flags.begin(),flags.end(),"-l")!=flags.end())   flag2=1;
            else flag1=1;
             if(dirPath.size()==0)
                iterateDocs(".",flag1,flag2);
            else if(dirPath.size()==1){
                path = string(currDir) + "/" + dirPath[0];
                iterateDocs(path,flag1,flag2);
            }
            else{
                for(int i=0;i<dirPath.size();i++){
                    path = string(currDir) + "/" + dirPath[i];
                    string str = dirPath[i] + "\n";
                    write(1,str.c_str(),str.size());
                    iterateDocs(path,0,1);
                }
            }
        }
    }else{
        if(dirPath.size()==0)
            iterateDocs(".",0,0);
        else{
            for(int i=0;i<dirPath.size();i++){
                path = string(currDir) + "/" + dirPath[0];
                string str = dirPath[i] + "\n";
                if(dirPath.size()>1)    write(1,str.c_str(),str.size());
                iterateDocs(path,0,0);
            
            }
        }
    }

    if(!tokens.empty() && tokens.front()!=";") 
        tokens.pop();
    
}