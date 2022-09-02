/* 
#idetified bug:
esc then colon will not work: may be using read help here
need to press colon two times
arrow key make transition of command mode to normal mode due to esc
*/
#include "myHeaderFiles.h"

bool commandMode=false;
int main(){

    signal(SIGWINCH, sigWinChHandler);
    setNonCanonicalMode();

    winCols=terminalCols();
    winRows=terminalRows();

    setHomePath();

    listDirectory(gbPath.c_str());
    displayWindowResetPointers();

    // display(0, 0, endPos,vecOfPaths);
    // cout<<"Terminal Rows: "<<terminalRows()<<endl;
    char ch;
    // ch=cin.get();
    while(1){

        if(commandMode){
            // Handle command mode functionlaity here.
            takeInput();
            displayWindowResetPointers();

        }
        else{
            ch = getchar();
            if(ch=='q')break;
            else if(ch==27){
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
            
            else if(ch==':'){
                commandMode=true;
                displayWindowResetPointers();
                // write code to which make enable the printing of commands.
            }
        }

    }
    return 0;
}