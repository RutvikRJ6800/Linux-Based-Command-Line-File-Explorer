/*
Integration of ls and scrolling functionality 

###DONE current modification i am doing : line 103 store correct path 
###DONE find home path and store it into home string.
        Sliding window type display of content

*/
#include "myHeaderFiles.h"
using namespace std;
struct winsize wins;
struct termios initialAttributes;
struct passwd *pw = getpwuid(getuid());
string home;


vector<vector<string>> infoVector;
stack<string> backStack,forStack;
int cursorPos, startPos, endPos,winRows;
string gbPath;

void setHomePath(){
    // find Home here
    // pw = getpwuid(getuid());
    home=pw->pw_dir;
    gbPath=home;
}

void setCanonicalMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &initialAttributes);
}

void setNonCanonicalMode(){
    struct termios rawAttributes;
    atexit(setCanonicalMode);
    tcgetattr(STDIN_FILENO,&initialAttributes);
    rawAttributes=initialAttributes;

    // rawAttributes.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    rawAttributes.c_lflag &= ~(ECHO | ICANON);
    rawAttributes.c_iflag &= ~(IXON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawAttributes);
    rawAttributes.c_cc[VMIN] = 0;
    rawAttributes.c_cc[VTIME] = 1;

    cout<<"Non Canonical Mode Set Successfully"<<endl;

}

size_t terminalRows(){
	ioctl(STDIN_FILENO, TIOCGWINSZ, &wins);
    return wins.ws_row;
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

void displayInfoVector(){
    cout<<"\33c";
    // for(int i=0; i<infoVector.size(); i++){
    for(int i=startPos; i<=endPos; i++){
        if(i==cursorPos){
            cout<<"--->\t";
        }
        else{
            cout<<"        ";
        }
        cout<<left<<setw(14)<<infoVector[i][1]; // permission
        cout<<right<<setw(8)<<infoVector[i][2]<<"    "; // file size
        cout<<left<<setw(12)<<infoVector[i][3]; // uname
        cout<<left<<setw(12)<<infoVector[i][4]; // gname
        cout<<left<<setw(29)<<infoVector[i][5]; // date
        cout<<left<<infoVector[i][6]<<endl; // file Name

        // for(int j=1; j<infoVector[i].size(); j++){
        //     cout<<infoVector[i][j]<<"\t";
        // }
        // cout<<endl;

    }
}
void resetPointers(){
    winRows= terminalRows();
    // endPos=winRows-6;
    endPos=infoVector.size()<winRows-6?infoVector.size()-1:winRows-6;
    cursorPos=startPos=0;
}

void displayWindow(){
    // int winRows= terminalRows();
    // endPos=winRows-6;
    // cursorPos=startPos=0;
    // int newWinRows= terminalRows();
    // if(newWinRows<winRows){
    //     // window shrink so set endpointer according ly
    //     endPos=newWinRows<infoVector.size()
    // }
    // endPos=winRows-6;
    winRows=terminalRows();
    endPos=(winRows-6+startPos)<infoVector.size()?(winRows-6+startPos):infoVector.size()-1;

    displayInfoVector();

    int emptyLine=winRows-(endPos-startPos+1)-3;
    while(emptyLine--){
        cout<<endl;
    }
    cout<<"PATH: "<<gbPath<<endl;
    cout<<"------NORMAL MODE------";
}

void displayWindowResetPointers(){
    // int winRows= terminalRows();
    // endPos=winRows-6; 
    // cursorPos=startPos=0;
    resetPointers();
    displayWindow();
    
}

string simplifyPath(string dirnameString){
    int dirLength=dirnameString.size();
    if(dirnameString[dirLength-1]=='.' && dirnameString[dirLength-2]=='.'){
        int idx=dirnameString.find_last_of('/');
        dirnameString=dirnameString.substr(0,idx);
        idx=dirnameString.find_last_of('/');
        dirnameString=dirnameString.substr(0,idx);
    }
    else if(dirnameString[dirLength-1]=='.'){
        int idx=dirnameString.find_last_of('/');
        dirnameString=dirnameString.substr(0,idx);
    }

    // if simplified dirnameString is  empty that means actually it would be "/"
    if(dirnameString=="")dirnameString="/";

    return dirnameString;

}

void createInfoVector(string path){
    struct stat inode;
    double fileSize;
    char mtime[50];
    string perm,uname,gname;
    time_t s_time;
    vector<string> name= split(path,'/');
    vector<string> details;

    if(stat(path.c_str(),&inode)!=0){
        cout<<"Can't open the file with path "<<path<<" error code: "<<strerror(stat(path.c_str(),&inode))<<endl;
        return;
    }

     /* print vec of name 
    for(int i=0; i<name.size(); i++){
        cout<<name[i]<<" ";
    }
    cout<<endl;
    */
    
    perm="";
    
    perm+=(S_ISDIR(inode.st_mode))?'d':'-';
    perm+=(inode.st_mode&S_IRUSR)?'r':'-';
    perm+=(inode.st_mode&S_IWUSR)?'w':'-';
    perm+=(inode.st_mode&S_IXUSR)?'x':'-';
    perm+=(inode.st_mode&S_IRGRP)?'r':'-';
    perm+=(inode.st_mode&S_IWGRP)?'w':'-';
    perm+=(inode.st_mode&S_IXGRP)?'x':'-';
    perm+=(inode.st_mode&S_IROTH)?'r':'-';
    perm+=(inode.st_mode&S_IWOTH)?'w':'-';
    perm+=(inode.st_mode&S_IXOTH)?'x':'-';

    // append path first so we can use it later while we want to open the file.
    details.push_back(path);
    details.push_back(perm);

    fileSize=inode.st_size;
    if(fileSize>=1024){
        fileSize=fileSize/1024;
        if(fileSize>=1024)
        {
            fileSize=fileSize/1024;
            if(fileSize>=1024){
                fileSize=fileSize/1024;
                string fs = to_string(fileSize);
                fs=fs.substr(0, fs.find(".")+3);
                fs+='G';
                details.push_back(fs);


            }
            // printf("%8.2fM\t",fileSize);
            string fs = to_string(fileSize);
            fs=fs.substr(0, fs.find(".")+3);
            fs+='M';
            details.push_back(fs);
        }
        else{
            // printf("%8.2fK\t",fileSize);
            string fs = to_string(fileSize);
            fs=fs.substr(0, fs.find(".")+3);
            fs+='K';
            details.push_back(fs);

        }     
    }
    else{
        // printf("%8.2f \t",fileSize);
        string fs = to_string(fileSize);
        fs=fs.substr(0, fs.find("."));
        details.push_back(fs);
    }

    uname=getpwuid(inode.st_uid)->pw_name;
    gname=getgrgid(inode.st_gid)->gr_name;
    // cout<<uname<<"\t";
    // cout<<gname<<"\t";
    details.push_back(uname);
    details.push_back(gname);

    time_t t = inode.st_mtime; /*st_mtime is type time_t */
    struct tm lt;
    localtime_r(&t, &lt); /* convert to struct tm */
    strftime(mtime, sizeof mtime, "%a, %d %b %Y %T", &lt);
    // printf("%s\t", mtime);
    details.push_back(mtime);
    // cout<<name[name.size()-1]<<endl;
    details.push_back(name[name.size()-1]);

    // push details in globally declare infoVector
    infoVector.push_back(details);

}

vector<string> listDirectory(const char *dirname){
    DIR *dir;
    struct dirent *d;
    vector<string> vecOfPath;
    //open the directory
    string dirnameString=dirname;
    int dirLength=dirnameString.size();
    if(dirnameString[dirLength-1]=='.' && dirnameString[dirLength-2]=='.'){
        int idx=dirnameString.find_last_of('/');
        dirnameString=dirnameString.substr(0,idx);
        idx=dirnameString.find_last_of('/');
        dirnameString=dirnameString.substr(0,idx);
    }
    else if(dirnameString[dirLength-1]=='.'){
        int idx=dirnameString.find_last_of('/');
        dirnameString=dirnameString.substr(0,idx);
    }

    // if simplified dirnameString is  empty that means actually it would be "/"
    if(dirnameString=="")dirnameString="/";

    cout<<"dirname"<<dirnameString<<endl;
    // dir=opendir(dirname);
    dir=opendir(dirnameString.c_str());
    if(!dir){
        cout<<"Error in opening directory"<<endl;
        exit(1);
    }


 
    
    while((d=readdir(dir))!=NULL){
        string s=d->d_name;
        
        if(dirnameString[dirnameString.size()-1]=='/'){
            s=dirnameString+s;
        }
        else
            s=dirnameString+'/'+s;

        vecOfPath.push_back(s);
        // cout<<d->d_name<<endl;
    }
    closedir(dir);

    sort(vecOfPath.begin(), vecOfPath.end(),cmp2);

    // clear screen before printing new things
    infoVector.clear();
    /*print vec of path
    for(int i=0; i<vecOfPath.size(); i++){
        cout<<vecOfPath[i]<<endl;
    }
    */

    for(int i=0; i<vecOfPath.size(); i++){
        createInfoVector(vecOfPath[i]);
    }

    // sort recently created infoVector
    // sort(infoVector.begin(), infoVector.end(), cmp);

    return vecOfPath;
}
// ************************************************
// function to open directory while enter key press
// ************************************************
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

int main(){

    setNonCanonicalMode();

    setHomePath();

    listDirectory(gbPath.c_str());
    displayWindowResetPointers();

    // display(0, 0, endPos,vecOfPaths);
    // cout<<"Terminal Rows: "<<terminalRows()<<endl;
    char ch;
    // ch=cin.get();
    while(1){
        ch = getchar();
        if(ch=='q')break;
        if(ch==27){
            ch=cin.get();
            if (ch == '['){
                ch=getchar();
                switch (ch)
                {
                case 'A':
                    upKey();
                    // printf("\033[A");
                    break;
                case 'B':
                    downKey();
                    // printf("\033[B");
                    break;
                case 'C':
                    forKey();
                    break;
                case 'D':
                    backKey();
                    break;
                }
            }
        }
        else if(ch==0x0A){
            enterKey();
        }
        else if(ch==127){
            backspace();
        }
        else if(ch=='h'){
            if(gbPath!=home)
                backStack.push(gbPath);
            gbPath=home;
            listDirectory(gbPath.c_str());
            displayWindowResetPointers();
        }
        else{
            cout<<ch<<endl;
        }

    }
    return 0;
}
