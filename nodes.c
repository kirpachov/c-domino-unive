//
// Created by sasha on 23/01/24.
//

#include "nodes.h"


static unsigned long int node_last_id = 0;

struct Node *create_node(struct Domino *domino, bool is_vert) {
  node_last_id++;
  struct Node *node = malloc(sizeof(struct Node));
  node->domino = domino;
  node->is_vert = is_vert;
  node->id = node_last_id;
  node->top_left = NULL;
  node->top_right = NULL;
  node->bottom_left = NULL;
  node->bottom_right = NULL;
  return node;
}

struct Domino *create_domino(int left, int right) {
  struct Domino *domino = malloc(sizeof(struct Domino));
  domino->left = left;
  domino->right = right;
  return domino;
}

/**
 * @param parent is the node that is already part of the tree.
 * @param child node to add.
 * @param side where to add the child node.
 */
void link_nodes(struct Node *parent, struct Node *child, int side) {
  switch (side) {
    case TOP_LEFT:
      parent->top_left = child;
      child->bottom_right = parent;
      break;
    case TOP_RIGHT:
      if (!parent->is_vert) {
        printf("Tried to link top right node to a horizontal node\n");
        exit(EXIT_FAILURE);
      }

      parent->top_right = child;
      child->top_left = parent;
      break;
    case BOTTOM_LEFT:
      if (!parent->is_vert) {
        printf("Tried to link bottom left node to a horizontal node\n");
        exit(EXIT_FAILURE);
      }

      parent->bottom_left = child;
      child->bottom_right = parent;

      break;
    case BOTTOM_RIGHT:
      parent->bottom_right = child;
      child->top_left = parent;
      break;
    default:
      log_fatal("Invalid side: %d", side);
      exit(1);
  }
}

void write_node(const struct Node *node, char **matrix, const int x, const int y) {
  if (node == NULL) {
    printf("node is NULL at write_node\n");
    exit(EXIT_FAILURE);
  }

  // TODO check if cells are all empty, throw error otherwise.

  if (!node->is_vert) {
    matrix[y + 0][x + 0] = '[';
    matrix[y + 0][x + 1] = int_to_char(node->domino->left);
    matrix[y + 0][x + 2] = '|';
    matrix[y + 0][x + 3] = int_to_char(node->domino->right);
    matrix[y + 0][x + 4] = ']';
    return;
  }

  matrix[y + 0][x + 0] = '[';
  matrix[y + 0][x + 1] = int_to_char(node->domino->left);
  matrix[y + 0][x + 2] = ':';
  matrix[y + 1][x + 0] = ':';
  matrix[y + 1][x + 1] = int_to_char(node->domino->right);
  matrix[y + 1][x + 2] = ']';
}

/**
 * Will add (quantity) empty columns before selected index (x)
 * @param matrix
 * @param x
 * @param quantity
 * @param matrix_width
 * @param matrix_height
 */
void add_empty_columns_before(char **matrix, const int x, const int quantity, const int matrix_width,
                              const int matrix_height) {
  for (int i = 0; i < matrix_height; i++) {
    char *row_tmp_copy = calloc(matrix_width, sizeof(char));
    for (int j = 0; j < matrix_width; j++) {
      row_tmp_copy[j] = matrix[i][j];
    }


    matrix[i] = calloc(matrix_width + quantity, sizeof(char));
    for (int j = 0; j < quantity; j++) {
      matrix[i][j] = ' ';
    }

    for (int j = 0; j < matrix_width; j++) {
      matrix[i][j + quantity] = row_tmp_copy[j];
    }

    // TODO free
  }
}

void print_matrix(char **matrix, const int width, const int height) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      printf("%c", matrix[i][j]);
    }

    printf("\n");
  }
}

/**
 *
 * @param matrix
 * @param node top-left node.
 * @param x
 * @param y
 * @param matrix_width
 * @param matrix_height
 * @param rtl boolean value. When true, we're going from right to left (<==).
 * Otherwise, we're going from left to rigt (==>)
 */
void write_nodes(
    char **matrix,
    struct Node *node,
    const int x,
    const int y,
    int *matrix_width,
    int *matrix_height,

    const bool rtl
) {
  printf("write_nodes | node#id: %ld | x: %d, y: %d | domino: [%d|%d]\n", node->id, x, y, node->domino->left,
         node->domino->right);

  if (x < 0) {
    printf("ERROR: x < 0");
    exit(EXIT_FAILURE);
  }

  if (y < 0) {
    printf("ERROR: y < 0");
    exit(EXIT_FAILURE);
  }

  write_node(node, matrix, x, y);

  if (node->bottom_right && !rtl) {
    // We're going right ==>
    write_nodes(matrix, node->bottom_right, x + (node->is_vert ? 3 : 5), y + (node->is_vert ? 1 : 0), matrix_width,
                matrix_height, false);
  }

  if (node->is_vert) {
    // Bottom-left positions are only for vertical nodes.
    if (node->bottom_left) {
      // TODO if node->bottom_left is vertical, check there is enough space to write it.
      const int width_needed = node->bottom_left->is_vert ? 3 : 5;
//      const int height_needed = node->bottom_left->is_vert ? 2 : 1;
      if (x < width_needed) {
        const int toadd = width_needed - x;
        add_empty_columns_before(matrix, x, toadd, *matrix_width, *matrix_height);
        *matrix_width = (*matrix_width) + toadd;
      }

      // We're going left <==
      write_nodes(matrix, node->bottom_left, x - 5, y + 1, matrix_width,
                  matrix_height, true);
    }

    // top-right positions are for vertical nodes only.
    if (node->top_right) {
      const int width_needed = node->top_right->is_vert ? 3 : 5;
//      const int height_needed = node->top_right->is_vert ? 2 : 1;
      if (x + width_needed > *matrix_width) {
        const int toadd = (width_needed + x) - *matrix_width;
        add_empty_columns_before(matrix, *matrix_width, toadd, *matrix_width, *matrix_height);
        *matrix_width = (*matrix_width) + toadd;
      }

      // We're going right ==>
      write_nodes(matrix, node->top_right, x + 3, y, matrix_width,
                  matrix_height, false);
    }
  }

  if (node->top_left && rtl) {
    const int width_needed = node->top_left->is_vert ? 3 : 5;
    const int height_needed = node->top_left->is_vert ? 2 : 1;
    if (x < width_needed) {
      const int toadd = width_needed - x;
      add_empty_columns_before(matrix, x, toadd, *matrix_width, *matrix_height);
      *matrix_width = (*matrix_width) + toadd;
    }

    // Going left <==
    write_nodes(matrix, node->top_left, x - width_needed < 0 ? 0 : x - width_needed, y, matrix_width, matrix_height,
                true);
  }
}

struct Node *get_most_left_node(struct Node *node) {
  return node->top_left ? get_most_left_node(node->top_left) : node;
}























