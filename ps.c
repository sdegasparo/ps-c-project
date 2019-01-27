#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>

#define DEBUG 0

/*
 ************** Typedef ********************
 */
  typedef enum {false, true} boolean;

  /*
   ******** Function prototype ************
   */
void readDirectory(char pid[]);
void printOutput(boolean isUse, char pid[], char nameOutput[], char vmrssOutput[]);

int main(void){
    // Pointer for directory entry
    struct dirent *dirent;

    DIR *dir = opendir("/proc");

    if (dir == NULL){
        #if DEBUG
          printf("Could not open current directory");
        #endif

        // Need to return 1 when error?
        return 1;
    }

    while ((dirent = readdir(dir)) != NULL){
        readDirectory(dirent->d_name);
    }

    closedir(dir);

    return 0;
}
/*
 ************** Function ********************
 */

/*
 *  Read directory and check if process is running with UserID,
 *  then call printOutput
 */
void readDirectory(char pid[]){
  /*
   *  Variable
   */
  char path[50];
  char buffer[128];

  char name[128];
  static char *nameOutput;
  char *saveName = name;

  char vmrss[128];
  static char *vmrssOutput;
  char *saveVmrss = vmrss;


  boolean isUse = false;

  /*
   *  Build the path for the current pid status file
   *  and open the file for reading
   */
  sprintf(path, "/proc/%s/status", pid);

  FILE *fptr;
  fptr = fopen(path, "r");
  if(fptr == NULL){
      #if DEBUG
          printf("PID: %s couldn't open \n", pid);
      #endif
    }else{
        #if DEBUG
            printf("PID: %s open successful \n", pid);
        #endif

        /*
         *  get UserID
         */
        unsigned int userID = getuid();
        char sUserID[10];
        sprintf(sUserID, "%d", userID);

        #if DEBUG
          printf("UserID = %d \n", userID);
        #endif

        /*
         *  Read the File line to line and save the Name, VmRSS.
         *  Transfer the variable to printOutput, to print the Output,
         *  when the process is running with the UserID.
         */
        while(fgets(buffer, sizeof(buffer), fptr)){
            if(strstr(buffer, "Name:")){

              strcpy(name, buffer);
              nameOutput = strtok_r(name, ":", &saveName);
              nameOutput = strtok_r(NULL, ":", &saveName);

              /*
               *  Remove new line "\n"
               */
              for(unsigned int i = strcspn (nameOutput, "\n"); i < strlen(nameOutput); i++){
                nameOutput[i] = 0;
              }

            } else if(strstr(buffer, "Uid:") && strstr(buffer, sUserID)){
                isUse = true;

                #if DEBUG
                  printf("The process is running \n Found Uid on this line: %s", buffer);
                #endif

                }else if(strstr(buffer, "VmRSS:")){
                      strcpy(vmrss, buffer);
                      vmrssOutput = strtok_r(vmrss, ":", &saveVmrss);
                      vmrssOutput = strtok_r(NULL, ":", &saveVmrss);

                      static char* deletekB;
                      if(NULL != (deletekB = strstr (vmrssOutput, "kB"))){
                        deletekB[-1] = 0;
                        deletekB[0] = 0;
                        deletekB[1] = 0;
                      }

                      printOutput(isUse, pid, nameOutput, vmrssOutput);
                    }

        }// close while reading

      }// close open File successful

}// close isOpen Function

/*
 *  Print the Output to the stdout
 */
void printOutput(boolean isUse, char pid[], char nameOutput[], char vmrssOutput[]){
    if(isUse){
        printf("%-10s %-20s %s \n", pid, nameOutput, vmrssOutput);
    }
}
