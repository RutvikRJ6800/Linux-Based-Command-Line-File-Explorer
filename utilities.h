// struct winsize wins;


/*##############################################
return terminal window's rows
 ##############################################*/
size_t terminalRows(){
	ioctl(STDIN_FILENO, TIOCGWINSZ, &wins);
    return wins.ws_row;
}

/*##############################################
return terminal window's cols
 ##############################################*/
size_t terminalCols(){
	ioctl(STDIN_FILENO, TIOCGWINSZ, &wins);
    return wins.ws_col;
}

/*##############################################
split strings with delimiter
 ##############################################*/
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

/*##############################################
empty backstack
 ##############################################*/
void emptyBackStack(){
    while(!backStack.empty()){
        backStack.pop();
    }
}

/*##############################################
empty forwardstack
 ##############################################*/
void emptyForStack(){
    while(!forStack.empty()){
        forStack.pop();
    }
}

/*##############################################
convert string to lowercase
 ##############################################*/
void convertLower(string &test){
    for(auto& c : test){
        c = tolower(c);
    }
}

/*##############################################
comparator used to sort string
 ##############################################*/
bool cmp2(string &a, string &b){
    string st1=a;
    string st2=b;

    convertLower(st1);
    convertLower(st2);

    return st1<st2;

}
