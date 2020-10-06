// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:_____________________________________ Date:_____________

// 3460:426 Lab 1 - Basic C shell rev. 9/10/2020

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_ARGS		64
#define MAX_ARG_LEN		16
#define MAX_LINE_LEN	80
#define WHITESPACE		" ,\t\n"

// Define True and False 
#define FALSE 0
#define TRUE 1

struct command_t {
    char *name;
    int argc;
    char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);

// My Function Prototypes
void LCommand();
void handleCommand(struct command_t *);

int main(int argc, char *argv[]) {
    int pid;
    int status;
    char cmdLine[MAX_LINE_LEN];
    struct command_t command;

   while (TRUE) {
        printPrompt();
        /* Read the command line and parse it */
        readCommand(cmdLine);
        parseCommand(cmdLine, &command);
        command.argv[command.argc] = NULL;

	    /*
	        TODO: if the command is one of the shortcuts you're testing for
	        either execute it directly or build a new command structure to
	        execute next
	    */

        //------------------------------------------------------------------
        // The following statements will convert the commands to their linux
        // counterpart or execute them in some other way, so that they work
        // correctly with this shell.
        //------------------------------------------------------------------

        if (*(command.name) == 'Q') // Quit Program command
        {
            break;
        }
        else
        {
            // This will handle the rest of the commands
            handleCommand(&command);
        }
        

        /* Create a child process to execute the command */
        if ((pid = fork()) == 0) {
            /* Child executing command */
            execvp(command.name, command.argv);

            // Execvp failed, give user proper message
            if (command.name[1] == '/') // This is for the 'X' command
            {
                // Executable or command are invalid 
                printf("Please ensure that the executable file and command are valid.\n");
            }
            else
            {
                // If the command isn't valid, let the user know
                printf("Command not found. Please only enter valid commands.\n");
            }
            
            // Exit the process if execvp failed.
            perror(command.name);
            return -1;
        }
        else
        {
            
            // Don't wait for the 'S' command (firefox) to finish
            if (*(command.name) != "firefox")
            {
                /* Wait for the child to terminate */
                wait(&status); /* EDIT THIS LINE */
            }
            
            // Finish 'L' command by listing contents of directory in long form
            if (command.name == "pwd")
            {
                // Skip another line
                printf("\n");

                // Change the command name to the 'ls' linux command
                command.name = "ls";
                command.argv[1] = "-l";

                // Fork Again and exec commands
                if((pid = fork()) == 0)
                {
                    execvp(command.name, command.argv);

                    // If the command is somehow invalid, let the user know
                    printf("Command not found. Please only enter valid commands.\n");
                }
                else
                {
                    wait(&status);
                }
            } 
        }  
   }

    /* Shell termination */
    printf("\n\n Shell: Terminating Successfully\n");
    return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
    int argc;
    char **clPtr;
    /* Initialization */
    clPtr = &cLine;	/* cLine is the command line */
    argc = 0;
    cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
    /* Fill argv[] */
    while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
        cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
    }

    /* Set the command name and argc */
    cmd->argc = argc-1;
    cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
    strcpy(cmd->name, cmd->argv[0]);
    return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - Nutt pp. 79-80 */

void printPrompt() {
    /* Build the prompt string to have the machine name,
     * current directory, or other desired information
     */
    char promptString[] = "linux (nds39) |>";
    printf("%s ", promptString);
}

void readCommand(char *buffer) {
    /* This code uses any set of I/O functions, such as those in
     * the stdio library to read the entire command line into
     * the buffer. This implementation is greatly simplified,
     * but it does the job.
     */
    fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */

void handleCommand(struct command_t *command)
{
    if (strlen(command->name) == 1) // This will ensure that the command length is 1 before converting
    {
        if (*(command->name) == 'C') // This is the copy file option
        {
            // This will convert the command name to a linux command name
            command->name = "cp";
        }
        else if (*(command->name) == 'D') // This is the delete the desired file option
        {
            // This will convert the command name to a linux command name
            command->name = "rm";
        }
        else if (*(command->name) == 'E') // This is the echo command
        {
            // This will convert the command name to a linux command name
            command->name = "echo";
        }
        else if (*(command->name) == 'H')
        {
            // This will set the command to read from an external file.
            command->name = "more";
            command->argv[1] = "shellManual.txt";
        }
        else if (*(command->name) == 'L') // This will print out the current directory contents
        {
            // THIS NEEDS WORK //
            // Skip a line
            printf("\n");
            // This will convert the command name to a linux command name
            command->name = "pwd";
            // Note: The rest of the 'L' command is finished later (below)
        }
        else if (*(command->name) == 'M') // This will create the named text file and launch it in a text editor
        {
            // This will convert the command name to a linux command name
            command->name = "nano";
        }
        else if (*(command->name) == 'P') // This is the print file text option
        {
            // This will convert the command name to a linux command name
            command->name = "more";
        } 
        else if (*(command->name) == 'S') // This is the 'S' command that runs firefox
        {
            // This will convert the command name to a linux command name
            command->name = "firefox";
        }
        else if (*(command->name) == 'W') // This is the clear screen option
        {
            // This will convert the command name to a linux command name
            command->name = "clear";
        }
        else if (*(command->name) == 'X') // This is the run program command
        {
            // Need to concatenate char arrays (strings)

            // Will be string used to concatenate
            char xcommand[30] = "";

            // Append './' to the xcommand
            strcat(xcommand, "./");

            // Append the name of the executable
            strcat(xcommand, command->argv[1]);

            // Set command name to the xcommand
            command->name = xcommand;
        }
    }
}
