#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

// #include <editline/readline.h>
#include <stdbool.h>
#include <sys/wait.h>

#define NB_IMPLEMENTATION
#include "nb.h"

#define MAX_ARGS 128

int main(void){
  char* line;
  // char *args[MAX_ARGS];

  while (true){
    line = readline("> ");

    if (!line) break;
    if (*line) add_history(line);

    char** args = nb_split_by_delim(line, ' ');

    // for (int i = 0; args[i]; i++) {
    //   printf("argv[%d] = '%s'\n", i, args[i]);
    // }
    
    if (!args || !args[0]){
      free(args);
      free(line);
      continue;
    }

    if (strcmp(args[0], "exit") == 0){
      free(args);
      free(line);
      break;
    } else if (strcmp(args[0], "cd") == 0){
      if (args[1]){
        if (!chdir(args[1])) perror("cd");
      } else {
        char *home = getenv("HOME");
        chdir(home);
      }
    } else {
    pid_t pid = fork();
    if (pid == 0){
      execvp(args[0], args);
      perror("execvp");
      _exit(1);
    } else if (pid > 0) {
      waitpid(pid, NULL, 0);
    } else {
      perror("fork");
    }
    free(args);
    free(line);
    }
  }
  return 0;
}
