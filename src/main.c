#include "../domino.c"
#include "../log.c"

int main(const int argc, const char **argv) {
  initialize();

  return run_interactive();
}
