#include <iostream>
#include <unistd.h>
#include <sys/sysctl.h>
#include <libproc.h>
#include <bits/stdc++.h>

using namespace std;

void getProcessInfo(queue<string>& tokens) {

    tokens.pop();
    int cnt, pid;
    cnt = 0;

    while (!tokens.empty() && tokens.front() != ";") {
        cnt++;
        try {
            pid = stoi(tokens.front());
        } catch (const invalid_argument& err) {
            perror("Error");
            return;
        }
        tokens.pop();
    }

    if (cnt > 1) {
        string err = "\nToo many parameters!!";
        write(1, err.c_str(), err.size());
        return;
    } else if (cnt == 0) {
        pid = getpid();
    }

    char exePath[PROC_PIDPATHINFO_MAXSIZE];
    string pidStr = "pid - " + to_string(pid);
    string statusStr = "Process Status - ";
    string memStr = "memory - ";
    string exeStr = "Executable Path - ";

    // Get process info using proc_bsdinfo
    struct proc_bsdinfo bsdinfo;
    int ret = proc_pidinfo(pid, PROC_PIDTBSDINFO, 0, &bsdinfo, sizeof(bsdinfo));
    if(ret <= 0){
        string err = "\nProcess info retrieval failed";
        write(1,err.c_str(),err.size());
        return;
    }

    // Determine process status
    switch (bsdinfo.pbi_status) {
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
        default:
            statusStr += "?";  // Unknown status
    }

    if (statusStr.find("S") != string::npos || statusStr.find("R") != string::npos) {
        int pgid = bsdinfo.pbi_pgid;
        int fgpgid = tcgetpgrp(STDIN_FILENO);

        if (pgid == fgpgid)
            statusStr += "+";
    }

    // Get memory usage (resident set size)
       struct proc_taskinfo taskinfo;

    ret = proc_pidinfo(pid, PROC_PIDTASKINFO, 0, &taskinfo, sizeof(taskinfo));

    if (ret <= 0) {

        perror("\nMemory info retrieval failed");

        return;

    }

    memStr += to_string(taskinfo.pti_resident_size);

    // Get executable path
    if (proc_pidpath(pid, exePath, sizeof(exePath)) <= 0) {
        perror("\nExecutable Path failed");
        return;
    } else {
        exeStr += exePath;
    }

    // Output the process information
    write(1, "\n", 1);
    write(1, pidStr.c_str(), pidStr.size());
    write(1, "\n", 1);
    write(1, statusStr.c_str(), statusStr.size());
    write(1, "\n", 1);
    write(1, memStr.c_str(), memStr.size());
    write(1, "\n", 1);
    write(1, exeStr.c_str(), exeStr.size());
}
