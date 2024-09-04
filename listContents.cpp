#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include<bits/stdc++.h>

using namespace std;

extern int nextFlag;

void listDetails(const string name,const struct stat detail) {
    struct passwd *pwd = getpwuid(detail.st_uid);
    struct group  *grp = getgrgid(detail.st_gid);
    struct tm *time = localtime(&detail.st_mtime);
    char timebuf[80];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", time);

    // cout <<"\n"<<((S_ISDIR(detail.st_mode)) ? "d" : "-");
    // cout << ((detail.st_mode & S_IRUSR) ? "r" : "-");
    // cout << ((detail.st_mode & S_IWUSR) ? "w" : "-");
    // cout << ((detail.st_mode & S_IXUSR) ? "x" : "-");
    // cout << ((detail.st_mode & S_IRGRP) ? "r" : "-");
    // cout << ((detail.st_mode & S_IWGRP) ? "w" : "-");
    // cout << ((detail.st_mode & S_IXGRP) ? "x" : "-");
    // cout << ((detail.st_mode & S_IROTH) ? "r" : "-");
    // cout << ((detail.st_mode & S_IWOTH) ? "w" : "-");
    // cout << ((detail.st_mode & S_IXOTH) ? "x" : "-");
    // cout << " " << setw(3) << detail.st_nlink;
    // cout << " " << setw(8) << (pwd ? pwd->pw_name : "");
    // cout << " " << setw(8) << (grp ? grp->gr_name : "");
    // cout << " " << setw(8) << detail.st_size;
    // cout << " " << timebuf;
    // cout << " " << name;

     char buffer[256];
    int len = 0;

    //permissions
    char permissions[11];
    permissions[0] = (S_ISDIR(detail.st_mode)) ? 'd' : '-';
    permissions[1] = (detail.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (detail.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (detail.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (detail.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (detail.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (detail.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (detail.st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (detail.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (detail.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    write(1,"\n",1);
    write(STDOUT_FILENO, permissions, strlen(permissions));

    len = snprintf(buffer, sizeof(buffer), " %3hu", detail.st_nlink);
    write(STDOUT_FILENO, buffer, len);

    const char* ownerName = (pwd && pwd->pw_name) ? pwd->pw_name : "";
    len = snprintf(buffer, sizeof(buffer), " %8s", ownerName);
    write(STDOUT_FILENO, buffer, len);

    const char* groupName = (grp && grp->gr_name) ? grp->gr_name : "";
    len = snprintf(buffer, sizeof(buffer), " %8s", groupName);
    write(STDOUT_FILENO, buffer, len);

    len = snprintf(buffer, sizeof(buffer), " %8lld", static_cast<long long>(detail.st_size));
    write(STDOUT_FILENO, buffer, len);

    len = snprintf(buffer, sizeof(buffer), " %s", timebuf);
    write(STDOUT_FILENO, buffer, len);

    len = snprintf(buffer, sizeof(buffer), " %s", name.c_str());
    write(STDOUT_FILENO, buffer, len);
}

void iterateDocs(string path,int shwHidden,int shwDetail){
    DIR *dptr = opendir(path.c_str());
    if (dptr == nullptr) {
        perror("opendir");
        return;
    }

    struct dirent *entPtr;
    while ((entPtr = readdir(dptr)) != nullptr) {

        if (!shwHidden && entPtr->d_name[0] == '.') 
            continue;

        string fullPath = path + "/" + entPtr->d_name;
        struct stat fileDetail;
        if (stat(fullPath.c_str(), &fileDetail) < 0) {
            perror("stat");
            continue;
        }

        if (shwDetail) {
            listDetails(entPtr->d_name, fileDetail);
        } else {
            write(1,"\n",1);
            write(1,entPtr->d_name,strlen(entPtr->d_name));
        }
    }
    closedir(dptr);
}

void listContents(queue<string>& tokens,string homeDir){
    vector<string> flags;
    vector<string> dirPath;
    string tempStr,path;
    char currDir[PATH_MAX];
    getcwd(currDir,sizeof(currDir));

    tokens.pop();
    
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