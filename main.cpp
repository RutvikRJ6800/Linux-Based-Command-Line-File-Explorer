#include "myHeaderFiles.h"

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
