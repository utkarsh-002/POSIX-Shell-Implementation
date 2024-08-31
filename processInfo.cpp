#include <iostream>
#include <unistd.h>
#include <sys/sysctl.h> // for sysctl
#include <libproc.h>
#include <bits/stdc++.h>
#include <mach/mach.h> 

using namespace std;

void getProcessInfo(queue<string>& tokens){

    tokens.pop();
    int cnt,pid;
    cnt=0;
    while(!tokens.empty() && tokens.front()!=";"){
        cnt++;
        try {
            int pid = stoi(tokens.front());
        } catch (const invalid_argument& err) {
        perror("Error");
        return;
    } 
        pid = stoi(tokens.front());
        tokens.pop();
    }
    
    if(cnt>1){
        string err = "Too many parameters!!";
        write(1,err.c_str(),err.size());
        return;
    }else if(cnt==0)
        pid = getpid();

    tokens.pop();
    char exePath[PROC_PIDPATHINFO_MAXSIZE];
    string pidStr = "pid -- " + to_string(pid);
    string statusStr = "Process Status -- ";
    string memStr = "memory -- ";
    string exeStr = "Executable Path -- ";

    int mib[4];
    struct kinfo_proc kp;
    size_t len = sizeof(kp);
    
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PID;
    mib[3] = pid;
    if (sysctl(mib, 4, &kp, &len, NULL, 0) == -1) {
        perror("Process status");
        return;
    } else {
        char status = kp.kp_proc.p_stat;
        switch (status) {
            case SRUN:
                statusStr += "R";
                break;
            case SSLEEP:
                statusStr += "S";
                break;
            case SSTOP:
                statusStr += "T";
                break;
            case SZOMB:
                statusStr += "Z";
                break;
        }

        if(statusStr.find("S") || statusStr.find("R")){
            int pgid = getpgid(pid);
            int fgpgid = tcgetpgrp(STDIN_FILENO); 

            if(pgid == fgpgid)
                statusStr+="+";
        }
    }

    task_t task;
    if (task_for_pid(mach_task_self(), pid, &task) != KERN_SUCCESS) {
        perror("pid");
        return;
    } else {
        struct task_basic_info t_info;
        mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

        if (task_info(task, TASK_BASIC_INFO, (task_info_t)&t_info, &t_info_count) == KERN_SUCCESS) {
            memStr += to_string(t_info.virtual_size);

        } else {
            perror("Memory access:");
            return;
        }

        mach_port_deallocate(mach_task_self(), task);
    }
    if (proc_pidpath(pid, exePath, sizeof(exePath)) <= 0) {
        perror("Executable Path");
        return;
    } else {
        exeStr+=exePath;
    }

    // write(1,pidStr.c_str(),pidStr.size());
    // write(1, "\n", 1);
    // write(1,statusStr.c_str(),statusStr.size());
    // write(1, "\n", 1);
    // write(1,memStr.c_str(),memStr.size());
    // write(1, "\n", 1);
    // write(1,exeStr.c_str(),exeStr.size());
    // write(1, "\n", 1);
    cout<<"\n"<<pidStr<<"\n"<<statusStr<<"\n"<<memStr<<"\n"<<exeStr<<"\n";
}