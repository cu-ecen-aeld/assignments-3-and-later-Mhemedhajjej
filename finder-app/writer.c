#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>


int main(int argc, char *argv[]) 
{
        /* open syslog */
        openlog("writer_syslog",0, LOG_USER);
        
        if (argc != 3) {
                syslog(LOG_ERR, "Invalid number of arguments passed: %d", argc);
                return 1;
        }
        
        /* get arguments; file name and string to write*/
        char *filename = argv[1];
        char *wrtiestr = argv[2];

        /* open file with write only, create if nonexistent, truncate to 0 length*/
        int f = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (f < 0) {
                syslog(LOG_ERR, "Error opening file %s: %s", filename, strerror(errno));
                return 1;
        }

        /* write string to the file*/
        ssize_t l = write(f, wrtiestr, strlen(wrtiestr));
        if (l < 0) {
                syslog(LOG_ERR, "Error writing to file %s: %s", filename, strerror(errno));
                close(f);
                return 1;
        } else {
                syslog(LOG_DEBUG, "Writing %s to file %s", wrtiestr, filename);
        }

        /* close syslog */
        closelog();
        /* close file descriptor */
        close(f);
        return 0;
}
