#include "test-framework/unity.h"
#include "domino.h"

void setUp(void) {
  initialize();
}

void tearDown(void) {
}

static void assert_node_print_format(struct Node *node, char *expected_str) {
  int width = 1, height = 1;

  char **matrix = init_matrix(width, height);

  write_nodes(matrix, get_most_left_node(node), 0, 0, &width, &height);

  char *actual_str = matrix_to_str(matrix, width, height);

//  printf("%s", actual_str);

  TEST_ASSERT_EQUAL_STRING(expected_str, actual_str);

  free(actual_str);
  free_matrix(matrix, height);
}

static void test_write_nodes_0(void) {
  struct Node *first = create_node(create_domino(1, 2), false);
  struct Node *second = create_node(create_domino(2, 3), true);
  link_nodes(first, second, BOTTOM_RIGHT);
  struct Node *third = create_node(create_domino(3, 4), false);
  link_nodes(second, third, BOTTOM_RIGHT);
  struct Node *fourth = create_node(create_domino(4, 5), true);
  link_nodes(third, fourth, BOTTOM_RIGHT);
  struct Node *fifth = create_node(create_domino(6, 5), false);
  link_nodes(fourth, fifth, BOTTOM_LEFT);
  struct Node *sixth = create_node(create_domino(6, 6), false);
  link_nodes(fifth, sixth, TOP_LEFT);
  struct Node *seventh = create_node(create_domino(1, 6), false);
  link_nodes(sixth, seventh, TOP_LEFT);
  struct Node *eighth = create_node(create_domino(1, 1), true);
  link_nodes(seventh, eighth, TOP_LEFT);
  struct Node *ninth = create_node(create_domino(2, 1), false);
  link_nodes(eighth, ninth, TOP_LEFT);
  struct Node *tenth = create_node(create_domino(2, 2), true);
  link_nodes(ninth, tenth, TOP_LEFT);

  assert_node_print_format(first, "             [1|2][2:        \n"
                                  "                  :3][3|4][4:\n"
                                  ":2][2|1]:1][1|6][6|6][6|5]:5]\n"
                                  "[2:     [1:                  \n");
}

static void test_write_nodes_1(void) {
  struct Node *node = create_node(create_domino(1, 2), true);
  struct Node *node2 = create_node(create_domino(2, 3), true);
  link_nodes(node, node2, BOTTOM_RIGHT);
  struct Node *node3 = create_node(create_domino(3, 4), true);
  link_nodes(node2, node3, BOTTOM_LEFT);

  assert_node_print_format(node, "[1:   \n"
                                 ":2][2:\n"
                                 ":3]:3]\n"
                                 "[4:   \n");
}

static void test_write_nodes_2(void) {
  struct Node *node = create_node(create_domino(1, 2), true);
  struct Node *node2 = create_node(create_domino(2, 3), true);
  link_nodes(node, node2, BOTTOM_RIGHT);
  struct Node *node3 = create_node(create_domino(3, 4), true);
  link_nodes(node2, node3, BOTTOM_LEFT);
  struct Node *node4 = create_node(create_domino(4, 5), true);
  link_nodes(node3, node4, BOTTOM_LEFT);

  assert_node_print_format(node, "   [1:   \n"
                                 "   :2][2:\n"
                                 "   :3]:3]\n"
                                 ":4][4:   \n"
                                 "[5:      \n");
}

static void test_write_nodes_3(void) {
  struct Node *node = create_node(create_domino(1, 2), true);
  struct Node *node2 = create_node(create_domino(2, 3), true);
  link_nodes(node, node2, BOTTOM_RIGHT);
  struct Node *node3 = create_node(create_domino(3, 4), true);
  link_nodes(node2, node3, BOTTOM_LEFT);
  struct Node *node4 = create_node(create_domino(4, 5), true);
  link_nodes(node3, node4, BOTTOM_LEFT);

  assert_node_print_format(node, "   [1:   \n"
                                 "   :2][2:\n"
                                 "   :3]:3]\n"
                                 ":4][4:   \n"
                                 "[5:      \n");
}

static void test_write_nodes_4(void) {
  struct Node *node = create_node(create_domino(1, 2), true);
  struct Node *node2 = create_node(create_domino(2, 3), true);
  link_nodes(node, node2, BOTTOM_RIGHT);
  struct Node *node3 = create_node(create_domino(3, 4), true);
  link_nodes(node2, node3, BOTTOM_LEFT);
  struct Node *node5 = create_node(create_domino(4, 6), true);
  link_nodes(node3, node5, BOTTOM_RIGHT);

  assert_node_print_format(node, "[1:   \n"
                                 ":2][2:\n"
                                 ":3]:3]\n"
                                 "[4:[4:\n"
                                 "   :6]\n");
}

static void test_write_nodes_5(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *node3 = create_node(create_domino(3, 4), true);
  link_nodes(node, node3, BOTTOM_RIGHT);
  link_nodes(node3, create_node(create_domino(4, 5), false), BOTTOM_LEFT);

  assert_node_print_format(node,
                           "[1|2][3:\n"
                           "[4|5]:4]\n");
}

static void test_write_nodes_6(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *node3 = create_node(create_domino(3, 4), true);
  link_nodes(node, node3, BOTTOM_RIGHT);
  link_nodes(node3, create_node(create_domino(4, 5), false), BOTTOM_RIGHT);

  assert_node_print_format(node,
                           "[1|2][3:     \n"
                           "     :4][4|5]\n");
}

static void test_write_nodes_7(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *node3 = create_node(create_domino(3, 4), true);
  link_nodes(node, node3, BOTTOM_RIGHT);
  link_nodes(node3, create_node(create_domino(4, 5), true), BOTTOM_RIGHT);

  assert_node_print_format(node,
                           "[1|2][3:   \n"
                           "     :4][4:\n"
                           "        :5]\n");
}

static void test_write_nodes_8(void) {
  struct Node *node = create_node(create_domino(1, 2), true);
  struct Node *node34 = create_node(create_domino(3, 4), true);
  link_nodes(node, node34, BOTTOM_RIGHT);
  link_nodes(node34, create_node(create_domino(4, 5), true), BOTTOM_RIGHT);

  assert_node_print_format(node,
                           "[1:      \n"
                           ":2][3:   \n"
                           "   :4][4:\n"
                           "      :5]\n");
}

static void test_write_nodes_9(void) {
  struct Node *node = create_node(create_domino(1, 2), true);
  struct Node *node34 = create_node(create_domino(3, 4), true);
  link_nodes(node, node34, BOTTOM_RIGHT);
  link_nodes(node34, create_node(create_domino(4, 5), true), BOTTOM_RIGHT);

  struct Domino selected_domino = (struct Domino) {1, 5};

  add_moves_to_tree(node, selected_domino);

  assert_node_print_format(node,
                           "<1>[1:<2>      \n"
                           "   :2][3:      \n"
                           "      :4][4:   \n"
                           "      <4>:5]<3>\n");
}

static void test_tree_to_array0(void) {
  struct Node *root = create_node(create_domino(1, 2), true);
  link_nodes(root, create_node(create_domino(2, 3), true), BOTTOM_RIGHT);

  struct Node **arr = calloc(1, sizeof(struct Node *));
  int size = 0;
  tree_to_array(root, &arr, &size);

  struct Domino *dominoes = calloc(2, sizeof(struct Domino));
  dominoes[0] = *arr[0]->domino;
  dominoes[1] = *arr[1]->domino;

  TEST_ASSERT_EQUAL_STRING(format_dominoes_inline((struct Domino[]) {{1, 2},
                                                                     {2, 3}}, 2),
                           format_dominoes_inline(dominoes, 2));

  TEST_ASSERT_EQUAL_INT(2, size);

  TEST_ASSERT_EQUAL_INT(1, arr[0]->domino->left);
  TEST_ASSERT_EQUAL_INT(2, arr[0]->domino->right);
  TEST_ASSERT_EQUAL_INT(2, arr[1]->domino->left);
  TEST_ASSERT_EQUAL_INT(3, arr[1]->domino->right);
}


static void test_tree_to_array1(void) {
  struct Node *root = create_node(create_domino(1, 2), true);
  struct Node *node34 = create_node(create_domino(3, 4), true);
  link_nodes(root, node34, BOTTOM_RIGHT);
  link_nodes(node34, create_node(create_domino(4, 5), true), BOTTOM_RIGHT);

  struct Node **arr = calloc(1, sizeof(struct Node *));
  int size = 0;
  tree_to_array(root, &arr, &size);
}

static void test_tree_to_array2(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *second = create_node(create_domino(2, 3), true);
  link_nodes(node, second, BOTTOM_RIGHT);
  struct Node *third = create_node(create_domino(1, 3), false);
  link_nodes(second, third, BOTTOM_LEFT);
  struct Node *fourth = create_node(create_domino(3, 1), false);
  link_nodes(third, fourth, TOP_LEFT);

  struct Node **arr = calloc(1, sizeof(struct Node *));
  int size = 0;
  tree_to_array(node, &arr, &size);

  TEST_ASSERT_EQUAL_INT(4, size);
}

static void test_table_moves_display_case(struct Node *node, struct Domino *d, const char *expect) {
  add_moves_to_tree(node, *d);
  int width = 1, height = 1;
  char **matrix = init_matrix(width, height);
  write_nodes(
      matrix,
      node,
      0, 0,
      &width,
      &height);

//  printf("%s\n", matrix_to_str(matrix, width, height));

  TEST_ASSERT_EQUAL_STRING(expect, matrix_to_str(matrix, width, height));
}

static void test_table_moves_display0(void) {
  struct Node *node = create_node(create_domino(3, 4), false);
  struct Domino selected_domino = (struct Domino) {3, 4};
  test_table_moves_display_case(node, &selected_domino, "<1>[3|4]<2>\n");
}

static void test_table_moves_display1(void) {
  struct Node *node = create_node(create_domino(3, 4), false);
  struct Domino selected_domino = (struct Domino) {1, 4};
  test_table_moves_display_case(node, &selected_domino, "[3|4]<1>\n");
}

static void test_table_moves_display2(void) {
  struct Node *node = create_node(create_domino(8, 8), false);
  struct Domino selected_domino = (struct Domino) {1, 4};
  test_table_moves_display_case(node, &selected_domino, "[8|8]\n");
}

static void test_table_moves_display3(void) {
  struct Node *node = create_node(create_domino(8, 8), false);
  struct Domino selected_domino = (struct Domino) {1, 8};
  test_table_moves_display_case(node, &selected_domino, "<1>[8|8]<2>\n");
}

static void test_table_moves_display4(void) {
  struct Node *node = create_node(create_domino(4, 4), false);
  struct Domino selected_domino = (struct Domino) {4, 4};
  test_table_moves_display_case(node, &selected_domino, "<1>[4|4]<2>\n");
}

static void test_table_moves_display5(void) {
  struct Node *node = create_node(create_domino(4, 4), true);
  struct Domino selected_domino = (struct Domino) {4, 4};
  test_table_moves_display_case(node, &selected_domino, "<1>[4:<2>\n<4>:4]<3>\n");
}

static void test_table_moves_display6(void) {
  struct Node *node = create_node(create_domino(4, 1), true);
  struct Domino selected_domino = (struct Domino) {4, 4};
  test_table_moves_display_case(node, &selected_domino, "<1>[4:<2>\n   :1]   \n");
}

static void test_table_moves_display7(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  link_nodes(node, create_node(create_domino(2, 3), true), BOTTOM_RIGHT);

  struct Domino selected_domino = (struct Domino) {4, 4};
  test_table_moves_display_case(node, &selected_domino, "[1|2][2:\n     :3]\n");
}

static void test_table_moves_display8(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  link_nodes(node, create_node(create_domino(2, 3), true), BOTTOM_RIGHT);

  struct Domino selected_domino = (struct Domino) {3, 4};
  test_table_moves_display_case(node, &selected_domino, "[1|2][2:   \n  <2>:3]<1>\n");
}

static void test_table_moves_display9(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *node1 = create_node(create_domino(2, 3), true);
  link_nodes(node, node1, BOTTOM_RIGHT);
  struct Node *node2 = create_node(create_domino(3, 4), false);
  link_nodes(node1, node2, BOTTOM_RIGHT);

  struct Domino selected_domino = (struct Domino) {3, 4};
  test_table_moves_display_case(node, &selected_domino, "[1|2][2:        \n  <1>:3][3|4]<2>\n");
}

static void test_table_moves_display10(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *second = create_node(create_domino(2, 3), true);
  link_nodes(node, second, BOTTOM_RIGHT);
  struct Node *third = create_node(create_domino(1, 3), false);
  link_nodes(second, third, BOTTOM_LEFT);

  struct Domino selected_domino = (struct Domino) {1, 3};

  test_table_moves_display_case(node, &selected_domino, "<1>[1|2][2:   \n"
                                                        "<3>[1|3]:3]<2>\n");
}

static void test_table_moves_display10a(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *second = create_node(create_domino(2, 3), true);
  link_nodes(node, second, BOTTOM_RIGHT);
  struct Node *third = create_node(create_domino(1, 3), false);
  link_nodes(second, third, BOTTOM_LEFT);
  struct Node *fourth = create_node(create_domino(3, 1), false);
  link_nodes(third, fourth, TOP_LEFT);

  struct Domino selected_domino = (struct Domino) {1, 3};

  test_table_moves_display_case(node, &selected_domino, "     <1>[1|2][2:   \n"
                                                        "<3>[3|1][1|3]:3]<2>\n");
}

static void test_table_moves_display11(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *second = create_node(create_domino(2, 3), true);
  link_nodes(node, second, BOTTOM_RIGHT);
  struct Node *third = create_node(create_domino(1, 3), false);
  link_nodes(second, third, BOTTOM_RIGHT);
  link_nodes(third, create_node(create_domino(3, 9), false), BOTTOM_RIGHT);

  struct Domino selected_domino = (struct Domino) {1, 3};
  test_table_moves_display_case(node, &selected_domino, "<1>[1|2][2:          \n"
                                                        "     <2>:3][1|3][3|9]\n");
}

static void test_table_moves_display12(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *second = create_node(create_domino(2, 3), true);
  link_nodes(node, second, BOTTOM_RIGHT);
  struct Node *third = create_node(create_domino(1, 3), false);
  link_nodes(second, third, BOTTOM_RIGHT);

  struct Domino selected_domino = (struct Domino) {1, 3};
  test_table_moves_display_case(node, &selected_domino, "<1>[1|2][2:        \n"
                                                        "     <2>:3][1|3]<3>\n");
}

static void test_table_moves_display13(void) {
  struct Node *node = create_node(create_domino(2, 1), false);
  struct Node *second = create_node(create_domino(4, 2), false);
  link_nodes(node, second, TOP_LEFT);
  node = get_most_left_node(node);

  struct Domino selected_domino = (struct Domino) {1, 3};
  test_table_moves_display_case(node, &selected_domino, "[4|2][2|1]<1>\n");
}

static void test_table_moves_display14(void) {}

static void test_table_moves_display15(void) {}

static void test_table_moves_display16(void) {}

static void test_table_moves_display17(void) {}

static void test_link_nodes(char *expected, struct Node *node, int caller_line) {
  int width = 1, height = 1;
  char **m = init_matrix(width, height);

  write_nodes(m, node, 0, 0, &width, &height);

  TEST_ASSERT_EQUAL_STRING(expected, matrix_to_str(m, width, height));
  free_matrix(m, height);
}

static void test_link_nodes0(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *node2 = create_node(create_domino(3, 2), false);
  link_nodes(node, node2, BOTTOM_RIGHT);

  // Second domino has been rotated
  test_link_nodes("[1|2][2|3]\n", node, __LINE__);
}

static void test_link_nodes1(void) {
  struct Node *node = create_node(create_domino(1, 2), false);
  struct Node *node2 = create_node(create_domino(1, 3), false);
  link_nodes(node, node2, TOP_LEFT);

  // Second domino has been rotated
  test_link_nodes("[3|1][1|2]\n", node2, __LINE__);
}

/**
 * TODO's
 * 1.
[3|3][2:[2:[2:
     :6]:6]:1]
     [4:

     è stato possibile inserire il [6|4] a fianco del [2|6] sovrascrivendo un'altra tessera, quella attaccata direttamente al [3|3]

  * 2.
Tavolo:
[5:[4|5]
:2][2|2]


Tessere disponibili:
1) [1|4]
2) [3|4]

 le tessere n.1 e n.2 non dovrebbero essere disponibili.

 3.
Tavolo:
[3:
:2][2|1][1:
        :1][1:
           :2]
free(): invalid next size (fast)
make: *** [makefile:41: run] Aborted (core dumped)
 */
int main(void) {
  UnityBegin("test_domino.c");

//  RUN_TEST(test_table_moves_display0);
//  RUN_TEST(test_table_moves_display1);
//  RUN_TEST(test_table_moves_display2);
//  RUN_TEST(test_table_moves_display3);
//  RUN_TEST(test_table_moves_display4);
//  RUN_TEST(test_table_moves_display5);
//  RUN_TEST(test_table_moves_display6);
//  RUN_TEST(test_table_moves_display7);
//  RUN_TEST(test_table_moves_display8);
//  RUN_TEST(test_table_moves_display9);
//  RUN_TEST(test_table_moves_display10);
//  RUN_TEST(test_table_moves_display10a);
//  RUN_TEST(test_table_moves_display11);
//  RUN_TEST(test_table_moves_display12);
//  RUN_TEST(test_table_moves_display13);
//  RUN_TEST(test_table_moves_display14);
//  RUN_TEST(test_table_moves_display15);
//  RUN_TEST(test_table_moves_display16);
//  RUN_TEST(test_table_moves_display17);
//
//  RUN_TEST(test_tree_to_array0);
//  RUN_TEST(test_tree_to_array1);
//  RUN_TEST(test_tree_to_array2);
//
//  RUN_TEST(test_link_nodes0);
  RUN_TEST(test_link_nodes1);

//  RUN_TEST(test_write_nodes_0);
//  RUN_TEST(test_write_nodes_1);
//  RUN_TEST(test_write_nodes_2);
//  RUN_TEST(test_write_nodes_3);
//  RUN_TEST(test_write_nodes_4);
//  RUN_TEST(test_write_nodes_5);
//  RUN_TEST(test_write_nodes_6);
//  RUN_TEST(test_write_nodes_7);
//  RUN_TEST(test_write_nodes_8);
//  RUN_TEST(test_write_nodes_9);

  return UnityEnd();
}
