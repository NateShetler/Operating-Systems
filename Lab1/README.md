# Operating Systems Lab 1 - Nathaniel Shetler

How to Compile:
    This project contact a Makefile. To compile and make an executable, simply type "make". This will create an executable named "lab1"

How to Use Shell:
    To use this shell, you can use any of the commands listed in the "shellManual.txt". "shellMnaual.txt" contains the list of shell commands and what they do. In addition, you can simply use regular linux commands.

Additional Functions that `Shell Contains:
    void handleCommand(struct command_t *command) - This function will accept a 'command' object. It will use the command name entered for this shell and then convert it to the correspoding linux command so that it can be passed to execvp.

    void LCommand(struct command_t *command) - This function will accept a 'command' object. It will run the proper operations for the 'L' command of the shell. It will set "pwd" as the command name the first time it is called, so that the directory is printed out, and then the second time it is called, it will set the command to be "ls -l" to list out the contents of the directory in long form.
