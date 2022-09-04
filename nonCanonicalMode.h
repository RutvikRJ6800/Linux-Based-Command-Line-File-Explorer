/*##############################################
GOLBAL VARIABLE DECLARATION
 ##############################################*/
// struct passwd *pw = getpwuid(getuid());
// struct termios initialAttributes;
// string gbPath;
// string home;


/*##############################################
save home path(/home/username) at gbPath
 ##############################################*/
void setHomePath(){
    // find Home here
    pw = getpwuid(getuid());
    home=pw->pw_dir;
    gbPath=home;
}

/*##############################################
set canonical mode(at exit time)
 ##############################################*/
void setCanonicalMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &initialAttributes);
}

/*##############################################
set non canonical mode
 ##############################################*/
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