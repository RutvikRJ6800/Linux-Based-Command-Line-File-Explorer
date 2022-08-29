#include "myHeaderFiles.h"

size_t cursorPos, startPos, endPos,winRows;
stack<string> backStack,forStack;

void openDirectory(){
    // cursor is standing at valid directory
        string newPath=infoVector[cursorPos][0];
        newPath= simplifyPath(newPath);


        listDirectory(newPath.c_str());
        // in future check if error generated or not in listing directory and handle the case
        backStack.push(gbPath);
        gbPath=newPath;
        emptyForStack();// forStack.clear();
        displayWindowResetPointers();
}

void enterKey(){
    if(cursorPos==0 || cursorPos==1){
        return;
    }
    // cursor is standing at valid directory
    if(infoVector[cursorPos][1][0]=='d'){
        openDirectory();
    }
    else if(infoVector[cursorPos][1][0]=='-'){
        // open file
        pid_t pid=fork();
        if(pid==0){
            execl("/usr/bin/xdg-open","xdg-open",infoVector[cursorPos][0].c_str(),NULL);
            exit(1);
        }
    }
}

void upKey(){
    cursorPos--;
    if(cursorPos>=0){
        // valid up move

        if(cursorPos<startPos){
            // window need to slide one move up
            startPos--;
            endPos--;
        }
        displayWindow();
    }
    else{
        // you are standing at top of the content window
        cursorPos++;
        displayWindow();
    }
}

// void downKey(int cursorPos, int startPos, int endPos, vector<int> vecOfPaths){
void downKey(){
    cursorPos++;
    if(cursorPos<infoVector.size()){
        // valid down move

        if(cursorPos>endPos){
            // window need to slide one move up
            startPos++;
            endPos++;
        }
        displayWindow();
    }
    else{
        // you are standing at top of the content window
        cursorPos--;
        displayWindow();
    }
}

void backKey(){
    if(backStack.empty())return;
    else{
        string newPath=backStack.top();
        newPath= simplifyPath(newPath);

        backStack.pop();
        listDirectory(newPath.c_str());
        // in future check if error generated or not in listing directory and handle the case
        forStack.push(gbPath);
        gbPath=newPath;

        displayWindowResetPointers();
    }
}

void forKey(){
    if(forStack.empty())return;
    else{
        string newPath=forStack.top();
        newPath= simplifyPath(newPath);


        forStack.pop();
        listDirectory(newPath.c_str());
        // in future check if error generated or not in listing directory and handle the case
        backStack.push(gbPath);
        gbPath=newPath;

        displayWindowResetPointers();

    }
}

void backspace(){
    // if(backStack.empty()){
    //     string newPath= simplifyPath(gbPath);
    //     int idx=newPath.find_last_of('/');
    //     newPath= newPath.substr(0,idx);

    //     if(newPath=="")newPath="/";

    //     listDirectory(newPath.c_str());
    //     // in future check if error generated or not in listing directory and handle the case

    //     // backStack.push(gbPath);
    //     emptyForStack();// forStack.clear();
    //     gbPath=newPath;
    //     displayWindowResetPointers();
    // }
    // else{
    //     // string newPath=backStack[backStack.size()-1];
    //     string newPath=backStack.top();
    //     backStack.pop();
    //     listDirectory(newPath.c_str());
    //     // in future check if error generated or not in listing directory and handle the case
    //     emptyForStack();// forStack.clear();
    //     gbPath=newPath;

    //     displayWindowResetPointers();
    // }

    // NEW BACKSPACE FUNCTIONALITY
    string newPath= simplifyPath(gbPath);

    // find parent path of the simplified path
    int idx=newPath.find_last_of('/');
    newPath= newPath.substr(0,idx);
    if(newPath=="")newPath="/";

    if(newPath==gbPath){
        // get executed when gbPath is "/" ---> so no need to list directory new time;
        return;
    }

    listDirectory(newPath.c_str());
    // in future check if error generated or not in listing directory and handle the case

    // backStack.push(gbPath);
    emptyForStack();// forStack.clear();
    backStack.push(gbPath);
    gbPath=newPath;
    displayWindowResetPointers();

}