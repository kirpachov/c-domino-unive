#include "../domino.c"
#include "../log.c"

int main(const int _argc, const char **_argv) {
  initialize();

  return run_interactive();
}
