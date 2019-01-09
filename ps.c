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
boolean isOpen();


int main(void){

  // Pointer for directory entry
  struct dirent *dirent;

  DIR *dir = opendir("/proc");

  if (dir == NULL){
    printf("Could not open current directory");
    // Need to return 1 when error?
    return 1;
  }

  while ((dirent = readdir(dir)) != NULL){
    isOpen(dirent->d_name);

  // Print all folder in "/proc"
  //printf("%s\n", dirent->d_name);
  }

  // Just if successful return 0
  return 0;
}

// Function

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

// Check if Process is running with UserID
boolean isOpen(char pid[]){
  char path[50];
  sprintf(path, "/proc/%s/status", pid);

  // Open Files
  FILE *fptr;
  fptr = fopen(path, "r");
  if(fptr == NULL){
    #if DEBUG
      printf("PID: %s couldn't open \n", pid);
    #endif

    return false;
  }else{
    #if DEBUG
      printf("PID: %s open successful \n", pid);
    #endif

    // Search for Uid
  char buffer[100]; // Buffer should allocat automaticly

  // get UserID
  unsigned int userID = getuid();
  char sUserID[10];
  sprintf(sUserID, "%d", userID);
  /*#if DEBUG
    printf("UserID = %d \n", userID);
  #endif*/

    while(fgets(buffer, sizeof(buffer), fptr)){
      //printf("sUserId is = %s \n", sUserID);
      //printf("%s", buffer);
      if(strstr(buffer, sUserID)){
        #if DEBUG
          printf("The program is running \n");
        #endif

        // get pid
        //unsigned int pid = 1035;

        // get rss
        unsigned int rss = 8888;

        printf("%s\t%u\n", pid, rss);

        break;
      }else{
      //  printf("The programm isn't running");
      }
    }

    return true;
  }

}
