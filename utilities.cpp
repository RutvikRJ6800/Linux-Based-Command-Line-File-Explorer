#include "myHeaderFiles.h"

struct winsize wins;

size_t terminalRows(){
	ioctl(STDIN_FILENO, TIOCGWINSZ, &wins);
    return wins.ws_row;
}

size_t terminalCols(){
	ioctl(STDIN_FILENO, TIOCGWINSZ, &wins);
    return wins.ws_col;
}

vector<string> split(string str, char del){
    vector<string> res;
    string temp="";

    for(int i=0; i<(int)str.size(); i++){
        if(str[i]!=del){
            temp+=str[i];
        }
        else{
            res.push_back(temp);
            temp="";
        }
    }
    res.push_back(temp);
    return res;
}

void emptyBackStack(){
    while(!backStack.empty()){
        backStack.pop();
    }
}

void emptyForStack(){
    while(!forStack.empty()){
        forStack.pop();
    }
}

void convertLower(string &test){
    for(auto& c : test){
        c = tolower(c);
    }
}

/* I Used it to sort InfoVector before...:) 
bool cmp(vector<string>& lhs, vector<string>& rhs){
    string st1=lhs[lhs.size()-1];
    string st2=rhs[rhs.size()-1];
    convertLower(st1);
    convertLower(st2);

    // transform(st1.begin(), st1.end(), st1.begin(), ::tolower);
    // transform(st2.begin(), st2.end(), st2.begin(), ::tolower);

    return st1<st2;
} */

bool cmp2(string &a, string &b){
    string st1=a;
    string st2=b;

    convertLower(st1);
    convertLower(st2);

    return st1<st2;

}
