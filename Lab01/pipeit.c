/*
* @Author: chriskim
* @Date:   2018-04-05 13:56:09
* @Last Modified by:   chriskim
* @Last Modified time: 2018-04-06 17:26:29
*/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define LSERROR "ls returned with error.\n"
#define SORTERROR "sort returned with error.\n"
#define OUTERROR "outfile: Permission denied\n"
#define PIPEERROR "outfile: Permission denied\n"
#define LS "/bin/ls"
#define REVERSE "-r"
#define SORT "/usr/bin/sort"
#define OUTFILE "./outfile"

int main(int argc, char const *argv[])
{
   int WRPipe[2];
   int childPid, childPid2;
   int outFD;
   int status, status2;

   /* if pipe is not made correctly exit with failure */
   if(!pipe(WRPipe)) {

      if ((childPid = fork()) == 0) {

         char *args[] = {LS, NULL};

         /* This closes the reading end of the pipe so there are no lock ups.
            It then sets up the stdout and std err to the writing end of the 
            pipe. */
         close(WRPipe[0]);
         dup2(WRPipe[1], STDOUT_FILENO);
         close(WRPipe[1]);

         execv(LS, args);
      }
      if ((childPid2 = fork()) == 0) {

         char *args[] = {SORT, REVERSE, NULL};

         /* This closes the writing end of the pipe so there are no lock ups.
            It creates a new file named "outfile" */
         close(WRPipe[1]);
         outFD = open(OUTFILE, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);

         /* if file can't be opened it will exit with failure */
         if (outFD == -1) {
            fprintf(stderr, OUTERROR);
            exit(EXIT_FAILURE);
         }
         /* sets up the stdout and std err to the file. */
         dup2(WRPipe[0], STDIN_FILENO);
         dup2(outFD, STDOUT_FILENO);
         close(WRPipe[0]);
         close(outFD);

         execv(SORT, args);
      }
      close(WRPipe[0]);
      close(WRPipe[1]);

      /* exit with failure  if child exits with failure*/
      waitpid(childPid, &status, 0);
      if (WEXITSTATUS(status) != 0) {
         
         fprintf(stderr, LSERROR);
         exit(EXIT_FAILURE);
      }

      waitpid(childPid2, &status2, 0);
      if (WEXITSTATUS(status) != 0) {

         fprintf(stderr, SORTERROR);
         exit(EXIT_FAILURE);
      }

   }
   else {

      fprintf(stderr, PIPEERROR);
      return 1;
   }

   return 0;
}