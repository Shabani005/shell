#define NB_IMPLEMENTATION
#include "nb.h"

int main(int argc, char** argv){
  nb_rebuild(argc, argv);

  nb_arr cmd = {0};
  nb_append_da(&cmd, "cc");
  nb_append_da(&cmd, "-o");
  nb_append_da(&cmd, "shell");
  nb_append_da(&cmd, "./shell.c");
  nb_append_da(&cmd, "-lreadline");
  // nb_append_da(&cmd, "-static");
    
  nb_cmd(&cmd);
  return 0;
}
