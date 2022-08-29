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
#include <sys/types.h>
#include <dirent.h>
#include<bits/stdc++.h>
#include <sys/stat.h>
using namespace std;

extern struct winsize wins;
extern struct termios initialAttributes;
extern struct passwd *pw;
extern string home;


extern vector<vector<string>> infoVector;
extern stack<string> backStack,forStack;
extern size_t cursorPos, startPos, endPos,winRows;
extern string gbPath;

void setHomePath();
void setCanonicalMode();
void setNonCanonicalMode();
size_t terminalRows();
vector<string> split(string str, char del);
void emptyBackStack();
void emptyForStack();
void convertLower(string &test);
bool cmp2(string &a, string &b);
void displayInfoVector();
void resetPointers();
void displayWindow();
void displayWindowResetPointers();
string simplifyPath(string dirnameString);
void createInfoVector(string path);
vector<string> listDirectory(const char *dirname);
void openDirectory();
void enterKey();
void upKey();
void downKey();
void backKey();
void forKey();
void backspace();