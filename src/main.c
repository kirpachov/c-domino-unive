#include "../domino.c"
#include "../nodes.c"

int main(const int argc, const char **argv) {
//  initialize();
//  parse_params(argc, argv);
//
//  if (is_challenge) return run_challenge();
//
//  return run_interactive();

  struct Node *first = create_node(create_domino(1, 2), 0);
  link_nodes(first, create_node(create_domino(4, 4), 0), TOP_LEFT);
  struct Node *n1 = create_node(create_domino(3, 3), 0);
  link_nodes(first, n1, BOTTOM_RIGHT);
//  struct Node *vert = create_node(create_domino(5, 6), 1);
//  link_nodes(n1, vert, BOTTOM_RIGHT);

//  struct Node *vert_top_right = create_node(create_domino(7, 8), 0);
//  link_nodes(vert, vert_top_right, TOP_RIGHT);

//  struct Node *vert_top_right1 = create_node(create_domino(8, 9), 1);
//  link_nodes(vert_top_right, vert_top_right1, BOTTOM_RIGHT);

//  link_nodes(vert_top_right, create_node(create_domino(9, 5), 1), TOP_RIGHT);


//  struct Node *vert_bottom_right = create_node(create_domino(9, 3), 0);
//  link_nodes(vert, vert_bottom_right, BOTTOM_RIGHT);
//  struct Node *vert_left = create_node(create_domino(9, 3), 0);
//  link_nodes(vert, vert_left, BOTTOM_LEFT);
//
//  struct Node *vert_left_l0 = create_node(create_domino(1, 5), 0);
//  link_nodes(vert_left, vert_left_l0, TOP_LEFT);
//
//  struct Node *vert_left_l1 = create_node(create_domino(2, 2), 0);
//  link_nodes(vert_left_l0, vert_left_l1, TOP_LEFT);
//
//  struct Node *vert_left_l2 = create_node(create_domino(4, 3), 0);
//  link_nodes(vert_left_l1, vert_left_l2, TOP_LEFT);

  int width = 11, height = 10;

  char **matrix = malloc(sizeof(char *) * height);
  for (int i = 0; i < height; i++) {
    matrix[i] = calloc(width, sizeof(char));
    for (int j = 0; j < width; j++) {
      matrix[i][j] = ' ';
    }
  }

  first = get_most_left_node(first);
  write_nodes(matrix, first, 0, 0, &width, &height, false);

  print_matrix(matrix, width, height);

  return 0;
}