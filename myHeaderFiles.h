/*##############################################
INCLUDE NEEDED HEADER FILES 
 ##############################################*/
#include<iostream>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include<bits/stdc++.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
using namespace std;


/*##############################################
EXTENDING SCOPE OF GLOBAL VARIABLE
 ##############################################*/
struct winsize wins;
termios initialAttributes;
struct passwd *pw;
string home;


vector<vector<string>> infoVector;
stack<string> backStack,forStack;
size_t cursorPos, startPos, endPos,winRows, winCols;
string gbPath;
bool commandMode=false;
bool searchFound;


/*##############################################
FUNCTIONS DEFINATIONS
 ##############################################*/
void setHomePath();
void setCanonicalMode();
void setNonCanonicalMode();


// listDirectory.h
void displayInfoVector();
void resetPointers();
void displayWindow();
void displayWindowResetPointers();
void sigWinChHandler(int);
string simplifyPath(string dirnameString);
void createInfoVector(string path);
vector<string> listDirectory(const char *dirname);


// keyPressActions.h
void openDirectory();
void enterKey();
void upKey();
void downKey();
void backKey();
void forKey();
void backspace();


// utilities.h
size_t terminalRows();
size_t terminalCols();
vector<string> split(string str, char del);
void emptyBackStack();
void emptyForStack();
void convertLower(string &test);
bool cmp2(string &a, string &b);


// commandMode.h
string getAbsolutePath(string);
vector<string> getCommand(string);
mode_t getPermission(string path);
void search(string path, string target);
int renameFile(string file1, string file2);
int createFile(string fileName, string path);
int createDir(string dirname, string path, mode_t perm);
int copyFile(string name, string destination, string source);
int copyDirect(string destination, string source);
bool destinationIsChildOfSource(string destination, string source);
int copyG(string name, string destination, string source);
int copyUsingPath(string destination, string source);
int deleteFile(string path);
int deleteDir(string path);
int deleteG(string path);
int move(string name, string path, string source);
int moveUsingPath(string destination, string source);
void takeInput();