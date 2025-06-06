#include "systemcalls.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    int result = system(cmd);
    if (result == -1 || ((WIFEXITED(result) && WEXITSTATUS(result) != 0))) {
        // system call failed or command returned a non-zero exit status
        return false;
    }

    // system call succeeded and command returned a zero exit status
    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    
        if (command[0] == NULL || command[0][0] != '/') {
                // Invalid command
                va_end(args);
                return false;
        }
        
        fflush(stdout);
        pid_t pid = fork();
        if (pid == -1) {
                // Fork failed
                va_end(args);
                return false;
        } else if (pid == 0) {
                // Child process
                execv(command[0], command);
                // If execv returns, it must have failed
                va_end(args);
                return false;
        } else {
                // Parent process
                int stat_loc;
                waitpid(pid, &stat_loc, 0);
                if (WIFEXITED(stat_loc) && WEXITSTATUS(stat_loc) == 0) {
                        // Command executed successfully
                        va_end(args);
                        return true; 
                } else {
                        // Command failed
                        va_end(args);
                        return false;
                }
        }

        va_end(args);
        return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
        va_list args;
        va_start(args, count);
        char * command[count+1];
        int i;
        for(i=0; i<count; i++)
        {
                command[i] = va_arg(args, char *);
        }
        command[count] = NULL;
        // this line is to avoid a compile warning before your implementation is complete
        // and may be removed
        command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
        bool success = true;
        pid_t pid;
        int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
        if (fd < 0) {
                // Failed to open file for writing
                va_end(args);
                return false;
        }
        
        if (command[0] == NULL || command[0][0] != '/') {
                // Invalid command
                close(fd);
                va_end(args);
                return false;
        }
        fflush(stdout); // Ensure stdout is flushed before redirecting
        switch (pid = fork()) {
        case -1: 
                success = false;
                break;
        case 0:
                if (dup2(fd, 1) < 0) {success = false;}
                if (success) {
                        close(fd);
                        execvp(command[0], command);
                        success = false;
                }
                break;
        default:
                // Parent process
                close(fd);
                int stat_loc;
                waitpid(pid, &stat_loc, 0);
                if (WIFEXITED(stat_loc) && WEXITSTATUS(stat_loc) == 0) {
                        // Command executed successfully
                        success = true;
                } else {
                        // Command failed
                        success = false;
                }
                break;
        }

        va_end(args);
        return success;
}
