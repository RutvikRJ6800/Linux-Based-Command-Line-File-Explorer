#include "myHeaderFiles.h"


bool searchFound=false;

string getAbsolutePath(string path){
    char resolved_path[500]; 
    if(path[0]=='.'){
        // handle relative path
        path=gbPath+'/'+path;
        if(realpath(path.c_str(), resolved_path)==NULL){
            cout<<"error"<<strerror(errno);
            return NULL;
        }
        else{
            // printf("\n%s\n",resolved_path);
            string ans(resolved_path);
            cout<<"path: "<<ans;
            // search on this path
            return ans;
        } 
        

    }
    else if(path[0]=='~'){
        path=home+'/'+path;
        if(realpath(path.c_str(), resolved_path)==NULL){
            cout<<"error"<<strerror(errno);
            return NULL;
        }
        else{
            // printf("\n%s\n",resolved_path);
            string ans(resolved_path);
            cout<<"path: "<<ans;
            // search on this path
            return ans;
        } 
    }
    else{
        // handle absolute path
        if(realpath(path.c_str(), resolved_path)==NULL){
            cout<<"error"<<strerror(errno);
        }
        string ans(resolved_path);
        cout<<"path: "<<ans;
        return ans;

        
    }
}

vector<string> getCommand(string inp){
vector<string> ans;
    istringstream ss(inp);
    string temp="";
    while(std::getline(ss, temp, ' ')) {
        // std::cout << token << '\n';
        if(temp==" ")continue;
        ans.push_back(temp);
    }

    return ans;
}

void search(string path, string target){
    // ans string contains valid absolute path now;
    // search recursively on it.

    // cout<<"path: "<<path<<'\t'<<"target: "<<target<<endl;
    DIR* dir = opendir(path.c_str());
    if(dir==NULL){
        cout<<strerror(errno);
    }
    else{
        // cout<<"searching in directory: "<<path<<endl;

        struct dirent* entity;
        while(((entity=readdir(dir)) != NULL) && !searchFound){
            string s=entity->d_name;
            // cout<<"s= "<<s<<"\t"<<"target= "<<target<<endl;   

            if(s==target){
                cout<<"target found"<<"path: "<<path<<endl; 
                searchFound=true;  
            }
            if(entity->d_type==DT_DIR && strcmp(entity->d_name,".")!=0 && strcmp(entity->d_name,"..")!=0){
                if(path[path.size()-1]=='/'){
                    s=path+s;
                }
                else{
                    s=path+'/'+s;
                }
                search(s,target);
            }
        }
    }
        closedir(dir);
        // cout<<"come to end of while"<<endl;
}



int renameFile(string file1, string file2){
    // check if file exist in corrent folder or not
    file1=gbPath+'/'+file1;
    file2=gbPath+'/'+file2;
    if(rename(file1.c_str(), file2.c_str())!=0){
        cout<<strerror(errno)<<endl;
        sleep(3);
        displayWindowResetPointers();
        return -1;
    }
    cout<<"\nFile Renamed Successfully"<<endl;
    sleep(3);
    listDirectory(gbPath.c_str());
    displayWindowResetPointers();
    return 0;

}

int createFile(string fileName, string path){
    path=getAbsolutePath(path);
    fileName=path+'/'+fileName;

    FILE* filePointer;
    filePointer=fopen(fileName.c_str(), "w+");
    if(filePointer==NULL){
        return -1   ;
    }
    else{
        fclose(filePointer);
        return 0;
    }
    
}

int createDir(string dirname, string path){
    path=getAbsolutePath(path);
    dirname=path+'/'+dirname;
    if(mkdir(dirname.c_str(),S_IRWXU)==-1){
        return -1;
    }
    else{
        return 0;
    }


}

// int copy(string name, string destination){
//     DIR* dir = opendir(gbPath.c_str());
//     if(dir==NULL){
//         cout<<strerror(errno)<<endl;
//     }
//     else{
//         struct dirent* entity;
//         bool flag=false;
//         while(((entity=readdir(dir)) != NULL) && !flag){
//             string s=entity->d_name;
//             if(s==name){
//                 flag=true;
//                 break;
//             }
//         }
//         if(flag && (entity->d_type==DT_REG)){
//             // file with name name found
//             destination=getAbsolutePath(destination);
//             string newName=destination+'/'+name;
//             string oldName;
//             if(gbPath[gbPath.size()-1]=='/'){
//                 oldName=gbPath+name;
//             }
//             else{
//                 oldName=gbPath+'/'+name;
//             }
//             ifstream origFile{oldName};
//             ofstream newFile{newName};
//             string line;

//             if(origFile && newFile){
//                 while(getline(origFile,line)){
//                     newFile<<line<<"\n";
//                 }
//                 printf("\nFile copied successfully."); 
//                 return 0;               
//             }
//             else{
//                 printf("\nError occured in copy");
//                 return -1;
//             }
//         }

//     }
//     return -1;

// }

int copyFile(string name, string destination, string source ){
    destination=getAbsolutePath(destination);
    string newName=destination+'/'+name;
    string oldName;
    if(source[source.size()-1]=='/'){
        oldName=source+name;
    }
    else{
        oldName=source+'/'+name;
    }

    ifstream origFile{oldName};
    ofstream newFile{newName};
    string line;

    if(origFile && newFile){
        while(getline(origFile,line)){
            newFile<<line<<"\n";
        }
        // printf("\nFile copied successfully."); 
        return 0;               
    }
    else{
        // printf("\nError occured in copy");
        return -1;
    }
}


int copyDirect(string destination, string source){
    // copy contets present at source;
    DIR* dir=opendir(source.c_str());
    if(dir==NULL){
        cout<<strerror(errno)<<endl;
    }
    else{
        struct dirent* entity;
        while((entity=readdir(dir))!=NULL){
            string ename=entity->d_name;
            if(entity->d_type==DT_DIR && (ename!=".") && (ename!="..")){
                // valid Directory to go
                if(createDir(entity->d_name,destination)==-1){
                    cout<<"ERRRO IN CREATE DIR"<<endl;
                    return -1;
                }
                string ss=source+'/'+ename;
                string sd=destination+'/'+ename;
                copyDirect(sd,ss); 
            }
            else if(entity->d_type==DT_REG){
                copyFile(ename,destination,source);
            }
        }
        return 0;

    }
    return -1;

}
int copyG(string name, string destination, string source){
    cout<<"copy of: "<<name<<" from: "<<source<<" to: "<<destination<<endl;
    DIR* dir = opendir(source.c_str());
    if(dir==NULL){
        cout<<strerror(errno)<<endl;
    }
    else{
        struct dirent* entity;
        bool flag=false;
        string ename;
        while(((entity=readdir(dir)) != NULL) && !flag){
            ename=entity->d_name;
            if(ename==name){
                flag=true;
                break;
            }
        }
        if(flag && (entity->d_type==DT_REG)){
            // file with name name found
            if(copyFile(ename, destination, source)==-1){
                return -1;
            }
            else return 0;
            // if(createFile(entity->d_name,destination)!=-1){
            //     cout<<"coud not create file at destination: "<<destination<<endl;
            // }
            // else{

            // }
        }
        else if(flag && (entity->d_type==DT_DIR)){
            if(createDir(ename,destination)==-1){
                cout<<"ERRRO IN CREATE DIR"<<endl;
                return -1;
            }
            string ss=source+'/'+ename;
            string sd=destination+'/'+ename;
            return copyDirect(sd,ss); 
            // recursively see and create things at destination
        
            // destination=getAbsolutePath(destination);
            // if(createDir(entity->d_name,destination)==-1){
            //     cout<<"\n"<<"Could'nt create Directory"<<endl;
            //     return -1;
            // }
            // string newName=destination+'/'+(entity->d_name);
            // string oldName;
            // if(source[source.size()-1]=='/'){
            //     oldName=source+(entity->d_name);
            // }
            // else{
            //     oldName=source+'/'+(entity->d_name);
            // }
            // dir = opendir(oldName.c_str());
            // if(dir==NULL){
            //     cout<<strerror(errno);
            // }
            // else{
            //     struct dirent* entity;
            //     while(((entity=readdir(dir)) != NULL)){
            //         string s=entity->d_name;
            //                 string newName=destination+'/'+(entity->d_name);
            //         string oldName;
            //         if(source[source.size()-1]=='/'){
            //             oldName=source+(entity->d_name);
            //         }
            //         else{
            //             oldName=source+'/'+(entity->d_name);
            //         }
            //         // cout<<"s= "<<s<<"\t"<<"target= "<<target<<endl;   
            //         if(entity->d_type==DT_REG){
            //             if(copyFile(entity->d_name, newName, source)==-1){
            //                 return -1;
            //             }
            //             else return 0;
            //         }
            //         else if(entity->d_type==DT_DIR && strcmp(entity->d_name,".")!=0 && strcmp(entity->d_name,"..")!=0){
            //             copyG(entity->d_name,newName,oldName);
            //         }
            //     }
            // }

        }

        // cout<<"Path i was looking: "<<gbPath<<" & i was copying "<<destination;
        // if(flag==true){
        //     cout<<"file found but error in copy"<<endl;
            
        // }
        // else{
        //     cout<<"file does'nt found"<<endl;
        // }
        // return -1;
    }
    return -1;

}





void takeInput(){
    char ch;
    do{
        string inp;
        while((ch=cin.get()) !='\n' && ch!=27){
            if(ch==127 && inp.size()>0){
                inp.pop_back();
                cout<<"\b \b";
            }
            else{
                cout<<ch;
                inp+=ch;
            }
        }
        // cout<<endl;
        if(ch=='\n' && inp.size()>0){
            vector<string> command=getCommand(inp);
            // cout<<"You written:";
            // for(size_t i=0; i<command.size(); i++){
            //     cout<<command[i]<<" ";
            // }
            
            // FIND WHICH COMMAND IS WRITTEN
            if(command[0]=="goto"){
                cout<<"goto executed"<<endl;
                string path=command[1];
                char resolved_path[500]; 
                if(path[0]=='.'){
                    // Handle relative Path
                    path=gbPath+'/'+path;
                    if(realpath(path.c_str(), resolved_path)==NULL){
                        cout<<"error"<<strerror(errno);
                    }
                    else{
                        printf("\n%s\n",resolved_path);
                        string ans(resolved_path);
                        cout<<"path: "<<ans;
                        listDirectory(ans.c_str());
                        gbPath=ans;
                        displayWindowResetPointers();

                    } 

                }
                else if(path[0]=='~'){
                    string withTildRemove=path.substr(1,path.size()-1);
                    path=home+'/'+withTildRemove;
                    if(realpath(path.c_str(), resolved_path)==NULL){
                        cout<<"error"<<strerror(errno);
                    }
                    else{
                        // printf("\n%s\n",resolved_path);
                        string ans(resolved_path);
                        cout<<"path: "<<ans;
                        listDirectory(ans.c_str());
                        gbPath=ans;
                        displayWindowResetPointers();
                        // search on this path
                    } 
                }
                else{
                    // Handle absolute path
                    if(realpath(path.c_str(), resolved_path)==NULL){
                        cout<<"error"<<strerror(errno);
                    }
                    string ans(resolved_path);
                    listDirectory(ans.c_str());
                    gbPath=ans;
                    displayWindowResetPointers();

                }
            }
            else if(command[0]=="search"){
                searchFound=false;
                search(gbPath,command[1]);
                if(searchFound==true)
                    cout<<"True";
                else cout<<"False";

            }
            else if(command[0]=="quit"){
                exit(1);
            }
            else if(command[0]=="rename"){
                renameFile(command[1],command[2]);
            }
            else if(command[0]=="create_file"){
                int res=createFile(command[1],command[2]);
                if(res==-1){
                    printf("\n%s",strerror(errno));
                    sleep(3);
                    displayWindowResetPointers();
                }
                if(res==0){
                    cout<<"\nFile Created Successfully"<<endl;
                    sleep(3);
                    listDirectory(gbPath.c_str());
                    displayWindowResetPointers();
                }
            }
            else if(command[0]=="create_dir"){
                int res=createDir(command[1],command[2]);
                if(res==-1){
                    printf("\n%s",strerror(errno));
                    sleep(3);
                }
                else if(res==0){
                    cout<<"\nDirectory Created Successfully"<<endl;
                    sleep(2);
                    listDirectory(gbPath.c_str());
                }
                displayWindowResetPointers();
            }
            else if(command[0]=="copy"){
                // cout<<endl;
                // for(int i=0; i<command.size(); i++){
                //     cout<<command[i]<<"$$";
                // }
                size_t sz=command.size();
                int res=0;
                for(int i=1; i<=sz-2; i++){
                    res+=copyG(command[i],command[sz-1],gbPath);
                }
                if(res==0){
                    cout<<"\n"<<"copy done successfuly"<<endl;
                }
                else if(res<0){
                    cout<<"\n"<<strerror(errno)<<endl;
                }
                sleep(2);
                displayWindowResetPointers();
            }

        }
    }
    while(ch!=27);

    // esc pressed so come out of command mode
    cout<<"exit from commamd mode";
    commandMode=false;
    return;

}