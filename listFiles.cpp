#include "myHeaderFiles.h"
using namespace std;
vector<vector<string>> infoVector;
int cursorPos, startPos, endPos;
    struct winsize wins;

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

void convertLower(string &test){
    for(auto& c : test){
        c = tolower(c);
    }
}

bool cmp(vector<string>& lhs, vector<string>& rhs){
    string st1=lhs[lhs.size()-1];
    string st2=rhs[rhs.size()-1];
    convertLower(st1);
    convertLower(st2);

    // transform(st1.begin(), st1.end(), st1.begin(), ::tolower);
    // transform(st2.begin(), st2.end(), st2.begin(), ::tolower);

    return st1<st2;
}

void displayInfoVector(){
    for(int i=0; i<infoVector.size(); i++){
        cout<<"\t";
        for(int j=0; j<infoVector[i].size(); j++){
            cout<<infoVector[i][j]<<"\t";
        }
        cout<<endl;
    }
}

void createInfoVector(string path){
    struct stat inode;
    double fileSize;
    char mtime[50];
    string perm,uname,gname;
    time_t s_time;
    vector<string> name= split(path,'/');

     /*print vec of name
    for(int i=0; i<name.size(); i++){
        cout<<name[i]<<" ";
    }
    cout<<endl;
     */
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
    perm+=(inode.st_mode&S_IWGRP)?'r':'-';
    perm+=(inode.st_mode&S_IXGRP)?'r':'-';

    details.push_back(perm);
    // cout<<perm;
    fileSize=inode.st_size;
    if(fileSize>100){
        fileSize=fileSize/1024;
        if(fileSize>100)
        {
            fileSize=fileSize/1024;
            // printf("%8.2fM\t",fileSize);
            string fs = to_string(fileSize);
            fs+='M';
            details.push_back(fs);
        }
        else{
            // printf("%8.2fK\t",fileSize);
            string fs = to_string(fileSize);
            fs+='K';
            details.push_back(fs);

        }     
    }
    else{
        // printf("%8.2f \t",fileSize);
        string fs = to_string(fileSize);
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
    dir=opendir(dirname);
    if(!dir){
        cout<<"Error in opening directory"<<endl;
        exit(1);
    }
 

    while((d=readdir(dir))!=NULL){
        string s=d->d_name,s1=dirname;
        s=s1+'/'+s;
        vecOfPath.push_back(s);
        // cout<<d->d_name<<endl;
    }
    closedir(dir);

    // sort(vecOfPath.begin(), vecOfPath.end());

    // clear screen before printing new things
    /*print vec of path
    for(int i=0; i<vecOfPath.size(); i++){
        cout<<vecOfPath[i]<<endl;
    }
    */

    for(int i=0; i<vecOfPath.size(); i++){
        createInfoVector(vecOfPath[i]);
    }

    // sort recently created infoVector
    sort(infoVector.begin(), infoVector.end(), cmp);

    return vecOfPath;
}

int main(){
    // using getcwd retrived current directory
    string path="/home/rutvik/";
    cout<<terminalRows()<<endl;
    cout<<path.c_str()<<endl;
    listDirectory(path.c_str());

    displayInfoVector();

    
    return 0;
}
