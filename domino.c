// 902835
// Source is available at
// https://github.com/kirpachov/c-domino-unive

#include "domino.h"

const char *LEFT_IDENTIFIERS[] = {" LEFT ", " L "};
const unsigned LEFT_IDENTIFIERS_COUNT = 2;

const char *RIGHT_IDENTIFIERS[] = {" RIGHT ", " R "};
const unsigned RIGHT_IDENTIFIERS_COUNT = 2;

const char *EXIT_COMMANDS[] = {"exit", "stop", "esci"};
const unsigned EXIT_COMMANDS_COUNT = 3;

/**
 * Are special dominoes allowed?
 * [0|0] can be placed everywhere.
 * E.g: [1|2][0|0][5|6]
 *
 * [11|11]: will add 1 to the closest domino. If it's 6, will become 1.
 * E.g:  [1|6][6|3][11|11] => [2|1][1|4][4|4]
 *
 * [12|21] will copy mirror the closest domino.
 * E.g: [1|2][2|3][12|21] => [1|2][2|3][3|2]
 */
//bool specials = true;

bool is_exit_command(char *command) {
  for (int i = 0; i < EXIT_COMMANDS_COUNT; i++)
    if (strcmp(command, EXIT_COMMANDS[i]) == 0) return true;

  return false;
}

void resize_dominoes_array(struct Domino **arr, const int new_size) {
  if (new_size <= 0) {
//    log_error("error! Tried to allocate memory with size %d. Function resize_dominoes_array(arr: %p, new_size: %d)",
//              new_size, arr, new_size);
    return;
  }

  *arr = realloc(*arr, sizeof(struct Domino) * new_size);
}

void push(const struct Domino domino, struct Domino *domino_arr, const int domino_arr_size) {
  domino_arr[domino_arr_size] = domino;
}

void unshift(struct Domino domino, struct Domino *domino_arr, const int before_size) {
  for (int i = before_size; i > 0; i--) domino_arr[i] = domino_arr[i - 1];

  domino_arr[0] = domino;
}

void pop(const int index, struct Domino *domino_arr, const int domino_arr_size) {
  for (int i = index; i < domino_arr_size - 1; i++) domino_arr[i] = domino_arr[i + 1];
}

/**
 * Smallest and biggest value domino cards can contain.
 * These values are used to populate the universe of dominoes.
 * [ 1 | 6 ]
 */


//int total_user_dominoes = 0;  // TODO --hand=X | -c=X (??)

//int total_user_dominoes_valid(void) {
//  if (total_user_dominoes > 0) return total_user_dominoes;
//
//  return TOTAL_USER_DOMINOES;
//}

/**
 * Dominoes owned by user
 */
//struct Domino *user_dominoes;
//int user_dominoes_size = 0;

//void user_dominoes_push(const struct Domino d) {
//  resize_dominoes_array(&user_dominoes, user_dominoes_size + 1);
//
//  push(d, user_dominoes, user_dominoes_size++);
//}
//
//void user_dominoes_pop(const int index) {
//  pop(index, user_dominoes, user_dominoes_size--);
//  resize_dominoes_array(&user_dominoes, user_dominoes_size);
//}

/**
 * Dominoes currently on table.
 * Variable 'table_dominoes' will contain the actual dominoes after special dominoes effect has been effected.
 * Variable 'table_dominoes_original' will contain the original dominoes, without any special dominoes effect.
 */
//struct Domino *table_dominoes;
//struct Domino *table_dominoes_original;
//int table_dominoes_size = 0;

//void table_dominoes_push(const struct Domino d, const bool left_side) {
//  resize_dominoes_array(&table_dominoes, table_dominoes_size + 1);
//  resize_dominoes_array(&table_dominoes_original, table_dominoes_size + 1);
//
//  if (left_side) {
//    unshift(d, table_dominoes, table_dominoes_size);
//    unshift(d, table_dominoes_original, table_dominoes_size);
//  } else {
//    push(d, table_dominoes, table_dominoes_size);
//    push(d, table_dominoes_original, table_dominoes_size);
//  }
//
//  table_dominoes_size++;
//}

//void table_dominoes_pop(const int index) {
//  pop(index, table_dominoes, table_dominoes_size);
//  pop(index, table_dominoes_original, table_dominoes_size);
//  table_dominoes_size--;
//
//  //  resize_dominoes_array(&table_dominoes, table_dominoes_size - 1);
//  // TODO is -1 necessary since it's already decremented?
//
//  resize_dominoes_array(&table_dominoes, table_dominoes_size);
//  resize_dominoes_array(&table_dominoes_original, table_dominoes_size);
//}

/**
 * All available dominoes
 */
//struct Domino *universe_dominoes;
//int universe_dominoes_size = 0;

//void universe_dominoes_push(const struct Domino d) {
//  push(d, universe_dominoes, universe_dominoes_size++);
//}

//char *last_command;
//char *last_command_feedback;

/**
 * Params
 */
// --challenge
bool is_challenge = false;

/**
 * Using ascii table, we can convert a char to a int by:
 * 1. Casting the char into an int => now we have the ascii index of the char
 * 2. Subtracting the index of the char '0'.
 * param is marked as 'const' because it's a good practice to mark as readonly anything you don't need to update.
 */
int char_to_int(const char ch) {
  return (int) ch - 48;
}

char int_to_char(const int i) {
  return (char) (i + 48);
}

bool is_char_a_number(const char ch) {
  return ((int) ch >= 48) && ((int) ch <= 57);
}

char *upcase_str(const char *original) {
  char *result = calloc(strlen(original), sizeof(char));
  for (unsigned i = 0; i < strlen(original); i++)
    result[i] = (char) toupper((int) original[i]);

  return result;
}

bool is_string_in_string(const char *str, const char *to_find) {
  return strstr(str, to_find) != NULL;
}

int first_number_from_string(const char *str) {
  const int str_length = (int) strlen(str);
  int last_was_integer = false;
  int result = 0;

  for (int i = 0; i < str_length + 1; i++) {
    const bool is_last_run = i == str_length;
    const char ch = is_last_run ? ' ' : str[i];
    const bool is_number = is_char_a_number(ch);

    if (is_number) {
      int integer = char_to_int(ch);
      if (last_was_integer) result = result * 10 + integer;
      else result = integer;

      last_was_integer = true;
    } else {
      if (result > 0) return result;
      last_was_integer = false;
    }
  }

  return result;
}

/**
 * is [12|21] domino?
 * @param domino struct Domino
 * @return true if provided domino is a [12|21] (or rotated [21|12])
 */
bool is_mirror_domino(const struct Domino domino) {
  return (domino.left == 12 && domino.right == 21) || (domino.left == 21 && domino.right == 12);
}

/**
 * Is [11|11] domino?
 * @param domino struct Domino
 * @return true if it's a [11|11] domino and specials are enabled
 */
bool is_addone_domino(const struct Domino domino) {
  return domino.left == 11 && domino.right == 11;
}

/**
 * Is [0|0] domino?
 * @param domino struct Domino
 * @return true if it's a [0|0] domino.
 */
bool is_glue_domino(const struct Domino domino) {
  return domino.left == 0 && domino.right == 0;
}

bool is_domino_special(const struct Domino domino) {
  return is_glue_domino(domino) || is_addone_domino(domino) || is_mirror_domino(domino);
}

/**
 * CAN PLACE ON LEFT?
 * @return true if domino can be placed on right side of the table.
 */

//bool can_place_on_left_unrotated(const struct Domino d) {
//  if (table_dominoes_size == 0 && is_mirror_domino(d)) return false;
//  if (table_dominoes_size <= 0) return true;
//  if (is_domino_special(d)) return true;
//  if (specials && table_dominoes[table_dominoes_size - 1].left == 0) return true;
//
//  return table_dominoes[table_dominoes_size - 1].left == d.right;
//}

//bool can_place_on_left_rotated(const struct Domino d) {
//  if (table_dominoes_size == 0 && is_mirror_domino(d)) return false;
//  if (table_dominoes_size <= 0) return true;
//  if (is_domino_special(d)) return true;
//  if (specials && table_dominoes[table_dominoes_size - 1].left == 0) return true;
//
//  return table_dominoes[table_dominoes_size - 1].left == d.left;
//}

//bool can_place_on_left(const struct Domino d) {
//  return can_place_on_left_unrotated(d) || can_place_on_left_rotated(d);
//}

/**
 * CAN PLACE ON RIGHT?
 * @return true if domino can be placed on right side of the table.
 */

//bool can_place_on_right_unrotated(const struct Domino d) {
//  if (table_dominoes_size == 0 && is_mirror_domino(d)) return false;
//  if (table_dominoes_size <= 0) return true;
//  if (is_domino_special(d)) return true;
//  if (specials && table_dominoes[table_dominoes_size - 1].right == 0) return true;
//
//  return table_dominoes[table_dominoes_size - 1].right == d.left;
//}
//
//bool can_place_on_right_rotated(const struct Domino d) {
//  if (table_dominoes_size == 0 && is_mirror_domino(d)) return false;
//  if (table_dominoes_size <= 0) return true;
//  if (is_domino_special(d)) return true;
//  if (specials && table_dominoes[table_dominoes_size - 1].right == 0) return true;
//
//  return table_dominoes[table_dominoes_size - 1].right == d.right;
//}

//bool can_place_on_right(const struct Domino d) {
//  return can_place_on_right_unrotated(d) || can_place_on_right_rotated(d);
//}

bool can_place_on_table(const struct Domino d) {
  log_warn("TODO can_place_on_table");
  return true;
//  return can_place_on_left(d) || can_place_on_right(d);
}

//struct Domino *valid_moves(void) {
//  struct Domino *result = malloc(sizeof(struct Domino) * valid_moves_count());
//  int result_index = 0;
//
//  for (int i = 0; i < user_dominoes_size; i++) {
//    if (can_place_on_table(user_dominoes[i])) {
//      result[result_index] = user_dominoes[i];
//      result_index++;
//    }
//  }
//
//  return result;
//}

int valid_moves_count(void) {
  log_debug("TODO valid_moves_count");
  return 9;
//  if (user_dominoes_size <= 0) return 0;
//
//  int count = 0;
//  for (int i = 0; i < user_dominoes_size; i++) {
//    if (can_place_on_table(user_dominoes[i])) count++;
//  }
//
//  return count;
}

//int valid_moves_count_side(const bool left_side, struct Domino *valid_moves) {
//  int count = 0;
//
//  for (int i = 0; i < user_dominoes_size; i++) {
//    const struct Domino domino = user_dominoes[i];
//    if ((left_side && !can_place_on_left(domino)) || (!left_side && !can_place_on_right(domino)))
//      continue;
//
//    valid_moves[count] = domino;
//    count++;
//  }
//
////  valid_moves = realloc(valid_moves, sizeof(struct Domino) * count);
//
//  return count;
//}

char *str_add_padding(const char *original) {
  char *result = calloc(strlen(original) + 2, sizeof(char));
  result[0] = ' ';
  result[strlen(original) + 1] = ' ';

  for (unsigned i = 0; i < strlen(original); i++) result[i + 1] = original[i];

  return result;
}

char *format_domino(const struct Domino d) {
  char *str = calloc(100, sizeof(char));
  if (str == NULL) {
    log_error("Failed to allocate memory for 'format_domino'");
    exit(EXIT_FAILURE);
  }

  sprintf(str, "[%d|%d]", d.left, d.right);

  str = realloc(str, strlen(str));

  if (str == NULL) {
    log_debug("Failed to re-allocate memory for 'format_domino'");
    exit(EXIT_FAILURE);
  }

  return str;
}

char *side_str(const bool is_left_side) {
  return is_left_side ? "left" : "right";
}

int random_between(const int lower, const int upper) {
  return (rand() % (upper - lower + 1)) + lower;
}

struct Domino random_domino_from(struct Domino *arr, const int size) {
  return arr[random_between(0, size - 1)];
}

int run_challenge_1(void) {
  log_debug("***** RUNNING CHALLENGE 1 *****");
  int dominoes_size;
  log_debug("scanf returned %d", scanf("%d", &dominoes_size));
  log_debug("We'll have %d dominoes.", dominoes_size);
  struct Domino *dominoes = calloc(dominoes_size, sizeof(struct Domino));
  for (int i = 0; i < dominoes_size; i++) {
    int left;
    int right;

    log_debug("scanf returned %d", scanf("%d", &left));
    log_debug("scanf returned %d", scanf("%d", &right));

    const struct Domino domino = (struct Domino) {left, right};
    char *formatted_domino = format_domino(domino);
    log_debug("Adding %s to list of dominoes", formatted_domino);
    free(formatted_domino);

    dominoes[i] = domino;
  }

  log_debug("Completed the acquisition of the dominoes. Got: %s", format_dominoes_inline(dominoes, dominoes_size));

//  printf("%s\n", process_challenge_1(dominoes, dominoes_size));

  return 0;
}

int run_challenge(void) {
  log_info("Running in challenge mode.");

  return run_challenge_1();
}

//void populate_universe_dominoes(struct Domino **universe, int *universe_size) {
//  log_debug("populate_universe_dominoes universe: %p, size: %d", universe, *universe_size);
//
//  *universe_size = 0;
//
//// TODO calc this value without any for loop.
//  for (int i = SMALLES_DOMINO_VALUE; i <= LARGEST_DOMINO_VALUE; i++)
//    for (int j = i; j <= LARGEST_DOMINO_VALUE; j++)
//      (*universe_size)++;
//
//  if (specials) (*universe_size) += 3; // [0|0], [11|11], [12|21]
//
//  resize_dominoes_array(universe, *universe_size);
//
//  for (int i = SMALLES_DOMINO_VALUE; i <= LARGEST_DOMINO_VALUE; i++) {
//    for (int j = i; j <= LARGEST_DOMINO_VALUE; j++) {
//      const struct Domino d = {i, j};
////      char *formatted_domino = format_domino(d);
////      log_debug("Adding %s to universe.", formatted_domino);
////      free(formatted_domino);
//
//      push(d, *universe, *universe_size);
//    }
//  }
//
//  if (specials) {
//    push((struct Domino) {0, 0}, *universe, *universe_size);
//    push((struct Domino) {11, 11}, *universe, *universe_size);
//    push((struct Domino) {12, 21}, *universe, *universe_size);
////    (*universe_size) = (*universe_size) + 3;
//  }
//
//  log_debug("Universe:");
//  for(int i = 0; i < *universe_size; i++){
//    log_debug("%s", format_domino((*universe)[i]));
//  }
//
//  log_debug("universe_dominoes_size is %d", *universe_size);
//}

char *format_dominoes_inline(const struct Domino *dominoes, const int size) {
  // TODO calc max size instead of using 100
  const int a = (size + 1) * strlen(" [x|y] ");
  char *result = calloc(a, sizeof(char));

  for (int i = 0; i < size; i++) {
    char *formatted = format_domino(dominoes[i]);
    strcat(result, formatted);
//    printf("domino %s\n", formatted);
    free(formatted);

    if (i < size - 1) strcat(result, " ");
  }

//  printf("format_dominoes_inline: %s\n", result);

  return result;
}

void print_table(struct Node *root_node) {
  printf("Tavolo:\n");
  if (root_node == NULL) {
    printf("<Nessuna tessera>\n\n");
    return;
  }

  print_tree(root_node);

  printf("\n\n");
}

/**
 * This function will generate a multi-line string with all the dominoes you pass it,
 * and will add information about how to select that domino.
 * In particular, will add the index if it can be selected,
 * and "R" or "L" to index the side where you can place it.
 * @param dominoes
 * @param arr_size
 * @return
 */
char *format_dominoes_with_valid_moves(const struct Domino *dominoes, const int arr_size) {
  const int max_row_size = 20;
  char *result = calloc(max_row_size * arr_size, sizeof(char));
  char *accumulator = calloc(max_row_size, sizeof(char));

  for (int i = 0; i < arr_size; i++) {
    const struct Domino d = dominoes[i];

    log_warn("TODO can place anywhere domino? %d", __LINE__);
//    const bool left = can_place_on_left(d);
//    const bool right = can_place_on_right(d);
    const bool can_place = true;
    char *formatted_domino = format_domino(d);
    strcat(result, formatted_domino);
    free(formatted_domino);

    if (can_place) {
      sprintf(accumulator, "%d", i + 1);
      strcat(result, accumulator);
    }

//    if (can_place) {
//      strcat(result, "  ");
//
//      if (left) {
//        sprintf(accumulator, "(%dL)", i + 1);
//        strcat(result, accumulator);
//      }
//      if (left && right) {
//        strcat(result, "  /  ");
//      }
//      if (right) {
//        sprintf(accumulator, "(%dR)", i + 1);
//        strcat(result, accumulator);
//      }
//    }

    strcat(result, "\n");
  }

  return result;
}

//char *format_user_dominoes() {
//  return format_dominoes_with_valid_moves(user_dominoes, user_dominoes_size);
//}

//void print_user_dominoes(void) {
//  printf("Your dominoes:\n");
//  printf("%s", format_user_dominoes());
//}

bool is_help_command(const char *command) {
  if (strcmp(command, "help") == 0) return true;
  if (strcmp(command, "?") == 0) return true;

  return false;
}

/**
 * Will calc points from an array of dominoes.
 * Note:
 * - if the array is empty, will return 0.
 * - if the only card in the array is special, will return 0. This is because special cards should be used only with other cards.
 *
 * @param arr pointer to array of dominoes
 * @param arr_size quantity of dominoes in the array
 * @return integer with the sum of all points.
 */
int calc_points_from(const struct Domino *arr, const int arr_size) {
  if (arr_size <= 0) return 0;
  if (arr_size == 1 && is_domino_special(arr[0])) return 0;

  int acc = 0;
  for (int i = 0; i < arr_size; i++) acc += arr[i].left + arr[i].right;

  return acc;
}

//int calc_user_points(void) {
//  return calc_points_from(table_dominoes, table_dominoes_size);
//}

char *format_user_points_str(void) {
  char *result = calloc(strlen("Your points: 999999"), sizeof(char));
  sprintf(result, "Your points: %d", 123);
  return result;
}

void print_user_points(void) {
  printf("%s\n", format_user_points_str());
}

void print_last_command_feedback(void) {
//  if (strlen(last_command) == 0 || is_help_command(last_command)) {
//    printf("Welcome!\n"
//           "Select the domino to place by specifying the domino's index and the side (Right/Left) where to place it.\n"
//           "Example: \"1L\" will place the first domino on the table, to the left of the other dominos\n");
//    return;
//  }
//
//  printf("%s\n", last_command_feedback);
}

void clean_screen(void) {
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
#ifdef _WIN64
  system("cls");
#elif __linux
  const int result = system("clear");
  log_debug("system(\"clear\") returned %d", result);
#else
  log_error("WARNING: CANNOT CLEAR SCREEN.");
#endif
}

bool node_in_array(struct Node **arr, const int size, struct Node *node) {
  for (int i = 0; i < size; i++) {
    if (arr[i]->id == node->id) return true;
  }

  return false;
}

void realloc_array_one_node(struct Node **arr, const int size) {
  *arr = realloc(*arr, sizeof(struct Node) * (size <= 0 ? 1 : size));
}

static void tree_to_array_rec(struct Node *node, struct Node ***result, int *result_size) {
  if (node_in_array(*result, *result_size, node)) {
    log_debug("tree_to_array_rec #%lul already in array", node->id);
    return;
  }

  log_debug("tree_to_array_rec #%lu | %s | result_size: %d", node->id, format_domino(*(node->domino)), *result_size);

  if (*result_size > 30) exit(1);

  *result = realloc(*result, sizeof(struct Node) * (*result_size + 1));
  (*result)[(*result_size)] = node;
  (*result_size)++;

  if (node->bottom_right) tree_to_array_rec(node->bottom_right, result, result_size);

  if (node->bottom_left) tree_to_array_rec(node->bottom_left, result, result_size);

  if (node->top_right) tree_to_array_rec(node->top_right, result, result_size);

  if (node->top_left) tree_to_array_rec(node->top_left, result, result_size);
}

void tree_to_array(struct Node *root_node, struct Node ***result, int *result_size) {
  if (root_node == NULL) return;

  tree_to_array_rec(root_node, result, result_size);
}

//void free_nodes(struct Node *root_node, struct Node *result, const int result_size) {
//
//}

bool can_place_domino_on_node(const struct Domino domino, const struct Node *node) {
  if (node == NULL) return false;
  if (node->domino == NULL) return false;

  const struct Domino node_domino = *(node->domino);
  return node_domino.left == domino.left || node_domino.left == domino.right || node_domino.right == domino.left ||
         node_domino.right == domino.right;
}

bool can_place_domino_in_tree(const struct Domino domino, struct Node *root_node) {
  if (root_node == NULL) return true;
  if (root_node->domino == NULL) {
    printf("ERROR: domino is null at row %d", __LINE__);
    exit(EXIT_FAILURE);
  }

  struct Node **arr = calloc(1, sizeof(struct Node *));
  int size = 0;
  tree_to_array(root_node, &arr, &size);

  for (int i = 0; i < size; i++) {
    if (can_place_domino_on_node(domino, arr[i])) return true;
  }

  return false;
}

void print_dominoes_with_valid_moves(
    struct Node *root_node,
    const struct Domino *dominoes,
    const int dominoes_size
) {
  for (int i = 0; i < dominoes_size; i++) {
    const struct Domino d = dominoes[i];
    if (can_place_domino_in_tree(d, root_node))
      printf("%d) ", i + 1);
    else printf("-) ");

    printf("%s", format_domino(d));

    printf("\n");
  }
}

void add_moves_to_tree(struct Node *root, struct Domino d) {
  if (root == NULL) return;

  struct Node **arr = calloc(1, sizeof(struct Node *));
  int size = 0;
  tree_to_array(root, &arr, &size);
  int move = 1;

//  log_debug("add_moves_to_tree %d", size);

  for (int i = 0; i < size; i++) {
    struct Node *node = arr[i];
    if (node->domino == NULL) {
      printf("ERROR: domino is null at row %d", __LINE__);
      exit(EXIT_FAILURE);
    }

    const bool top_match = node->domino->left == d.left || node->domino->left == d.right;
    const bool bottom_match = node->domino->right == d.left || node->domino->right == d.right;

//    log_debug("add_moves_to_tree. Domino %s. %s %s", format_domino(*(node->domino)), top_match ? "top_match" : "", bottom_match ? "bottom_match" : "");

    if (node->top_left == NULL && top_match) node->top_left_move = move++;
    if (node->is_vert && node->top_right == NULL && top_match) node->top_right_move = move++;

    if (node->bottom_right == NULL && bottom_match) node->bottom_right_move = move++;

    if (node->is_vert && node->bottom_left == NULL && bottom_match) node->bottom_left_move = move++;
  }

  // TODO free arr
}

void remove_moves_from_tree(struct Node *root) {
  if (root == NULL) return;

  struct Node **arr = calloc(1, sizeof(struct Node *));
  int size = 0;
  tree_to_array(root, &arr, &size);

  for (int i = 0; i < size; i++) {
    struct Node *node = arr[i];
    node->top_left_move = 0;
    node->top_right_move = 0;
    node->bottom_left_move = 0;
    node->bottom_right_move = 0;
  }

  // TODO free arr
}

void print_everything(
    struct Node *root_node,
    struct Domino *user_dominoes,
    const int user_dominoes_size,
    int status,
    struct Domino *selected_domino
) {
  clean_screen();
  if (status == STATUS_SELECT_DOMINO) {
    print_table(root_node);
    printf("Tessere disponibili:\n");
    print_dominoes_with_valid_moves(root_node, user_dominoes, user_dominoes_size);
    printf("?> Seleziona il domino da posizionare utilizzando l'indice indicato sulla sinistra.\n");
    printf("\n");
    return;
  }

  if (status == STATUS_SELECT_POSITION) {
    if (selected_domino == NULL) {
      printf("ERROR: selected_domino is null at line %d", __LINE__);
      exit(EXIT_FAILURE);
    }

    add_moves_to_tree(root_node, *selected_domino);
    print_table(root_node);
    printf("?> Seleziona la posizione in cui mettere il domino indicando l'<indice>.\n");
    printf("?> Tessera selezionata: %s\n", format_domino(*selected_domino));
    printf("\n");
    return;
  }

  printf("INVALID STATUS %d AT LINE %d\n", status, __LINE__);
  exit(EXIT_FAILURE);
}

//void assign_user_random_dominoes(void) {
//  while (user_dominoes_size < total_user_dominoes_valid()) {
//    const struct Domino d = random_domino_from_universe();
//    char *formatted_domino = format_domino(d);
//    log_debug("Adding %s to user's hand", formatted_domino);
//    free(formatted_domino);
//    user_dominoes_push(d);
//  }
//}

/**
 * This function will try to decode user's will.
 * What card is user trying to select? (if any)
 * @param command: string to evaluate.
 * @param result_index: index of the chosen domino. 0 is returned if nothing is found.
 * @param position
 * is 0 if not found;
 * is 1 if left;
 * is 2 if right.
 */
void guess_selection(const char *command, int *result_index, int *position) {
  *result_index = first_number_from_string(command);

  if (*result_index < 0 || *result_index > (int) strlen(command) * 10) *result_index = 0;

  const char *formatted_command = str_add_padding(upcase_str(command));

  if (strlen(command) == 2 && is_string_in_string(formatted_command, "L")) {
    *position = LEFT_SIDE;
    return;
  }

  if (strlen(command) == 2 && is_string_in_string(formatted_command, "R")) {
    *position = RIGHT_SIDE;
    return;
  }

  *position = 0;

  for (unsigned i = 0; i < LEFT_IDENTIFIERS_COUNT; i++) {
    if (is_string_in_string(formatted_command, LEFT_IDENTIFIERS[i])) {
      *position = LEFT_SIDE;
      return;
    }
  }

  for (unsigned i = 0; i < RIGHT_IDENTIFIERS_COUNT; i++) {
    if (is_string_in_string(formatted_command, RIGHT_IDENTIFIERS[i])) {
      *position = RIGHT_SIDE;
      return;
    }
  }
}

//bool can_be_put_on_table(const struct Domino domino, const bool left_side) {
//  return left_side ? can_place_on_left(domino) : can_place_on_right(domino);
//}

//bool needs_to_be_rotated_before_putting_on_table(const struct Domino selected_domino, const bool is_left_side) {
//  if (table_dominoes_size <= 0) return false;
//
//  const struct Domino table_domino = is_left_side ? table_dominoes[0] : table_dominoes[table_dominoes_size - 1];
//
//  if (is_left_side) return table_domino.left != selected_domino.right;
//
//  return table_domino.right != selected_domino.left;
//}

struct Domino rotate_domino(const struct Domino d) {
  return (struct Domino) {d.right, d.left};
}

//struct Domino rotate_if_necessary(const struct Domino domino, const bool is_left_side) {
//  return needs_to_be_rotated_before_putting_on_table(domino, is_left_side) ? rotate_domino(domino) : domino;
//}

/**
 * @param index starting from 0
 * @param left_side boolean value
 * @return 0 if want to interrupt `last_command` processing.
 */
int put_on_table(const int user_dominoes_index, const bool left_side) {
//  const struct Domino selected = user_dominoes[user_dominoes_index];
//  const char *side_str = left_side ? "left" : "right";
//  char *selected_str = format_domino(selected);
//  // TODO calc size instead of 100
//  last_command_feedback = calloc(100, sizeof(char));
//
////  log_debug("put_on_table | user_dominoes_index: %d | side: %s | domino: %s", user_dominoes_index, side_str, selected_str);
//
//  if (!(can_be_put_on_table(selected, left_side))) {
//    sprintf(last_command_feedback, "Domino %s cannot be put on %s side", selected_str, side_str);
//    return 0;
//  }
//
//  table_dominoes_push(rotate_if_necessary(selected, left_side), left_side);
//
//  user_dominoes_pop(user_dominoes_index);
//
//  sprintf(last_command_feedback, "Domino %s has been put on %s side", selected_str, side_str);
//
//  apply_special_dominoes_effect();
//
//  free(selected_str);
//
//  return 0;
}

void process_last_command(void) {
//  log_debug("processing command \"%s\"", last_command);
//  if (is_help_command(last_command)) return;
//
//  int selected_domino_index = 0; // 0 is invalid as index
//  int selected_domino_side = 0; // 0 is invalid as position
//  guess_selection(last_command, &selected_domino_index, &selected_domino_side);
//
//  log_debug("guess_selection returned | index %d | selected_domino_side %d", selected_domino_index,
//            selected_domino_side);
//
//  if (selected_domino_index == 0 || selected_domino_side == 0 || (selected_domino_index - 1) > user_dominoes_size) {
//    log_info("user selected invalid index (%d) or position (%d) (command is: \"%s\")", selected_domino_index,
//             selected_domino_side, last_command);
//
//    last_command_feedback = "Invalid command.";
//    return;
//  }
//
//  selected_domino_index--;
//
//  if (put_on_table(selected_domino_index, selected_domino_side == LEFT_SIDE) == 0) return;
//
//  /**
//   * TODO:
//   * - if user has invalid position or index, populate "last_command_feedback" with the error messages.
//   * otherwise, select domino and try to put it on the table.
//   * if it's a valid move, populate last_command_feedback with something like "Domino [x|y] has been put on <left/right> side"
//   * otherwise, if it's not a valid move, let user know that it's not a valid move.
//   *
//   */
//
//  last_command_feedback = "Invalid command.";
}

void acquire_command(char **last_command) {
  char tmp_command[MAX_COMMAND_SIZE];
  int N = 0;
  char ch;

  printf("$ ");
  ch = getchar();

  while (ch != '\n' && N < MAX_COMMAND_SIZE) {
    tmp_command[N++] = ch;
    ch = getchar();
  }

  free(*last_command);
  *last_command = calloc(N, sizeof(char));

  if (*last_command == NULL) exit(EXIT_FAILURE);

  for (int i = 0; i < N; i++) (*last_command)[i] = tmp_command[i];
}

/**
 * Apply effect of [11|11].
 * Add 1 to all the values of all dominoes on the table. If 6 + 1, set 1.
 */
//void apply_special_sumall(const int sumall_index) {
//  for (int j = 0; j < table_dominoes_size; j++) {
//    if (j == sumall_index) continue;
//
//    table_dominoes[j].left++;
//    table_dominoes[j].right++;
//
//    if (table_dominoes[j].left > 6) table_dominoes[j].left = 1;
//    if (table_dominoes[j].right > 6) table_dominoes[j].right = 1;
//  }
//
//  if (sumall_index == 0) {
//    table_dominoes[sumall_index].left = table_dominoes[1].left;
//    table_dominoes[sumall_index].right = table_dominoes[1].left;
//    return;
//  }
//
//  if (sumall_index == table_dominoes_size - 1) {
//    table_dominoes[sumall_index].left = table_dominoes[table_dominoes_size - 2].right;
//    table_dominoes[sumall_index].right = table_dominoes[table_dominoes_size - 2].right;
//    return;
//  }
//
//  printf("invalid sumall_index %d at row %d\n", sumall_index, __LINE__);
//  log_error("apply_special_sumall: sumall_index is %d, but it should be 0 or %d", sumall_index,
//            table_dominoes_size - 1);
//  exit(EXIT_FAILURE);
//}

//void apply_special_mirror(const int mirror_index) {
//  if (mirror_index == 0) {
//    table_dominoes[mirror_index].right = table_dominoes[1].left;
//    table_dominoes[mirror_index].left = table_dominoes[1].right;
//    return;
//  }
//
//  if (mirror_index == table_dominoes_size - 1) {
//    const struct Domino last = table_dominoes[table_dominoes_size - 2];
//    table_dominoes[mirror_index].right = last.left;
//    table_dominoes[mirror_index].left = last.right;
//    return;
//  }
//
//  log_error("apply_special_mirror: mirror_index is %d, but it should be 0 or %d", mirror_index,
//            table_dominoes_size - 1);
//  exit(EXIT_FAILURE);
//}

//void apply_special_dominoes_effect(void) {
//  if (!specials) return;
//  if (table_dominoes_size <= 1) return;
//
//  for (int i = 0; i < table_dominoes_size; i++) {
//    const struct Domino domino = table_dominoes[i];
//    if (!is_domino_special(domino)) continue;
//    if (domino.left == 0 && domino.right == 0) continue;
//
//    if (domino.left == 11 && domino.right == 11) {
//      apply_special_sumall(i);
//      continue;
//    }
//
//    if ((domino.left == 12 && domino.right == 21) || (domino.right == 12 && domino.left == 21)) {
//      apply_special_mirror(i);
//      continue;
//    }
//
//    // Should never see this message
//    printf("TODO manage special domino of type %s\n", format_domino(domino));
//  }
//}

struct Domino *dominoes_without_element(
    const struct Domino *dominoes,
    const int size,
    const int index_of_element_to_remove) {

  struct Domino *dominoes_without_domino = calloc(size, sizeof(struct Domino));
  int dominoes_without_domino_size = size;
  for (int j = 0; j < size; j++) {
    dominoes_without_domino[j] = dominoes[j];
  }

  pop(index_of_element_to_remove, dominoes_without_domino, dominoes_without_domino_size--);
  return dominoes_without_domino;
}

bool dominoes_equal(const struct Domino a, const struct Domino b) {
  return (a.left == b.left && a.right == b.right) || (a.right == b.left && a.left == b.right);
}

/**
 * Function scenario_with will return the max points you can make from a given point.
 * You'll pass the actual situation to this function, such as:
 * - Dominoes user has
 * - Dominoes that are on the table
 * - ...
 * And you'll get:
 * - Best points you can make from that combination
 * - Array of dominoes showing you how to place the dominoes user has to make the maximum amount of points.
 * @param user_arr the dominoes user has currently.
 * @param user_arr_size how many dominoes has the user currently.
 * @param table_arr the dominoes that are already on the table.
 * @param table_arr_size how many dominoes are already on the table
 * @param table_arr_original actual dominoes, without any special domino effect.
 * @param best_table_possible where the best solution will be stored.
 * @param best_table_possible_size size of the best solution array.
 * @return best points you can make from that combination.
 */
//int scenario_with(
//    const struct Domino *user_arr,
//    const int user_arr_size,
//
//    const struct Domino *table_arr,
//    const int table_arr_size,
//
//    const struct Domino *table_arr_original,
//
//    struct Domino **best_table_possible,
//    int *best_table_possible_size,
//    struct Domino **best_table_possible_original
//) {
//  set_table_dominoes(table_arr, table_arr_size);
//  set_table_dominoes_original(table_arr_original, table_arr_size);
//
//  set_user_dominoes(user_arr, user_arr_size);
//
//  /* DEBUG */
////  struct Domino *valid_moves = calloc(user_dominoes_size, sizeof(struct Domino));
////  int valid_moves_count = valid_moves_count_side(false, valid_moves);
////  log_debug("SCENARIO_WITH | user_dominoes: %s | table_dominoes: %s | valid_moves: %s",
////            format_dominoes_for_table(user_dominoes, user_dominoes_size),
////            format_dominoes_for_table(table_dominoes, table_dominoes_size),
////            format_dominoes_for_table(valid_moves, valid_moves_count)
////  );
//  /* /DEBUG */
//
//  if (calc_points_from(*best_table_possible, *best_table_possible_size) < calc_user_points()) {
//    *best_table_possible = malloc(sizeof(struct Domino) * table_arr_size);
//    *best_table_possible_original = malloc(sizeof(struct Domino) * table_arr_size);
//    *best_table_possible_size = table_arr_size;
//
//    memcpy(*best_table_possible, table_arr, sizeof(struct Domino) * table_arr_size);
//    memcpy(*best_table_possible_original, table_arr_original, sizeof(struct Domino) * table_arr_size);
//  }
//
////  if (valid_moves_count == 0) {
////    log_debug("No more valid moves. | points: %d | Table: %s ", calc_user_points(),
////              format_dominoes_for_table(table_dominoes, table_dominoes_size)
////    );
////
////    return calc_user_points();
////  }
//
//  int best_until_now = calc_user_points();
//
//  for (int i = 0; i < user_arr_size; i++) {
//    const struct Domino domino = user_arr[i];
//
//    if (!(can_place_on_right(domino))) {
//      /* DEBUG */
////      log_debug("Cannot place on right %s", format_domino(domino));
//      /* /DEBUG */
//      continue;
//    }
//
//    /* DEBUG */
////    log_debug("for has now domino = %s | Table is %s | user has %s", format_domino(domino),
////              format_dominoes_for_table(table_dominoes, table_dominoes_size),
////              format_dominoes_for_table(user_dominoes, user_dominoes_size));
//    /* /DEBUG */
//
//    int first_valid_move_index_in_user_arr = -1;
//    for (int j = 0; j < user_dominoes_size; j++) {
//      if (!dominoes_equal(user_dominoes[j], domino)) continue;
//
//      first_valid_move_index_in_user_arr = j;
//      break;
//    }
//
//    if (first_valid_move_index_in_user_arr == -1) {
//      log_error("I was unable to find domino %s in user dominoes (%s)", format_domino(domino),
//                format_dominoes_for_table(user_dominoes, user_dominoes_size));
//
//      exit(EXIT_FAILURE);
//    }
//
//    bool was_put_unrotated = false;
//    struct Domino *table_dominoes_before_special_effects;
//
//    if (can_place_on_right_unrotated(domino)) {
//      /* DEBUG */
////      log_debug("Putting domino %s NOT rotated on table %s",
////                format_domino(domino),
////                format_dominoes_for_table(table_dominoes, table_dominoes_size)
////      );
//      /* /DEBUG */
//
//
//      was_put_unrotated = true;
//      table_dominoes_push(domino, false);
//      user_dominoes_pop(first_valid_move_index_in_user_arr);
//      table_dominoes_before_special_effects = calloc(table_dominoes_size, sizeof(struct Domino));
//      memcpy(table_dominoes_before_special_effects, table_dominoes, sizeof(struct Domino) * table_dominoes_size);
//      apply_special_dominoes_effect();
//
//      const int current = scenario_with(
//          user_dominoes,
//          user_dominoes_size,
//          table_dominoes,
//          table_dominoes_size,
//          table_dominoes_original,
//          best_table_possible,
//          best_table_possible_size,
//          best_table_possible_original
//      );
//
//      if (best_until_now < current) {
//        best_until_now = current;
//      }
//
//      set_table_dominoes(table_dominoes_before_special_effects, table_dominoes_size);
////      free(table_dominoes_before_special_effects);
//      table_dominoes_pop(table_dominoes_size - 1);
//    }
//
//    if (domino.left != domino.right && can_place_on_right_rotated(domino)) {
//      /* DEBUG */
////      log_debug("Putting domino %s rotated on table %s",
////                format_domino(rotate_domino(domino)),
////                format_dominoes_for_table(table_dominoes, table_dominoes_size)
////      );
//      /* /DEBUG */
//
//      table_dominoes_push(rotate_domino(domino), false);
//      if (!was_put_unrotated) { user_dominoes_pop(first_valid_move_index_in_user_arr); }
//      table_dominoes_before_special_effects = calloc(table_dominoes_size, sizeof(struct Domino));
//      memcpy(table_dominoes_before_special_effects, table_dominoes, sizeof(struct Domino) * table_dominoes_size);
//      apply_special_dominoes_effect();
//
//      const int current = scenario_with(
//          user_dominoes,
//          user_dominoes_size,
//          table_dominoes,
//          table_dominoes_size,
//          table_dominoes_original,
//          best_table_possible,
//          best_table_possible_size,
//          best_table_possible_original
//      );
//
//      if (best_until_now < current) {
//        best_until_now = current;
//      }
//
//      set_table_dominoes(table_dominoes_before_special_effects, table_dominoes_size);
//    }
//
//    set_table_dominoes(table_arr, table_arr_size);
//    set_table_dominoes_original(table_arr_original, table_arr_size);
//    set_user_dominoes(user_arr, user_arr_size);
//    /* DEBUG */
////    log_debug("Ending block where domino was %s | Table domino is %s | User domino is %s",
////              format_domino(domino), format_dominoes_for_table(table_dominoes, table_dominoes_size),
////              format_dominoes_for_table(user_dominoes, user_dominoes_size));
//    /* /DEBUG */
//  }
//
//  return best_until_now;
//}

//int best_scenario(
//    const struct Domino *dominoes,
//    const int dominoes_size,
//    struct Domino **best_table_possible,
//    int *best_table_possible_size,
//    struct Domino **best_table_possible_original
//) {
//  return scenario_with(
//      dominoes,
//      dominoes_size,
//      (struct Domino[]) {{0, 0}},
//      0,
//      (struct Domino[]) {{0, 0}},
//      best_table_possible,
//      best_table_possible_size,
//      best_table_possible_original
//  );
//}

/**
 * Formats domino as commands to execute.
 * Example: [2|3] [3|5] [5|1] [1|5] => "S 2 3 R 3 5 R 5 1 R 1 5"
 * @param dominoes array of dominoes to format
 * @param dominoes_size size of the array to format
 * @return char array - formatted commands
 */
char *format_dominoes_as_commands(const struct Domino *dominoes, const int dominoes_size) {
  char *str = calloc((strlen("S DD GG ") * dominoes_size), sizeof(char));

  for (int i = 0; i < dominoes_size; i++) {
    char *tmp = calloc(20, sizeof(char));
    sprintf(tmp, "%c %d %d", i == 0 ? 'S' : 'R', dominoes[i].left, dominoes[i].right);
    strcat(str, tmp);

    free(tmp);

    if (i + 1 < dominoes_size) strcat(str, " ");
  }

  return str;
}

/**
 * Given an array of dominoes, first challenge is about returning the commands to execute in order to have the best
 * points possible.
 * @param dominoes array of dominoes - dominoes user can play
 * @param dominoes_size length of array of dominoes
 * @return formatted string with the commands to execute to get the maximum amount of points with the given dominoes.
 */
//char *process_challenge_1(const struct Domino *dominoes, const int dominoes_size) {
//  // TODO calc instead of static 100
//  struct Domino *best_table = calloc(100, sizeof(struct Domino));
//  struct Domino *best_table_original = calloc(100, sizeof(struct Domino));
//  if (best_table == NULL) {
//    log_error("Failed to allocate memory for best_table at line %d", __LINE__);
//    exit(EXIT_FAILURE);
//  }
//
//  int best_table_size = 1;
//  int best_points = best_scenario(dominoes, dominoes_size, &best_table, &best_table_size, &best_table_original);
//
//  char *commands = format_dominoes_as_commands(best_table_original, best_table_size);
////  best_table = realloc(best_table, sizeof(struct Domino) * best_table_size);
//  if (best_table == NULL) {
//    log_error("Failed to reallocate memory for best_table at line %d", __LINE__);
//    exit(EXIT_FAILURE);
//  }
//
//  log_debug("process_challenge_1: universe: %s | best table is %s | max points: %d | commands: %s",
//            format_dominoes_for_table(dominoes, dominoes_size),
//            format_dominoes_for_table(best_table, best_table_size),
//            best_points,
//            commands
//  );
//
//  return commands;
//}

void run_terminal(void) {
//  while (user_dominoes_size > 0 && valid_moves_count() > 0) {
////    acquire_command();
//    process_last_command();
////    print_everything();
//  }
//
//  printf("Game over!\n");
//  printf("You made %d points!\n", calc_user_points());
}

struct Domino *random_special_domino(void) {
  const int index = rand() % 3;
  if (index == 0) return create_domino(11, 11);
  if (index == 1) return create_domino(12, 21);

  return create_domino(0, 0);
}

struct Domino *random_domino(bool specials) {
  if (specials && (rand() % 10 == 0)) {
    return random_special_domino();
  }

  return create_domino(rand() % LARGEST_DOMINO_VALUE + 1, rand() % LARGEST_DOMINO_VALUE + 1);
}

struct Domino *init_random_dominoes_arr(int size, bool specials) {
  struct Domino *arr = calloc(size, sizeof(struct Domino));
  for (int i = 0; i < size; i++) {
    arr[i] = *random_domino(specials);
  }

  return arr;
}

char *status_str(const int status) {
  switch (status) {
    case STATUS_SELECT_DOMINO:
      return "STATUS_SELECT_DOMINO";
    case STATUS_SELECT_POSITION:
      return "STATUS_SELECT_POSITION";
    case STATUS_SELECT_ORIENTATION:
      return "STATUS_SELECT_ORIENTATION";
    default:
      printf("INVALID STATUS %d at line %d\n", status, __LINE__);
      exit(EXIT_FAILURE);
  }
}

struct Domino *
selected_domino_from_command(const char *command, struct Domino *dominoes, const int dominoes_size) {
  const int index = atoi(command);
  if (index < 1 || index > dominoes_size) return NULL;

  struct Domino *domino = &(dominoes[index - 1]);
  return domino;
}

//struct CommandFeedback {
//  int status;
//  char *feedback;
//
//};

int command_select_domino(
    const char *command,
    struct Node *root,
    struct Domino *users,
    const int users_size,
    char **feedback,
    struct Domino **selected_domino
) {
  struct Domino *domino = selected_domino_from_command(command, users, users_size);
  if (domino == NULL) {
    log_debug("command_select_domino | domino is NULL");
    *feedback = "Nessun domino con quell'indice.";
    return STATUS_SELECT_DOMINO;
  }

  if (!can_place_domino_in_tree(*domino, root)) {
    log_debug("command_select_domino | domino cannot be placed in tree");
    sprintf(*feedback, "Cannot place domino %s in tree", format_domino(*domino));
    return STATUS_SELECT_DOMINO;
  }

  log_debug("command_select_domino | domino %s selected", format_domino(*domino));
  sprintf(*feedback, "Selected domino %s", format_domino(*domino));

  *selected_domino = domino;

  return STATUS_SELECT_POSITION;
}

//void process_command(
//    const char *command,
//    struct Node *root,
//    struct Domino *users,
//    const int users_size,
//    int *status,
//    char **feedback
//) {
//  log_debug("process_command | command: %s | status: %s | users: %s", command, status_str(*status),
//            format_dominoes_inline(users, users_size));
//
//  if (*status == STATUS_SELECT_DOMINO) {
//    *status = command_select_domino(command, root, users, users_size, feedback);
//    return;
//  }
//
//  printf("process_command has INVALID STATUS %d\n", *status);
//  exit(EXIT_FAILURE);
//}

// \sas
int run_interactive(void) {
  log_info("Running in interactive mode.");
  const bool specials_enabled = true;
  const int users_size = 10;
  struct Domino *users = init_random_dominoes_arr(users_size, specials_enabled);
  log_debug("User's dominoes: %s", format_dominoes_inline(users, users_size));

  char *command = calloc(1, sizeof(char));

  struct Node *root = NULL;

  root = create_node(create_domino(1, 2), false);

//  INTRO
//  printf("%s\n", WELCOME_MESSAGE);
//  acquire_command(&command);

//  int width = 1, height = 1;

//  char **matrix = init_matrix(width, height);
  int status = STATUS_SELECT_DOMINO;

  char *feedback = calloc(100, sizeof(char));
  struct Domino *selected_domino = NULL;

  while (valid_moves_count() > 0 && !is_exit_command(command)) {
    print_everything(root, users, users_size, status, selected_domino);
    acquire_command(&command);
    if (status == STATUS_SELECT_DOMINO) {
      status = command_select_domino(command, root, users, users_size, &feedback, &selected_domino);
      continue;
    }

    printf("INVALID STATUS AT LINE %d: %d", __LINE__, status);
    exit(EXIT_FAILURE);

//    process_command(command, root, users, users_size, &status, &feedback);
//    process_command(command);
//    printf("last command was '%s'\n", command);
  }

  printf("Ciao ciao!\n");

  log_info("Exiting. \n\n");
  return 0;
}

/**
 * Params management:
 *
 * This function will iterate over the provided params and do something with them.
 */
void parse_params(const int argc, const char **argv) {
  /**
   * TODO:
   * split all params by '--' and allow user to pass params as:
   * --dominoes-count 1
   * --dominoes-count=2
   */

  /**
   * TODO:
   * allow user to set dominoes-count with short version of command:
   * -c 1
   * -c=2
   */
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--challenge") == 0) {
      is_challenge = true;
      log_debug("challenge is true");
    } else {
      log_debug("argv[%d]: %s != \"--challenge\"", i, argv[i]);
    }
  }
}

/**
 * Function called on the beginning of the execution.
 */
void initialize(void) {
  log_add_fp(fopen("game.log", "a"), LOG_DEBUG);
  log_info("\n\n\nInitialized.");
  srand(time(0));
//  last_command = calloc(MAX_COMMAND_SIZE, sizeof(char));

//  table_dominoes = calloc(1, sizeof(struct Domino));
//  user_dominoes = calloc(1, sizeof(struct Domino));
//  last_command_feedback = calloc(100, sizeof(char));
//  universe_dominoes = calloc(1, sizeof(struct Domino));
}


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

  node->bottom_left_move = 0;
  node->bottom_right_move = 0;
  node->top_left_move = 0;
  node->top_right_move = 0;
  return node;
}

struct Domino *create_domino(const int left, const int right) {
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

void write_node(
    const struct Node *node,
    char **matrix,
    const int x,
    const int y,
    const bool rtl
) {
  if (node == NULL) {
    printf("node is NULL at write_node\n");
    exit(EXIT_FAILURE);
  }

  if (node->domino == NULL) {
    printf("ERROR: node->domino is NULL at %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  // TODO check if cells are all empty, throw error otherwise.

  log_debug("write_node:%d | %s domino %s | %s %s %s %s", __LINE__, node->is_vert ? "vertical" : "horizontal",
            format_domino(*(node->domino)), node->top_left_move > 0 ? "top_left" : "",
            node->top_right_move > 0 ? "top_right" : "", node->bottom_left_move > 0 ? "bottom_left" : "",
            node->bottom_right_move > 0 ? "bottom_right" : "");

  if (node->is_vert) {
    matrix[y + 0][x + 0] = rtl ? ':' : '[';
    matrix[y + 0][x + 1] = int_to_char(node->domino->left);
    matrix[y + 0][x + 2] = rtl ? ']' : ':';
    matrix[y + 1][x + 0] = rtl ? '[' : ':';
    matrix[y + 1][x + 1] = int_to_char(node->domino->right);
    matrix[y + 1][x + 2] = rtl ? ':' : ']';
    return;
  }

  matrix[y + 0][x + 0] = '[';
  matrix[y + 0][x + 1] = int_to_char(node->domino->left);
  matrix[y + 0][x + 2] = '|';
  matrix[y + 0][x + 3] = int_to_char(node->domino->right);
  matrix[y + 0][x + 4] = ']';
}

/**
 * Will add (quantity) empty columns before selected index (x)
 * @param matrix
 * @param x
 * @param quantity
 * @param matrix_width
 * @param matrix_height
 */
void add_empty_columns_before(char **matrix, const int quantity, const int matrix_width,
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

void add_empty_columns_after(char **matrix, const int quantity, const int matrix_width,
                             const int matrix_height) {
  for (int i = 0; i < matrix_height; i++) {
    char *row_tmp_copy = calloc(matrix_width, sizeof(char));
    for (int j = 0; j < matrix_width; j++) {
      row_tmp_copy[j] = matrix[i][j];
    }

    matrix[i] = calloc(matrix_width + quantity, sizeof(char));
    for (int j = 0; j < matrix_width; j++) {
      matrix[i][j] = row_tmp_copy[j];
    }

    for (int j = 0; j < quantity; j++) {
      matrix[i][matrix_width + j] = ' ';
    }
  }
}

void add_empty_rows_after(char ***matrix, const int quantity, const int matrix_width, const int matrix_height) {
//  char **cpy = calloc(matrix_height, sizeof(char*));
//  memcpy(cpy, *matrix, sizeof(struct Domino) * matrix_height);
//  *matrix = realloc(*matrix, sizeof(char*) * (matrix_height + quantity));

//  if (*matrix == NULL) {
//    printf("Failed to allocate memory for matrix at line %d\n", __LINE__);
//    exit(EXIT_FAILURE);
//  }

  for (int i = 0; i < quantity; i++) {
    (*matrix)[matrix_height + i] = calloc(matrix_width, sizeof(char));
    for (int j = 0; j < matrix_width; j++) {
      (*matrix)[matrix_height + i][j] = ' ';
    }
  }
}

char *matrix_to_str(char **matrix, const int width, const int height) {
  const int max_size = (width * height) + height + 1;
  char *str = calloc(max_size, sizeof(char));

  int write_index = 0;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      str[write_index++] = matrix[i][j];
    }

    str[write_index++] = '\n';
  }

  str[write_index] = '\0';

  return str;
}

void print_matrix(char **matrix, const int width, const int height) {
  char *str = matrix_to_str(matrix, width, height);
  printf("%s", str);
  free(str);
}

void print_tree(struct Node *root) {
  int width = 1, height = 1;
  char **matrix = init_matrix(width, height);
  write_nodes(
      matrix,
      root,
      0, 0,
      &width,
      &height);
  char *str = matrix_to_str(matrix, width, height);
  printf("%s", str);

  free(str);
  free_matrix(matrix, height);
}

/**
 * First rows will always be populated.
 * Since a certain point till the end, we'll have empty rows.
 * @param matrix
 * @param width
 * @param height
 * @return new height of the matrix.
 */
//int remove_unused_rows(char **matrix, const int width, const int height) {
//  int since_row = -1;
//  for (int i = 0; i < height; i++) {
//    for (int j = 0; j < width; j++) {
//      if (matrix[i][j] != ' ') {
//        since_row = i + 1;
//        break;
//      }
//    }
//  }
//
//  if (since_row == -1) return height;
//
//  for (int i = since_row; i < height; i++) {
//    free(matrix[i]);
//  }
//
//  return since_row;
//}

void write_nodes(
    char **matrix,
    struct Node *node,
    const int x,
    const int y,
    int *matrix_width,
    int *matrix_height
) {
  unsigned long int *nodes_already_written = calloc(1, sizeof(int *));
  int nodes_already_written_length = 0;
  write_nodes_rec(matrix, node, x, y, matrix_width, matrix_height, &nodes_already_written, nodes_already_written_length,
                  false);
  free(nodes_already_written);
}

void add_node_to_written(unsigned long int **written, const int written_length, const unsigned long int node_id) {
  *written = realloc(*written, sizeof(unsigned long int) * (written_length + 1));
  (*written)[written_length] = node_id;
}

bool is_long_into_arr(const unsigned long int needle, const unsigned long int *haystack,
                      const unsigned long int haystack_length) {
//  printf("looking for %lu in ", needle);
//
//  for (int i = 0; i < haystack_length; i++) {
//    printf("%lu ", haystack[i]);
//  }
//
//  printf("\n");

  for (int i = 0; i < haystack_length; i++) {
    if (haystack[i] == needle) return true;
  }

  return false;
}

int max2(const int first, const int second) {
  return first > second ? first : second;
}

int max4(const int first, const int second, const int third, const int fourth) {
  int max = first;

  if (second > max) max = second;

  if (third > max) max = third;

  if (fourth > max) max = fourth;

  return max;
}

int count_digits(int number) {
  if (number == 0) return 1;

  int count = 0;

  while (number != 0) {
    number = number / 10;
    count++;
  }

  return count;
}

int add_moves_informations(char **matrix, struct Node *node, int x, const int y, int *matrix_width,
                           int *matrix_height) {

  int x_offset = 0;

  const int node_width = node->is_vert ? 3 : 5;

  const int top_left_move_width = node->top_left_move ? count_digits(node->top_left_move) + 2 : 0;
  const int bottom_left_move_width = node->bottom_left_move ? count_digits(node->bottom_left_move) + 2 : 0;
  const int left_move_width = max2(top_left_move_width, bottom_left_move_width);

  const int top_right_move_width = node->top_right_move ? count_digits(node->top_right_move) + 2 : 0;
  const int bottom_right_move_width = node->bottom_right_move ? count_digits(node->bottom_right_move) + 2 : 0;
  const int right_move_width = max2(top_right_move_width, bottom_right_move_width);

  log_debug("add_moves_informations x: %d, y: %d | %s", x, y, format_domino(*(node->domino)));

  if (node->domino->left == 3 && node->domino->right == 1){
    log_debug("SIUUUUUUUUUUUUUUUU");
  }

  if (left_move_width > x) {
    // Need more space on the left.
    const int adding = left_move_width - x;
    log_debug("Not enough space on left. Adding %d.", adding);
    add_empty_columns_before(matrix, adding, *matrix_width, *matrix_height);
    *matrix_width = (*matrix_width) + adding;
    x_offset = x_offset + adding;
    x += adding;
  }

  if (x + node_width + right_move_width > *matrix_width) {
    const int adding = (x + node_width + right_move_width) - *matrix_width;
    add_empty_columns_after(matrix, adding, *matrix_width, *matrix_height);
    *matrix_width = (*matrix_width) + adding;
    log_debug("No enough space on right. Adding %d", adding);
  }

  /**
   * TOP-LEFT
   */
  if (node->top_left_move && top_left_move_width) {
    matrix[y][x - 3] = '<';
    matrix[y][x - 2] = int_to_char(node->top_left_move);
    matrix[y][x - 1] = '>';
  }

  /**
   * BOTTOM-RIGHT
   */
  if (node->bottom_right_move && bottom_right_move_width) {
    const int y_offset = node->is_vert ? 1 : 0;
    matrix[y + y_offset][x + node_width] = '<';
    matrix[y + y_offset][x + node_width + 1] = int_to_char(node->bottom_right_move);
    matrix[y + y_offset][x + node_width + 2] = '>';
  }

  /**
   * TOP-RIGHT
   */
  if (node->top_right_move && top_right_move_width && node->is_vert) {
    matrix[y][x + node_width] = '<';
    matrix[y][x + node_width + 1] = int_to_char(node->top_right_move);
    matrix[y][x + node_width + 2] = '>';
  }

  /**
   * BOTTOM-LEFT
   */
  if (node->bottom_left_move && bottom_left_move_width && node->is_vert) {
    matrix[y + 1][x - 3] = '<';
    matrix[y + 1][x - 2] = int_to_char(node->bottom_left_move);
    matrix[y + 1][x - 1] = '>';
  }

  return x_offset;
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
void write_nodes_rec(
    char **matrix,
    struct Node *node,
    int x,
    const int y,
    int *matrix_width,
    int *matrix_height,

    // Ids of nodes already written. Avoid infinite loops.
    unsigned long int **nodes_already_written,
    int written_length,

    const bool rtl
) {

  log_debug("write_nodes_rec | node#id: %ld (%d x %d) | x: %d, y: %d | rtl: %d | domino: [%d|%d]", node->id,
            *matrix_width,
            *matrix_height, x, y, rtl,
            node->domino->left,
            node->domino->right
  );

  if (x < 0) {
    printf("ERROR: x < 0");
    exit(EXIT_FAILURE);
  }

  if (y < 0) {
    printf("ERROR: y < 0");
    exit(EXIT_FAILURE);
  }

  if (is_long_into_arr(node->id, *nodes_already_written, written_length)) {
    log_debug("Node %ld already written. Skipping.\n", node->id);
    return;
  }

  const int current_node_width = node->is_vert ? 3 : 5;

  if (current_node_width > *matrix_width) {
    add_empty_columns_before(matrix, current_node_width - *matrix_width, *matrix_width, *matrix_height);
    *matrix_width = current_node_width;
  }

  const int missing_height = (y + 2) - *matrix_height;

  if (node->is_vert && missing_height > 0) {
    add_empty_rows_after(&matrix, missing_height, *matrix_width, *matrix_height);
    *matrix_height = *matrix_height + missing_height;
  }

  x += add_moves_informations(matrix, node, x, y, matrix_width, matrix_height);
  write_node(node, matrix, x, y, rtl);
  add_node_to_written(nodes_already_written, written_length++, node->id);

  if (node->bottom_right && (node->is_vert || !rtl)) {
    const int width_needed = node->bottom_right->is_vert ? 3 : 5;
    const int toadd = (width_needed + x + current_node_width) - *matrix_width;
    if (toadd > 0) {
      add_empty_columns_after(matrix, toadd, *matrix_width, *matrix_height);
      *matrix_width = (*matrix_width) + toadd;
    }

    // We're going right ==>
    write_nodes_rec(matrix, node->bottom_right, x + (node->is_vert ? 3 : 5), y + (node->is_vert ? 1 : 0), matrix_width,
                    matrix_height, nodes_already_written, written_length, false);
  }

  if (node->top_left && rtl) {
    const int width_needed = node->top_left->is_vert ? 3 : 5;
    if (x < width_needed) {
      const int toadd = width_needed - x;
      add_empty_columns_before(matrix, toadd, *matrix_width, *matrix_height);
      *matrix_width = (*matrix_width) + toadd;
    }

    // Going left <==
    write_nodes_rec(matrix, node->top_left, x - width_needed < 0 ? 0 : x - width_needed, y, matrix_width, matrix_height,
                    nodes_already_written, written_length, true);
  }

  if (!node->is_vert) return;

  // Bottom-left positions are only for vertical nodes.
  if (node->bottom_left) {
    // TODO if node->bottom_left is vertical, check there is enough space to write it.
    const int width_needed = node->bottom_left->is_vert ? 3 : 5;
//      const int height_needed = node->bottom_left->is_vert ? 2 : 1;
    if (x < width_needed) {
      const int toadd = width_needed - x;
      add_empty_columns_before(matrix, toadd, *matrix_width, *matrix_height);
      *matrix_width = (*matrix_width) + toadd;
    }

    // We're going left <==
    write_nodes_rec(matrix, node->bottom_left, x - width_needed < 0 ? 0 : x - width_needed, y + 1, matrix_width,
                    matrix_height, nodes_already_written, written_length, true);
  }

  // top-right positions are for vertical nodes only.
  if (node->top_right) {
    const int width_needed = node->top_right->is_vert ? 3 : 5;
    const int toadd = (width_needed + x + current_node_width) - *matrix_width;
    printf("topright x: %d | width_needed: %d | toadd: %d | matrix_width: %d\n", x, width_needed, toadd,
           *matrix_width);
    if (toadd > 0) {
      add_empty_columns_after(matrix, toadd, *matrix_width, *matrix_height);
      *matrix_width = (*matrix_width) + toadd;
    }

    // We're going right ==>
    write_nodes_rec(matrix, node->top_right, x + current_node_width, y, matrix_width,
                    matrix_height, nodes_already_written, written_length, false);
  }
}

struct Node *get_most_left_node(struct Node *node) {
  return node->top_left ? get_most_left_node(node->top_left) : node;
}

struct Node *get_most_right_node(struct Node *node) {
  if (node->is_vert) return node->top_right ? get_most_right_node(node->top_right) : node;

  return node->bottom_right ? get_most_right_node(node->bottom_right) : node;
}

char **init_matrix(const int width, const int height) {
//  char **matrix = malloc(sizeof(char *) * height);
//  for (int i = 0; i < height; i++) {
//    matrix[i] = calloc(width, sizeof(char));
//    for (int j = 0; j < width; j++) {
//      matrix[i][j] = ' ';
//    }
//  }

  char **matrix = calloc(height, sizeof(char *));
  for (int i = 0; i < height; i++) {
    matrix[i] = calloc(width, sizeof(char));
  }

  return matrix;
}

char **init_matrix_with(const int width, const int height, const char ch) {
  char **matrix = calloc(height, sizeof(char *));
  for (int i = 0; i < height; i++) {
    matrix[i] = calloc(width, sizeof(char));

    for (int j = 0; j < width; j++) {
      matrix[i][j] = ch;
    }
  }

  return matrix;
}

void free_matrix(char **matrix, const int height) {
  for (int i = 0; i < height; i++) {
    free(matrix[i]);
  }

  free(matrix);
}

/* ********************* TEST UTILS ********************* */
//void empty_user_dominoes() {
//  user_dominoes = calloc(1, sizeof(struct Domino));
//  user_dominoes_size = 0;
//}
//
//void empty_table_dominoes() {
//  table_dominoes = calloc(1, sizeof(struct Domino));
//  table_dominoes_size = 0;
//}

//void set_user_dominoes(const struct Domino *arr, const int arr_size) {
//  user_dominoes = malloc(sizeof(struct Domino) * arr_size);
//  user_dominoes_size = arr_size;
//
//  user_dominoes = memcpy(user_dominoes, arr, sizeof(struct Domino) * arr_size);
//}
//
//struct Domino *get_user_dominoes(void) {
//  return user_dominoes;
//}

//int get_user_dominoes_size(void) {
//  return user_dominoes_size;
//}
//
//void set_table_dominoes_original(const struct Domino *arr, const int arr_size) {
//  table_dominoes_original = malloc(sizeof(struct Domino) * arr_size);
//  table_dominoes_size = arr_size; // TODO is this an issue?
//
//  table_dominoes_original = memcpy(table_dominoes_original, arr, sizeof(struct Domino) * arr_size);
//}
//
//void set_table_dominoes(const struct Domino *arr, const int arr_size) {
//  table_dominoes = malloc(sizeof(struct Domino) * arr_size);
//  table_dominoes_size = arr_size; // TODO is this an issue?
//
//  table_dominoes = memcpy(table_dominoes, arr, sizeof(struct Domino) * arr_size);
//}
//
//struct Domino *get_table_dominoes(void) {
//  return table_dominoes;
//}

//int get_table_dominoes_size(void) {
//  return table_dominoes_size;
//}

//void set_last_command(char *new_last_command) {
//  last_command = calloc(strlen(new_last_command), sizeof(char));
//
//  last_command = new_last_command;
//}

//char *get_last_command(void) {
//  return last_command;
//}

/* ********************* END TEST UTILS ********************* */

void complete(void) {
//  free(last_command);
//  free(table_dominoes);
//  free(user_dominoes);
}

/**
* MAIN FUNCTION IS INSIDE src/main.c
 * for testing purposes.
 * Run this file with
 * $ make run
*/