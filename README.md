# Linux-Based-Command-Line-File-Explorer
## Advanced Operating System

## Intruction for execution
1. To run the File Explorer use command: g++ main.cpp
2. Then use: ./a.out
3. If file or directory name or path have space in it then provide it in ""(double inverted comma).

# Features
## Two mode of execution.
1. Normal Mode: used to explore the current directory and navigate the filesystem.
2. Command Mode: used to enter shell commands.

# Normal Mode
    Display files and directories present in current directory with it's size, permission, username, groupname and name of file/directory.

    left and right arrow used to travers previously visited directory.
    up and down arrow used to go up and down in directory list.
    h key will take user to Home directory.

# Command Mode
## Syntax of commands 
    copy (file/dir)(name/path)... (destination path)
    move (file/dir)(name/path)... (destination path)
    rename (file/dir)(name) (new Name)
    create_file (filename) (destination path)
    create_dir (dirname) (destination path)
    delete_file (filepath)
    delete_dir (dir path)
    goto (dir path)
    search (file/dir Name)
    quit
