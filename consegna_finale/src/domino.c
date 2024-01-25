// 902835
// Source is available at
// https://github.com/kirpachov/c-domino-unive

#include "domino.h"

const char *EXIT_COMMANDS[] = {"exit", "stop", "esci"};
const unsigned EXIT_COMMANDS_COUNT = 3;

bool is_exit_command(char *command) {
  for (unsigned int i = 0; i < EXIT_COMMANDS_COUNT; i++)
    if (strcmp(command, EXIT_COMMANDS[i]) == 0) return true;

  return false;
}

int points_from_tree(struct Node *root) {
  if (root == NULL) return 0;

  int result = 0;
  struct Node **arr = calloc(1, sizeof(struct Node *));
  int size = 0;
  tree_to_array(root, &arr, &size);

  for (int i = 0; i < size; i++) {
    result += arr[i]->domino->left;
    result += arr[i]->domino->right;
  }

  return result;
}

void print_points(struct Node *root) {
  const int points = points_from_tree(root);
  if (points <= 0) return;

  printf("Hai totalizzato %d punti\n", points);
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

int char_to_int(const char ch) {
  return (int) ch - 48;
}

char int_to_char(const int i) {
  return (char) (i + 48);
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

int random_between(const int lower, const int upper) {
  return (rand() % (upper - lower + 1)) + lower;
}

char *format_dominoes_inline(const struct Domino *dominoes, const int size) {
  // TODO calc max size instead of using 100
  const int a = (size + 1) * strlen(" [x|y] ");
  char *result = calloc(a, sizeof(char));

  for (int i = 0; i < size; i++) {
    char *formatted = format_domino(dominoes[i]);
    strcat(result, formatted);
    free(formatted);

    if (i < size - 1) strcat(result, " ");
  }

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

void tree_to_array_rec(struct Node *node, struct Node ***result, int *result_size) {
  if (node_in_array(*result, *result_size, node)) {
//    log_debug("tree_to_array_rec #%lul already in array", node->id);
    return;
  }

//  log_debug("tree_to_array_rec #%lu | %s | result_size: %d", node->id, format_domino(*(node->domino)), *result_size);

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

//bool is_valid_domino_for_move(struct Node *root_node, const struct Domino domino, const int move){
//  struct Node **arr = calloc(1, sizeof(struct Node *));
//  // TODO free arr
//
//  int size = 0;
//  tree_to_array(root_node, &arr, &size);
//}

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
  // TODO free arr

  int size = 0;
  tree_to_array(root_node, &arr, &size);

  for (int i = 0; i < size; i++) {
    if (can_place_domino_on_node(domino, arr[i])) return true;
  }

  return false;
}

int valid_moves_count(struct Node *root_node, const struct Domino *dominoes, const int dominoes_size) {
  int count = 0;
  for (int i = 0; i < dominoes_size; i++) {
    const struct Domino d = dominoes[i];
    if (can_place_domino_in_tree(d, root_node)) count++;
  }

  return count;
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

int str_to_i(const char *str) {
  char *str_end;
  const long result = strtol(str, &str_end, 10);

  if (*str_end != '\0' && *str_end != '\n') {
    log_debug("Error while converting string into integer. String was \"%s\".\n", str);
    return -1;
  }

  return (int) result;
}

int select_position(
    struct Node *root_node,
    struct Domino domino,
    struct Node **to_attach_node
) {
  log_debug("select_position #%ul %s | domino: %s", root_node->id, format_domino(*(root_node->domino)),
            format_domino(domino));

  int valid_position = -1;
  char *command = calloc(100, sizeof(char));
  char *command_feedback = NULL;
  struct Node **arr = calloc(1, sizeof(struct Node *)); // TODO free arr
  int size = 0;
  tree_to_array(root_node, &arr, &size);
  while (valid_position == -1) {
    clean_screen();
    add_moves_to_tree(root_node, domino);
    print_table(root_node);
    printf("\n");
    print_points(root_node);
    printf("?> Hai selezionato la tessera %s\n", format_domino(domino));
    printf("?> Seleziona la posizione in cui mettere il domino indicando l'<indice>.\n");
    if (command_feedback) printf("?> %s\n", command_feedback);
    acquire_command(&command);
    const int position = str_to_i(command);
    for (int i = 0; i < size; i++) {
      struct Node *node = arr[i];
      if (!(node->top_left_move == position || node->top_right_move == position || node->bottom_left_move == position ||
            node->bottom_right_move == position))
        continue;

      valid_position = position;
      *to_attach_node = node;
      log_debug(
          "Found node #%lu | %s | top_left_move: %d, top_right_move: %d, bottom_left_move: %d, bottom_right_move: %d",
          node->id, format_domino(*(node->domino)),
          node->top_left_move, node->top_right_move, node->bottom_left_move, node->bottom_right_move);
      break;
    }

    if (valid_position == -1) command_feedback = "Posizione non valida.";
  }

  free(command);
//  free(command_feedback);
//  remove_moves_from_tree(root_node);
  log_debug("exiting select_position");
  return valid_position;
}

int select_domino(
    struct Node *root_node,
    struct Domino *user_dominoes,
    const int user_dominoes_size
) {
  int valid_index = -1;
  char *command = calloc(100, sizeof(char));
  char *command_feedback = NULL;
  while (valid_index == -1) {
    log_debug("init while");
    clean_screen();
    print_table(root_node);
    printf("Tessere disponibili:\n");
    print_dominoes_with_valid_moves(root_node, user_dominoes, user_dominoes_size);
    printf("\n");
    print_points(root_node);
    printf("?> Seleziona il domino da posizionare utilizzando l'indice indicato sulla sinistra.\n");
    if (command_feedback) printf("?> %s\n", command_feedback);
    acquire_command(&command);
    const int index = str_to_i(command);
    if (index <= 0 || index > user_dominoes_size) {
      command_feedback = "Indice non valido.";
      continue;
    }

    log_debug("before[]");
    const struct Domino d = user_dominoes[index - 1];
    log_debug("after[]");
    if (can_place_domino_in_tree(d, root_node)) {
      valid_index = index - 1;
    }
    log_debug("after can_place_domino_in_tree");
  }

  free(command);

  log_debug("after free command. %s", command_feedback);

//  if (command_feedback) free(command_feedback);

  log_debug("after free command_feedback");

  return valid_index;
}

struct Domino rotate_domino(const struct Domino d) {
  return (struct Domino) {d.right, d.left};
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

  if (is_exit_command(*last_command)) exit(EXIT_SUCCESS);
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

/**
 * Returns true if vertical.
 */
bool select_orientation(struct Node *root_node, struct Domino selected_domino) {
  bool is_vert = false;
  bool valid_command = false;
  char *command = calloc(100, sizeof(char));
  char *command_feedback = NULL;
  while (!valid_command) {
    clean_screen();
    print_table(root_node);
    printf("\n");
    printf("?> Hai selezionato la tessera %s\n", format_domino(selected_domino));
    printf("?> Seleziona come vuoi posizionare la tessera: in verticare oppure in orizzontale.\n");
    printf("?> VERTICALE => vert, 1, verticale.\n");
    printf("?> ORIZZONTALE => ori, 2, orizzontale.\n");
    if (command_feedback) printf("?> %s\n", command_feedback);
    acquire_command(&command);
    if (strcmp(command, "1") == 0 || strcmp(command, "vert") == 0 || strcmp(command, "VERT") == 0 ||
        strcmp(command, "v") == 0 || strcmp(command, "vertical") == 0 || strcmp(command, "verticale") == 0) {
      valid_command = true;
      is_vert = true;
    } else if (strcmp(command, "2") == 0 || strcmp(command, "ori") == 0 || strcmp(command, "ORIZZONTALE") == 0 ||
               strcmp(command, "o") == 0 || strcmp(command, "orizzontale") == 0) {
      valid_command = true;
      is_vert = false;
    } else {
      command_feedback = "Comando non valido. Riprova.";
      valid_command = false;
    }
  }

  free(command);

//  if (command_feedback) free(command_feedback);

  return is_vert;
}

void display_welcome_message(void) {
  char *command = calloc(100, sizeof(char));
  printf("%s\n", WELCOME_MESSAGE);
  acquire_command(&command);
  free(command);
}

// \sas
int run_interactive(void) {
  log_info("Running in interactive mode.");
  const bool specials_enabled = false;
  int users_size = 10;
  struct Domino *users = init_random_dominoes_arr(users_size, specials_enabled);
  log_debug("User's dominoes: %s", format_dominoes_inline(users, users_size));

  clean_screen();
  display_welcome_message();

  struct Node *root = NULL;
  struct Node *selected_node = NULL;

  while (valid_moves_count(root, users, users_size) > 0) {
    const int selected_domino_index = select_domino(root, users, users_size);
    struct Domino selected_domino = users[selected_domino_index];

    log_debug("Selected %s domino", format_domino(selected_domino));
    if (root == NULL) {
      const bool is_vert = select_orientation(root, selected_domino);
      root = create_node(create_domino(selected_domino.left, selected_domino.right), is_vert);
      pop(selected_domino_index, users, users_size--);
      resize_dominoes_array(&users, users_size);
      continue;
    }

    const int ask_domino_position = select_position(root, selected_domino, &selected_node);
    log_debug("ask_domino_position returned %d. Selected node is #%ul", ask_domino_position, selected_node->id);

    log_debug("top_left_move: %d, top_right_move: %d, bottom_left_move: %d, bottom_right_move: %d",
              selected_node->top_left_move, selected_node->top_right_move, selected_node->bottom_left_move,
              selected_node->bottom_right_move);

    int pos = -1;
    if (selected_node->top_left_move == ask_domino_position) {
      pos = TOP_LEFT;
    } else if (selected_node->top_right_move == ask_domino_position) {
      pos = TOP_RIGHT;
    } else if (selected_node->bottom_left_move == ask_domino_position) {
      pos = BOTTOM_LEFT;
    } else if (selected_node->bottom_right_move == ask_domino_position) {
      pos = BOTTOM_RIGHT;
    } else {
      printf("Something went wrong. Selected node is #%lu. Selected move is %d\n", selected_node->id,
             ask_domino_position);
      exit(EXIT_FAILURE);
    }

    remove_moves_from_tree(root);

    const bool is_vert = select_orientation(root, selected_domino);

    log_debug("is_vert %d", is_vert);

    link_nodes(selected_node, create_node(
        create_domino(selected_domino.left, selected_domino.right), is_vert), pos);

    root = get_most_left_node(root);

    pop(selected_domino_index, users, users_size--);
    resize_dominoes_array(&users, users_size);
    log_debug("\n\n");
  }

  clean_screen();
  print_table(root);
  print_dominoes_with_valid_moves(root, users, users_size);
  print_points(root);
  printf("Ciao ciao!\n");

  log_info("Exiting. \n\n");
  return 0;
}

/**
 * Function called on the beginning of the execution.
 */
void initialize(void) {
  log_add_fp(fopen("game.log", "a"), LOG_DEBUG);
  log_info("\n\n\nInitialized.");
  srand(time(0));
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

void rotate_if_necessary(struct Node *parent, struct Node *child, int side) {
  switch (side) {
    case TOP_LEFT:
      // left side of child | right side of parent
      log_debug("rotate_if_necessary TOP_LEFT, child: %s, parent: %s", format_domino(*(child->domino)),
                format_domino(*(parent->domino)));
      // right side of child | left side of parent
      if (parent->domino->left != child->domino->right && parent->domino->left == child->domino->left) {
        struct Domino *tmp = child->domino;
        log_debug("rotated");
        child->domino = create_domino(child->domino->right, child->domino->left);
        free(tmp);
      }

      parent->top_left = child;
      child->bottom_right = parent;
      break;
    case TOP_RIGHT:
      // left side parent | left side child
      if (!parent->is_vert) {
        printf("Tried to link top right node to a horizontal node\n");
        exit(EXIT_FAILURE);
      }

      if (parent->domino->left != child->domino->left && parent->domino->left == child->domino->right) {
        struct Domino *tmp = child->domino;
        child->domino = create_domino(child->domino->right, child->domino->left);
        free(tmp);
      }

      parent->top_right = child;
      child->top_left = parent;
      break;
    case BOTTOM_LEFT:
      // right side parent | right side child
      if (!parent->is_vert) {
        printf("Tried to link bottom left node to a horizontal node\n");
        exit(EXIT_FAILURE);
      }

      if (parent->domino->right != child->domino->right && parent->domino->right == child->domino->left) {
        struct Domino *tmp = child->domino;
        child->domino = create_domino(child->domino->right, child->domino->left);
        free(tmp);
      }

      parent->bottom_left = child;
      child->bottom_right = parent;

      break;
    case BOTTOM_RIGHT:
      // right side parent | left side child
      if (parent->domino->right != child->domino->left && parent->domino->right == child->domino->right) {
        struct Domino *tmp = child->domino;
        child->domino = create_domino(child->domino->right, child->domino->left);
        free(tmp);
      }

      parent->bottom_right = child;
      child->top_left = parent;
      break;
  }
}

/**
 * @param parent is the node that is already part of the tree.
 * @param child node to add.
 * @param side where to add the child node.
 */
void link_nodes(struct Node *parent, struct Node *child, int side) {
  rotate_if_necessary(parent, child, side);

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
  for (unsigned long int i = 0; i < haystack_length; i++) {
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

void free_matrix(char **matrix, const int height) {
  for (int i = 0; i < height; i++) {
    free(matrix[i]);
  }

  free(matrix);
}

/**
* MAIN FUNCTION IS INSIDE src/main.c
 * for testing purposes.
 * Run this file with
 * $ make run
*/


int main(void) {
  initialize();

  return run_interactive();
}
