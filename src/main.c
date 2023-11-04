#include "../domino.c"
#include "../log.c"

int main(const int argc, const char **argv) {
  initialize();
  parse_params(argc, argv);

  if (is_challenge) return run_challenge();

  return run_interactive();
}