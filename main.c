//Eric Chiu
//2017-4-18
//CSCI 340 Project 1
//Making a shell



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

void u_shell(char* const input, int* input_count); //Runs the system calls for the shell

int main()
{
    int input_count = 0;
    char* input_array[9];

    printf("Please enter your shell command. \n");
    printf("Enter \"end\" to exit the shell. \n");
    printf("Enter \"myhis\" to access the history feature. \n");

    while(1) { //while loop
      printf("osh> ");

      //start getline setup
      char *input; //hold user input
      size_t characters;
      size_t buffer_size = 32;
      input = (char *)malloc(buffer_size * sizeof(char));

      if( input == NULL) {
          perror("Out of memory");
          exit(1);
      }

      characters = getline(&input ,&buffer_size, stdin);
      //end getline setup

      //start remove newline
      char *nl = strchr(input, '\n'); //getline also includes the newline. This section removes it.
      if (nl) {
        *nl = 0;
      }
      //end remove newline

      if (strcmp(input, "end") == 0) { //Quit my shell
        break;
      }
      else if (strcmp(input, "myhis") == 0) {
        //history feature
        if (input_count == 0) {
          printf("There are no recently entered commands.\n");
        }
        else if (input_count >= 10) { //start print command history
          int backw = 10; //Need to print the array backwards

          for (int i = 0; i < 10; i++) {
            printf("%d ", backw);
            backw--;
            printf("%s \n", input_array[i]);
          }
        }
        else {
          int backw = input_count;

          for (int i = 0; i < input_count; i++) {
            printf("%d ", backw);
            backw--;
            printf("%s \n", input_array[i]);
          }
        }//end print command history

        if (input_count < 10) {//run command when less than 10
          printf("Enter the command you would like to re-execute. \n");
          printf("\"!!\" for the most recent command or ");
          printf("\"!#\" where # is the number next to the command.\n");

          char h_input[2];
          fflush(stdin);
          fgets(h_input, 4, stdin);
          if (h_input[0] == '!') {
            if (h_input[1] == '!') {
              printf("%s\n", input_array[input_count - 1]);
              u_shell(input_array[input_count - 1], &input_count);
            }
            else if (isdigit(h_input[1])) {
              int a = h_input[1] - '0';
              if (a > input_count) {
                printf("Invalid command.\n");
              }
              else {
                printf("%s\n", input_array[input_count - a]);
                u_shell(input_array[input_count - a], &input_count);
              }
            }
            else {
              printf("Invalid input2\n");
            }
          }
          else {
            printf("Invalid input1\n");
          }
        }//end run when less than 10
        else if (input_count > 0) {//run command when greater than 0
          printf("Enter the command you would like to re-execute. \n");
          printf("\"!!\" for the most recent command or ");
          printf("\"!#\" where # is the number next to the command.\n");

          char h_input[1];
          fflush(stdin);
          fgets(h_input, 3, stdin);
          if (h_input[0] == '!') {
            if (h_input[1] == '!') {
              printf("%s\n", input_array[9]);
              u_shell(input_array[9], &input_count);
            }
            else if (isdigit(h_input[1])) {
              int a = h_input[1] - '0';
              printf("%s\n", input_array[a]);
              u_shell(input_array[a], &input_count);
            }
            else {
              printf("Invalid input2\n");
            }
          }
          else {
            printf("Invalid input1\n");
          }
        }//end run command when greater than 0

      }//end history feature
      else { //start else loop
        //start track input history
        if (input_count >= 10) {
          for (int i = 0; i < 8; i ++) {
            input_array[i] = input_array[i+1];
          }
          input_array[9] = input;
        }
        else {
          input_array[input_count] = input;
          input_count++; //should stop at 10
        }//end track input history
        u_shell(input, &input_count);
      }//end else loop
    } //end while loop

    return 0;
}

void u_shell(char* const input, int* input_count) {
  //start strtok setup
  char cinput[255];
  strcpy(cinput, input); //Make a copy of the string for strtok

  char *token;
  token = strtok(cinput, " ");

  char *args[10];
  int count = 0;
  while (token != NULL) {
    args[count] = token;
    count++;
    if (count > 10) {
      perror("Command too long. \n");
    }
    token = strtok (NULL, " ,");
  }

  args[count] = NULL;
  //end strtok setup

  //start fork setup
  pid_t pid = fork();
  if (pid == 0) {
    //child
    if (execvp(args[0], args) == -1) {
      *input_count = *input_count - 2;
      perror("execvp() failed to run.\n");
    }
    exit(EXIT_FAILURE);
  }
  else if (pid > 0) {
    //parent
    int status;
    waitpid(pid, &status, 0);
  }
  else {
    perror("Failed to run fork(). \n");
  }
  //end fork setup
}
