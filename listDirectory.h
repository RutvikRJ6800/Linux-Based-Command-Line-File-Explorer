/*##############################################
GLOBAL VARIABLES
 ##############################################*/
// vector<vector<string>> infoVector;



/*##############################################
display info Vectors
 ##############################################*/
void displayInfoVector(){
    cout<<"\33c";
    for(size_t i=startPos; i<=endPos; i++){
        if(i==cursorPos){
            cout<<"\033[1;7m"<<"--->    ";
        }
        else{
            cout<<"        ";
        }
        if(winCols>70)cout<<left<<setw(10)<<infoVector[i][1]<<"    "; // permission
        if(winCols>60)cout<<right<<setw(8)<<infoVector[i][2]<<"    "; // file size
        if(winCols>130)cout<<left<<setw(12)<<infoVector[i][3]; // uname
        if(winCols>140)cout<<left<<setw(12)<<infoVector[i][4]; // gname
        if(winCols>100)cout<<left<<setw(29)<<infoVector[i][5]; // date
        if(infoVector[i][1][0]=='d')cout<<left<<"\033[1;34m"<<infoVector[i][6]<<"\033[0m"; // file Name
        else cout<<left<<infoVector[i][6];
        if(i==cursorPos){
            cout<<"\033[0m";
        }
        cout<<"\n";
    }
}
/*##############################################
set startPos, ensPos, & cursorPos to 0. 
 ##############################################*/
void resetPointers(){
    winRows= terminalRows();
    // endPos=winRows-6;
    endPos=infoVector.size()<winRows-7?infoVector.size()-1:winRows-7;
    cursorPos=startPos=0;
}
/*##############################################
display window without reset pos
 ##############################################*/
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
    // winRows=terminalRows();


    // size_t newWinRows=terminalRows();
    // if(newWinRows>=winRows){
    //     winRows=newWinRows;
        
    // }else{
    //     startPos=cursorPos;
    //     winRows=newWinRows;
    // }
    //     endPos=(winRows-6+startPos)<infoVector.size()?(winRows-6+startPos):infoVector.size()-1;

    displayInfoVector();

    int emptyLine=winRows-(endPos-startPos+1)-5;
    while(emptyLine--){
        cout<<endl;
    }
    cout<<"PATH: "<<gbPath<<endl;
    if(!commandMode)
    cout<<"\033[1;32m"<<"--------NORMAL MODE--------"<<"\033[0m";
    else
    cout<<"\033[1;32m"<<"--------COMMAND MODE-------"<<"\033[0m"<<"\n> ";
}

/*##############################################
display window with reset pos
 ##############################################*/
void displayWindowResetPointers(){
    // int winRows= terminalRows();
    // endPos=winRows-6; 
    // cursorPos=startPos=0;
    resetPointers();
    displayWindow();
    
}

/*##############################################
window size change handler - calls by signal
 ##############################################*/
void sigWinChHandler(int sig){
    winCols=terminalCols();
    // winRows=terminalRows();

    size_t newWinRows=terminalRows();
    if(newWinRows>=winRows){
        winRows=newWinRows;
        
    }else{
        startPos=cursorPos;
        winRows=newWinRows;
    }
    endPos=(winRows-7+startPos)<infoVector.size()?(winRows-7+startPos):infoVector.size()-1;
    displayWindow();

}
/*##############################################
simplify path if it has . or .. in last
 ##############################################*/
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

/*##############################################
create info vector of entity presets at path
 ##############################################*/
void createInfoVector(string path){
    struct stat inode;
    double fileSize;
    char mtime[50];
    string perm,uname,gname;
    vector<string> name= split(path,'/');
    vector<string> details;

    if(stat(path.c_str(),&inode)!=0){
        cout<<"Can't open the file with path "<<path<<" error code: "<<strerror(stat(path.c_str(),&inode))<<endl;
        return;
    }
    
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

/*##############################################
list directory from path(create info vector and sort it)
 ##############################################*/
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

    // cout<<"dirname"<<dirnameString<<endl;
    // dir=opendir(dirname);
    dir=opendir(dirnameString.c_str());
    if(!dir){
        cout<<"\nError in opening directory"<<endl;
        // exit(1);
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

    for(size_t i=0; i<vecOfPath.size(); i++){
        createInfoVector(vecOfPath[i]);
    }

    // sort recently created infoVector
    // sort(infoVector.begin(), infoVector.end(), cmp);

    return vecOfPath;
}