#include "../domino.c"
#include "../log.c"

int main(const int argc, const char **argv) {
  initialize();

  struct Node *node = create_node(create_domino(1, 2), true);
  struct Node *node34 = create_node(create_domino(3, 4), true);
  link_nodes(node, node34, BOTTOM_RIGHT);
//  link_nodes(node34, create_node(create_domino(4, 5), true), BOTTOM_RIGHT);

  struct Domino selected_domino = (struct Domino) {1, 5};
  log_debug("DOMINO: %s", format_domino(selected_domino));

  add_moves_to_tree(node, selected_domino);

  print_table(node);

//  struct Node *node = create_node(create_domino(1, 2), false);
//  link_nodes(node, create_node(create_domino(2, 3), true), BOTTOM_RIGHT);
//
//  struct Domino selected_domino = (struct Domino) {1, 3};
//
//  add_moves_to_tree(node, selected_domino);
//
//  print_table(node);



//
//  return run_interactive();


//  printf("Command: %s\n", command);
//  int status = STATUS_BEGIN;
//  initialize();
//  parse_params(argc, argv);
//
//  if (is_challenge) return run_challenge();
//
//  return run_interactive();

//  struct Node *node = create_node(create_domino(1, 2), true);
//  struct Node *node34 = create_node(create_domino(3, 4), true);
//  link_nodes(node, node34, BOTTOM_RIGHT);
//  link_nodes(node34, create_node(create_domino(4, 5), true), BOTTOM_LEFT);
//
//  int width = 1, height = 1;
//
//  char **matrix = malloc(sizeof(char *) * height);
//  for (int i = 0; i < height; i++) {
//    matrix[i] = calloc(width, sizeof(char));
//    for (int j = 0; j < width; j++) {
//      matrix[i][j] = ' ';
//    }
//  }
//
//  write_nodes(matrix, get_most_left_node(node), 0, 0, &width, &height);
//
//  print_matrix(matrix, width, height);

  return 0;
}
