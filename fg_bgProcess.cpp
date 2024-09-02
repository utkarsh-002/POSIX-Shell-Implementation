#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<bits/stdc++.h>

using namespace std;

int main(){
    cout<<"\nhello world!!";
    int id = fork();
    if(id!=0)
        cout<<"\nHello after fork "<<id;
    else 
        cout<<"\nHello after fork "<<id;
    return 0;
}