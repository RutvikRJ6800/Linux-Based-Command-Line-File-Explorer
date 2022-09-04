/*##############################################
GLOBAL VARIABLES
 ##############################################*/
// bool searchFound = false;



/*##############################################
return absolute path
 ##############################################*/
string getAbsolutePath(string path)
{
    char resolved_path[500];
    if (path[0] == '.')
    {
        // handle relative path
        path = gbPath + '/' + path;
        if (realpath(path.c_str(), resolved_path) == NULL)
        {
            cout << "\nError Message: " << strerror(errno);
            return "";
        }
        else
        {
            // printf("\n%s\n",resolved_path);
            string ans(resolved_path);
            // cout<<"path: "<<ans;
            // search on this path
            return ans;
        }
    }
    else if (path[0] == '~')
    {
        string withTildRemove = path.substr(1, path.size() - 1);
        path = home + '/' + withTildRemove;
        if (realpath(path.c_str(), resolved_path) == NULL)
        {
            cout << "\nError Message: " << strerror(errno);
            return "";
        }
        else
        {
            // printf("\n%s\n",resolved_path);
            string ans(resolved_path);
            // cout<<"path: "<<ans;
            // search on this path
            return ans;
        }
    }
    else if(path[0]=='/'){
        // it Must be absolute path
            if (realpath(path.c_str(), resolved_path) == NULL)
        {
            cout << "\nError Message: " << strerror(errno);
            return "";
        }
        else{
        string ans(resolved_path);
        // listDirectory(ans.c_str());
        return ans;
        }
    }
    else
    {
        // Handle Relative path
        path=gbPath+'/'+path;
        if (realpath(path.c_str(), resolved_path) == NULL)
        {
            cout << "\nError Message: " << strerror(errno);
            return "";
        }
        else{
            string ans(resolved_path);
            return ans;
        }
    }
        return "";
}
/*##############################################
split command taken as inp with space separate
 ##############################################*/
vector<string> getCommand(string inp)
{
    vector<string> ans;
    string temp = "";
    bool flag=false;
    for(size_t i=0; i<inp.size(); i++){
        if(inp[i]=='"' && !flag){
            flag=true;
        }
        else if(inp[i]=='"' && flag){
            ans.push_back(temp);
            i++;
            temp="";
            flag=false;
        }
        else if(inp[i]==' ' && flag){
            temp+=inp[i];
        }
        else if(inp[i]==' ' && !flag){
            ans.push_back(temp);
            temp="";
        }
        else{
            temp+=inp[i];
        }
    }
    if(temp.size()>0){
        ans.push_back(temp);
    }


    return ans;
}
/*##############################################
return permission of file whose path is passed
 ##############################################*/
mode_t getPermission(string path)
{
    struct stat st;

    if (stat(path.c_str(), &st) == 0)
    {
        mode_t perm = st.st_mode;
        perm = perm | ((perm & S_IRUSR) ? 00400 : 0);
        perm = perm | ((perm & S_IWUSR) ? 00200 : 0);
        perm = perm | ((perm & S_IXUSR) ? 00100 : 0);
        perm = perm | ((perm & S_IRGRP) ? 00040 : 0);
        perm = perm | ((perm & S_IWGRP) ? 00020 : 0);
        perm = perm | ((perm & S_IXGRP) ? 00010 : 0);
        perm = perm | ((perm & S_IROTH) ? 00004 : 0);
        perm = perm | ((perm & S_IWOTH) ? 00002 : 0);
        perm = perm | ((perm & S_IXOTH) ? 00001 : 0);
        return perm;
    }
    else
    {
        return errno;
    }
}
/*##############################################
search at path with name as target
 ##############################################*/
void search(string path, string target)
{
    DIR *dir = opendir(path.c_str());
    if (dir == NULL)
    {
        cout << strerror(errno);
    }
    else
    {
        struct dirent *entity;
        while (((entity = readdir(dir)) != NULL) && !searchFound)
        {
            string s = entity->d_name;

            if (s == target)
            {
                searchFound = true;
            }
            if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
            {
                if (path[path.size() - 1] == '/')
                {
                    s = path + s;
                }
                else
                {
                    s = path + '/' + s;
                }
                search(s, target);
            }
        }
    }
    closedir(dir);
}
/*##########################################################
rename file from file1 to file2 (file is at currentDirectory)
 ##########################################################*/
int renameFile(string file1, string file2)
{
    // check if file exist in corrent folder or not
    file1 = gbPath + '/' + file1;
    file2 = gbPath + '/' + file2;
    if (rename(file1.c_str(), file2.c_str()) != 0)
    {
        return -1;
    }
    // cout<<"\nFile Renamed Successfully"<<endl;

    return 0;
}
/*##########################################################
create file with name as fileName at path as destination path
 ###########################################################*/
int createFile(string fileName, string path)
{
    path = getAbsolutePath(path);
    fileName = path + '/' + fileName;

    int newFile = open(fileName.c_str(), O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (newFile == -1)
    {
        // close(newFile);
        return -1;
    }
    close(newFile);
    return 0;
}

/*#############################################################
create directory with name dirname at path with permission perm
 #############################################################*/
int createDir(string dirname, string path, mode_t perm)
{
    path = getAbsolutePath(path);
    dirname = path + '/' + dirname;
    if (mkdir(dirname.c_str(), perm) == -1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
/*#######################################################################
copy file with name at at destination from source(does't include file name)
 ######################################################################*/
int copyFile(string name, string destination, string source)
{
    destination = getAbsolutePath(destination);
    string newName = destination + '/' + name;
    string oldName;
    if (source[source.size() - 1] == '/')
    {
        oldName = source + name;
    }
    else
    {
        oldName = source + '/' + name;
    }

    // cout<<"Old File: "<<oldName<<endl<<"New File: "<<newName<<endl;
    // usleep(10000000);
    // open oldfile in read mode
    int newFile, oldFile;
    oldFile = open(oldName.c_str(), O_RDONLY);
    if (oldFile == -1)
    {
        // close(oldFile);
        return -1;
    }

    mode_t perm = getPermission(oldName);
    // cout<<"PERMISSIONS: "<<perm<<endl;
    newFile = open(newName.c_str(), O_RDONLY);
    if (newFile != -1)
    {   
        deleteFile(newName);
        // cout << "\nError Message: file already exist.";
        // return -1;
    }

    newFile = open(newName.c_str(), O_CREAT | O_WRONLY, perm);

    // cout<<"copying started..."<<endl;
    char ch;
    while (read(oldFile, &ch, 1))
    {
        write(newFile, &ch, 1);
    }
    close(newFile);
    close(oldFile);
    return 0;
}
/*#######################################################
copy directory (path as source) recursively at destination 
 #######################################################*/
int copyDirect(string destination, string source)
{
    // copy contets present at source;
    DIR *dir = opendir(source.c_str());
    if (dir == NULL)
    {
        cout << strerror(errno) << endl;
    }
    else
    {
        struct dirent *entity;
        while ((entity = readdir(dir)) != NULL)
        {
            string ename = entity->d_name;
            if (entity->d_type == DT_DIR && (ename != ".") && (ename != ".."))
            {
                // valid Directory to go
                string ss = source + '/' + ename;
                mode_t perm = getPermission(ss);
                createDir(entity->d_name, destination, perm);
                // if (errno == EEXIST)
                // {
                //     string sd = destination + '/' + ename;
                //     copyDirect(sd, ss);
                //     return 0;
                // }
                // else {
                    
                // }
                string sd = destination + '/' + ename;
                copyDirect(sd, ss);
            }
            else if (entity->d_type == DT_REG)
            {
                copyFile(ename, destination, source);
            }
        }
        return 0;
    }
    return -1;
}
/*##################################################
generic copy method called when copy commnad entered
 ###################################################*/
bool isPathValidForCopy(string destination, string source){

    destination=getAbsolutePath(destination);
    source=getAbsolutePath(source);

    if(destination.size()==0 || source.size()==0 )return false;

    if(source.size()>destination.size())return true;
    string s=destination.substr(0,source.size());
    if(s==source)return false;
    return true;

}
int copyG(string name, string destination, string source)
{
    DIR *dir = opendir(source.c_str());
    if (dir == NULL)
    {
        cout << strerror(errno) << endl;
    }
    else
    {   
        struct dirent *entity;
        bool flag = false;
        string ename;
        while (((entity = readdir(dir)) != NULL) && !flag)
        {
            ename = entity->d_name;
            if (ename == name)
            {
                flag = true;
                break;
            }
        }
        if (flag && (entity->d_type == DT_REG))
        {
            // file with name name found
            string temp=source+'/'+name;
            bool pathValid=isPathValidForCopy(destination,temp);
            if(!pathValid){
                cout<<"\nDestination path is either same as source or child of source";
                return -1;
            }
            if (copyFile(ename, destination, source) == -1)
            {
                return -1;
            }
            else
                return 0;
            // if(createFile(entity->d_name,destination)!=-1){
            //     cout<<"coud not create file at destination: "<<destination<<endl;
            // }
            // else{

            // }
        }
        else if (flag && (entity->d_type == DT_DIR))
        {
            string ss = source + '/' + ename;
            mode_t perm = getPermission(ss);
            // if (createDir(ename, destination, perm) == -1)
            // {
            //     cout << "ERRRO IN CREATE DIR" << endl;
            //     return -1;
            // }
            bool pathValid=isPathValidForCopy(destination,ss);
            if(!pathValid){
                cout<<"\nDestination path is either same as source or child of source";
                return -1;
            }
            createDir(ename, destination, perm);
            string sd = destination + '/' + ename;
            return copyDirect(sd, ss);
            
        }
        else{
            // name could be path 
            name=getAbsolutePath(name);
            if(name.size()==0){
                return -1;
            }
            else{
                bool pathValid=isPathValidForCopy(destination,name);
                if(!pathValid){
                cout<<"\nDestination path is either same as source or child of source";
                return -1;
                }
                if(copyUsingPath(destination,name)==0)return 0;
                
            }

        }
        
    }
    return -1;
}
/*##############################################
copu file/dir with path as source to destination
 ##############################################*/
int copyUsingPath(string destination, string source){
    // source = souce folder + / + name of file/folder
    int idx=source.find_last_of('/');
    string ss= source.substr(0,idx);
    if(ss=="")ss="/";
    string name=source.substr(idx+1,source.size()-(idx+1));

    return copyG(name,destination,ss);



}
/*###########################################
delete file with path
 ###########################################*/
int deleteFile(string path)
{
    path = getAbsolutePath(path);

    if (unlink(path.c_str()) == 0)
    {
        //    cout<<"    Deleted "<<path<<" successfully."<<endl;
        return 0;
    }
    else
        return -1;
}

/*###########################################
delete directory with path
 ###########################################*/
int deleteDir(string path)
{
    // cout<<"    deleting "<<path<<" ...."<<endl;
    path = getAbsolutePath(path);
    DIR *dir = opendir(path.c_str());
    if (dir == NULL)
    {
        cout << strerror(errno) << endl;
        return -1;
    }
    else
    {
        struct dirent *entity;
        while (((entity = readdir(dir)) != NULL))
        {
            string ename;
            ename = entity->d_name;
            string s = path + '/' + ename;
            if (ename == "." || ename == "..")
            {
                continue;
            }
            else if (entity->d_type == DT_DIR && (ename != ".") && (ename != ".."))
            {
                if (deleteDir(s) != 0)
                    return -1;
                // sleep(1);
            }
            else
            {
                if (deleteFile(s) != 0)
                    return -1;
                // sleep(1);
            }
        }
        if (rmdir(path.c_str()) != 0)
            return -1;
        // cout<<"    deleted "<<path<<" successfully"<<endl;
        // sleep(1);
    }
    return 0;
}
/*#####################################################
generic delete method called when delete command entered
 #####################################################*/
int deleteG(string path)
{
    path = getAbsolutePath(path);
    // cout<<"deleting "<<path<<" ...."<<endl;
    DIR *dir = opendir(path.c_str());
    if (dir == NULL)
    {
        cout << strerror(errno) << endl;
        return -1;
    }
    else
    {
        struct dirent *entity;
        string ename;
        while (((entity = readdir(dir)) != NULL))
        {
            ename = entity->d_name;
            string s = path + '/' + ename;
            if (ename == "." || ename == "..")
            {
                continue;
            }
            else if (entity->d_type == DT_DIR && (ename != ".") && (ename != ".."))
            {
                deleteDir(s);
            }
            else
            {
                if (deleteFile(s) != 0)
                    return -1;
            }
        }
        if (rmdir(path.c_str()) != 0)
            return -1;
        // cout<<"deleted "<<path<<" successfully"<<endl;
    }
    return 0;
}
/*##############################################################
move file/dir with name as name from  source to destination(path)
 ##################################################################*/
int move(string name, string path, string source)
{
    // cout<<"\n$name: "<<name <<" $path: "<<path<<" $source: "<<source<<endl;
    path = getAbsolutePath(path);
    bool flag = false;
    DIR *dir = opendir(source.c_str());
    struct dirent *entity;
    string ename;
    if (dir == NULL)
    {
        cout << strerror(errno) << endl;
        return -1;
    }
    else
    {   
        while (((entity = readdir(dir)) != NULL) && !flag)
        {
            ename = entity->d_name;
            if (ename == name)
            {
                flag = true;
                break;
            }
        }
        if (flag && (entity->d_type == DT_DIR) && (ename != ".") && (ename != ".."))
        {
            // copy dir and delete it;
            string ss = source + '/' + ename;
            bool pathValid=isPathValidForCopy(path,ss);
            if(!pathValid){
                cout<<"\nDestination path is either same as source or child of source";
                return -1;
            }
            mode_t perm = getPermission(ss);
            // if (createDir(ename, path, perm) == -1)
            // {
            //     return -1;
            // }
            createDir(ename, path, perm);
            string sd = path + '/' + ename;
            copyDirect(sd, ss);
            // cout<<"DELETED FOLD";
            deleteDir(ss);
            return 0;
        }
        else if (flag && (entity->d_type == DT_REG))
        {
            string temp=source+'/'+name;
            bool pathValid=isPathValidForCopy(path,temp);
            if(!pathValid){
                cout<<"\nDestination path is either same as source or child of source";
                return -1;
            }
            copyFile(name, path, source);
            // cout<<"DELETED FILE";
            string ss = source + '/' + ename;
            if (deleteFile(ss) != 0)
            {
                return -1;
            }
            return 0;
        }
        else{
            // name could be path 
            name=getAbsolutePath(name);
            if(name.size()==0){
                return -1;
            }
            else{
                bool pathValid=isPathValidForCopy(path,name);
                if(!pathValid){
                cout<<"\nDestination path is either same as source or child of source";
                return -1;
                }
                if(moveUsingPath(path,name)==0)return 0;
            }

        }
    }
    return -1;
}

/*################################################
move file/dir from source path to destination path
 ################################################*/
int moveUsingPath(string destination, string source){
    // source = souce folder + / + name of file/folder
    int idx=source.find_last_of('/');
    string ss= source.substr(0,idx);
    if(ss=="")ss="/";
    string name=source.substr(idx+1,source.size()-(idx+1));

    return move(name,destination,ss);
}

/*#################################################
manage input commands and calls respective functions
 #################################################*/
void takeInput()
{
    char ch;
    do
    {
        int size = 0;
        string inp = "";

        while ((ch = cin.get()) != '\n' && ch != 27)
        {
            int as = (int)ch;
            if(as == 127){
                if(inp == ""){
                    continue;
                }else{
                    size--;
                    inp.pop_back();
                    cout << "\b \b";
                }
            }
            else{
                cout << ch;
                inp += ch;
                size++;
            }
        }
        if (ch == '\n' && inp.size() > 0)
        {
            vector<string> command = getCommand(inp);

            // FIND WHICH COMMAND IS WRITTEN
/*###########################################
goto command
 ###########################################*/
            if (command[0] == "goto" && command.size()==2)
            {
                
                string path = command[1];
                path = getAbsolutePath(path);
                if(path.size()==0){
                    cout << "\nError Message: " << strerror(errno);
                    sleep(2);
                }
                else{
                    listDirectory(path.c_str());
                    if(gbPath!=path){
                        backStack.push(gbPath);
                        gbPath=path;
                    }
                }
                displayWindowResetPointers();
            }
/*###########################################
search command
 ###########################################*/
            else if (command[0] == "search" && command.size()>=2)
            {
                searchFound = false;
                search(gbPath, command[1]);
                if (searchFound == true){
                    cout<<endl;
                    string s="Search Result: True";
                    write(STDOUT_FILENO, s.c_str(),s.size());
                }
                else{
                    cout<<endl;
                    string s="Search Result: False";
                    write(STDOUT_FILENO, s.c_str(),s.size());
                }
                sleep(2);
                displayWindowResetPointers();
            }
/*###########################################
quite
 ###########################################*/
            else if (command[0] == "quit")
            {
                exit(1);
            }
/*###########################################
rename command
 ###########################################*/
            else if (command[0] == "rename")
            {
                if (renameFile(command[1], command[2]) == 0)
                {
                    string s="Message: File Renamed Successfully";
                    cout<<endl;
                    write(STDOUT_FILENO, s.c_str(),s.size());
                    listDirectory(gbPath.c_str());
                }
                else{
                    string s(strerror(errno));
                    s="Error Message: "+s;
                    write(STDOUT_FILENO, s.c_str(),s.size());

                }
                sleep(3);
                displayWindowResetPointers();
            }
/*###########################################
create_file command
 ###########################################*/
            else if (command[0] == "create_file")
            {
                int res = createFile(command[1], command[2]);
                if (res == -1)
                {
                    cout<<endl;
                    string s(strerror(errno));
                    s="Error Message: "+s;
                    write(STDOUT_FILENO, s.c_str(),s.size());

                }
                if (res == 0)
                {
                    cout<<endl;
                    string s="Message: File Created Successfully";
                    write(STDOUT_FILENO, s.c_str(),s.size());
                    listDirectory(gbPath.c_str());
                }
                sleep(3);
                displayWindowResetPointers();
            }
/*###########################################
create_dir command
 ###########################################*/
            else if (command[0] == "create_dir")
            {
                int res = createDir(command[1], command[2], (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
                if (res == -1)
                {
                    cout<<endl;
                    string s(strerror(errno));
                    s="Error Message: "+s;
                    write(STDOUT_FILENO, s.c_str(),s.size());
                }
                else if (res == 0)
                {
                    cout << endl;
                    string s= "Message: Directory Created Successfully";
                    write(STDOUT_FILENO, s.c_str(),s.size());
                    listDirectory(gbPath.c_str());
                }
                sleep(2);
                displayWindowResetPointers();
            }
/*###########################################
copy command
 ###########################################*/
            else if (command[0] == "copy")
            {
                size_t sz = command.size();
                int res = 0;
                for (size_t i = 1; i <= sz - 2; i++)
                {
                    res += copyG(command[i], command[sz - 1], gbPath);
                }
                if (res == 0)
                {
                    cout << endl;
                    string s= "Message: Copy successfull";
                    write(STDOUT_FILENO, s.c_str(),s.size());
                }
                else if (res < 0)
                {
                    cout<<endl;
                    string s(strerror(errno));
                    s="Error Message: "+s;
                    write(STDOUT_FILENO, s.c_str(),s.size());
                }
                listDirectory(gbPath.c_str());
                sleep(2);
                displayWindowResetPointers();
            }
/*###########################################
delete_file command
 ###########################################*/
            else if (command[0] == "delete_file")
            {
                if (deleteFile(command[1]) == 0)
                {
                    cout << endl;
                    string s= "Message: Delete File Successfull";
                    write(STDOUT_FILENO, s.c_str(),s.size());
                    listDirectory(gbPath.c_str());
                }
                else
                {
                    cout<<endl;
                    string s(strerror(errno));
                    s="Error Message: "+s;
                    write(STDOUT_FILENO, s.c_str(),s.size());
                }
                sleep(2);
                displayWindowResetPointers();
            }
/*###########################################
delete_dir command
 ###########################################*/
            else if (command[0] == "delete_dir")
            {
                if (deleteG(command[1]) == 0)
                {
                    cout << endl;
                    string s= "Message: Deleted Directory Successfuly";
                    write(STDOUT_FILENO, s.c_str(),s.size());
                    listDirectory(gbPath.c_str());
                }
                else
                {
                    cout<<endl;
                    string s(strerror(errno));
                    s="Error Message: "+s;
                    write(STDOUT_FILENO, s.c_str(),s.size());
                }
                sleep(2);
                displayWindowResetPointers();
            }
/*###########################################
move command
 ###########################################*/
            else if (command[0] == "move")
            {
                size_t sz = command.size();
                int res = 0;
                for (size_t i = 1; i <= sz - 2; i++)
                {
                    res += move(command[i], command[sz - 1],gbPath);
                }
                if (res == 0)
                {
                    cout << endl;
                    string s= "Message: Moved successfull";
                    write(STDOUT_FILENO, s.c_str(),s.size());
                }
                else if (res < 0)
                {
                    cout<<endl;
                    string s(strerror(errno));
                    s="Error Message: "+s;
                    write(STDOUT_FILENO, s.c_str(),s.size());
                }
                // sleep(2);
                // listDirectory(gbPath.c_str());
                // displayWindowResetPointers();
            }
/*###########################################
invalid command
 ###########################################*/
            else{
                cout<<endl;
                string s="Invalid Command..!!";
                write(STDOUT_FILENO, s.c_str(),s.size());
                sleep(2);
                displayWindowResetPointers();
            }
        }
    } while (ch != 27);

    // esc pressed so come out of command mode
    commandMode = false;
    return;
}