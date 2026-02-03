#define CHAOS_IMPLEMENTATION
#include "chaos.h"

int main(int argc, char** argv){
  rebuild(argc, argv, __FILE__);

  cmd_arr cmd = {0};
  cmd_append(&cmd, "cc");
  cmd_append(&cmd, "-o");
  cmd_append(&cmd, "shell");
  cmd_append(&cmd, "./shell.c");
  cmd_append(&cmd, "-lreadline");
    
  cmd_run(&cmd);
  return 0;
}
