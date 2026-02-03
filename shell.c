#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <pwd.h>
#include <limits.h>

// #include <editline/readline.h>
#include <stdbool.h>
#include <sys/wait.h>

#define CHAOS_IMPLEMENTATION
#include "chaos.h"

#define MAX_ARGS 128

const char* getuser(){
  struct passwd *pw = getpwuid(getuid());
  return pw ? pw->pw_name : "unknown";
}

const char* getdir(){
  static char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd))) return cwd;
  return "?";
}


int main(void){
  bool config_exist = does_file_exist("~/.shellrc");

  char* line;
  char* arg;
  char* args[MAX_ARGS];
  

  while (true){
    signal(SIGINT, SIG_IGN);
    memset(args, 0, sizeof(args));
    line = readline(temp_sprintf("\033[31m%s\033[0m:%s \033[37m$\033[0m ", getuser(), getdir()));


    size_t arg_count = 0;

    if (!line) break;
    if (*line) add_history(line);

    String_View sv_line = sv_from_cstr(line);

    while (sv_line.count > 0 && arg_count < MAX_ARGS - 1) {
      String_View head = split_by_delim(&sv_line, ' ');
      arg = sv_to_cstr(&head);
      args[arg_count++] = arg;    
    }

    args[arg_count] = NULL;
               
    if (strcmp(args[0], "exit") == 0){
      break;
    } else if (strcmp(args[0], "cd") == 0){
      if (args[1]){
        if (chdir(args[1]) != 0) perror("cd");
      } else {
        char *home = getenv("HOME");
        chdir(home);
      }
    } else {
      pid_t pid = fork();
      if (pid == 0){
        signal(SIGINT, SIG_DFL);
        execvp(args[0], args);
        perror("execvp");
        _exit(1);
    } else if (pid > 0) {
      waitpid(pid, NULL, 0);
    } else {
      perror("fork");
    }
    }
  }
  return 0;
}
