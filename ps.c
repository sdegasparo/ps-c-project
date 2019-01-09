#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>

#define DEBUG 0

// Typedef
  typedef enum {false, true} boolean;

// Function prototype
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

    // Just if successful return 0
    return 0;
}
/*
 * Function
 */

/*
const char* getUserID(){
  // get UserID
  unsigned int userID = getuid();
  char sUserID[10];
  sprintf(sUserID, "%d", userID);
  #if DEBUG
    printf("UserID = %d \n", userID);
  #endif
  return sUserID;
}*/

/*
 *  Read directory and check if process is running with UserID,
 *  then call printOutput
 */
void readDirectory(char pid[]){
  /*
   *  Local variable
   */
  char path[50];
  char buffer[100];

  char *nameOutput;
  char name[128];
  char *vmrssOutput;
//  char *vmrssOutputF;
  char vmrss[128];
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
              nameOutput = strtok(name, ":");
              nameOutput = strtok(NULL, ":");

            } else if(strstr(buffer, sUserID)){
                isUse = true;
                #if DEBUG
                  printf("The process is running \n");
                #endif

                }else if(strstr(buffer, "VmRSS:")){
                      //strcpy(vmrss, buffer);

                      unsigned int len = strlen(buffer) - 3;
                      strncpy(vmrss, buffer, len);

                      vmrssOutput = strtok(vmrss, ":");
                      vmrssOutput = strtok(NULL, ":");

                      /*
                     static char* kBSubstring;
                      if(NULL != (kBSubstring = strstr (vmrssOutput, "kB"))){
                        kBSubstring[-1] = 0;
                        kBSubstring[0] = 0;
                        kBSubstring[1] = 0;
                      }*/

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
        printf("%s %s %s", pid, nameOutput, vmrssOutput);
    }
}
