// 902835
// Source is available at
// https://github.com/kirpachov/c-domino-unive

#include "domino.h"

const char *LEFT_IDENTIFIERS[] = {" LEFT ", " L "};
const unsigned LEFT_IDENTIFIERS_COUNT = 2;

const char *RIGHT_IDENTIFIERS[] = {" RIGHT ", " R "};
const unsigned RIGHT_IDENTIFIERS_COUNT = 2;

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
bool specials = true;

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


int total_user_dominoes = 0;  // TODO --hand=X | -c=X (??)

int total_user_dominoes_valid(void) {
  if (total_user_dominoes > 0) return total_user_dominoes;

  return TOTAL_USER_DOMINOES;
}

/**
 * Dominoes owned by user
 */
struct Domino *user_dominoes;
int user_dominoes_size = 0;

void user_dominoes_push(const struct Domino d) {
  resize_dominoes_array(&user_dominoes, user_dominoes_size + 1);

  push(d, user_dominoes, user_dominoes_size++);
}

void user_dominoes_pop(const int index) {
  pop(index, user_dominoes, user_dominoes_size--);
  resize_dominoes_array(&user_dominoes, user_dominoes_size);
}

/**
 * Dominoes currently on table.
 * Variable 'table_dominoes' will contain the actual dominoes after special dominoes effect has been effected.
 * Variable 'table_dominoes_original' will contain the original dominoes, without any special dominoes effect.
 */
struct Domino *table_dominoes;
struct Domino *table_dominoes_original;
int table_dominoes_size = 0;

void table_dominoes_push(const struct Domino d, const bool left_side) {
  resize_dominoes_array(&table_dominoes, table_dominoes_size + 1);
  resize_dominoes_array(&table_dominoes_original, table_dominoes_size + 1);

  if (left_side) {
    unshift(d, table_dominoes, table_dominoes_size);
    unshift(d, table_dominoes_original, table_dominoes_size);
  } else {
    push(d, table_dominoes, table_dominoes_size);
    push(d, table_dominoes_original, table_dominoes_size);
  }

  table_dominoes_size++;
}

void table_dominoes_pop(const int index) {
  pop(index, table_dominoes, table_dominoes_size);
  pop(index, table_dominoes_original, table_dominoes_size);
  table_dominoes_size--;

  //  resize_dominoes_array(&table_dominoes, table_dominoes_size - 1);
  // TODO is -1 necessary since it's already decremented?

  resize_dominoes_array(&table_dominoes, table_dominoes_size);
  resize_dominoes_array(&table_dominoes_original, table_dominoes_size);
}

/**
 * All available dominoes
 */
struct Domino *universe_dominoes;
int universe_dominoes_size = 0;

void universe_dominoes_push(const struct Domino d) {
  push(d, universe_dominoes, universe_dominoes_size++);
}

char *last_command;
char *last_command_feedback;

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
  return specials && ((domino.left == 12 && domino.right == 21) || (domino.left == 21 && domino.right == 12));
}

/**
 * Is [11|11] domino?
 * @param domino struct Domino
 * @return true if it's a [11|11] domino and specials are enabled
 */
bool is_addone_domino(const struct Domino domino) {
  return specials && domino.left == 11 && domino.right == 11;
}

/**
 * Is [0|0] domino?
 * @param domino struct Domino
 * @return true if it's a [0|0] domino.
 */
bool is_glue_domino(const struct Domino domino) {
  return specials && domino.left == 0 && domino.right == 0;
}

bool is_domino_special(const struct Domino domino) {
  return is_glue_domino(domino) || is_addone_domino(domino) || is_mirror_domino(domino);
}

/**
 * CAN PLACE ON LEFT?
 * @return true if domino can be placed on right side of the table.
 */

bool can_place_on_left_unrotated(const struct Domino d) {
  if (table_dominoes_size == 0 && is_mirror_domino(d)) return false;
  if (table_dominoes_size <= 0) return true;
  if (is_domino_special(d)) return true;
  if (specials && table_dominoes[table_dominoes_size - 1].left == 0) return true;

  return table_dominoes[table_dominoes_size - 1].left == d.right;
}

bool can_place_on_left_rotated(const struct Domino d) {
  if (table_dominoes_size == 0 && is_mirror_domino(d)) return false;
  if (table_dominoes_size <= 0) return true;
  if (is_domino_special(d)) return true;
  if (specials && table_dominoes[table_dominoes_size - 1].left == 0) return true;

  return table_dominoes[table_dominoes_size - 1].left == d.left;
}

bool can_place_on_left(const struct Domino d) {
  return can_place_on_left_unrotated(d) || can_place_on_left_rotated(d);
}

/**
 * CAN PLACE ON RIGHT?
 * @return true if domino can be placed on right side of the table.
 */

bool can_place_on_right_unrotated(const struct Domino d) {
  if (table_dominoes_size == 0 && is_mirror_domino(d)) return false;
  if (table_dominoes_size <= 0) return true;
  if (is_domino_special(d)) return true;
  if (specials && table_dominoes[table_dominoes_size - 1].right == 0) return true;

  return table_dominoes[table_dominoes_size - 1].right == d.left;
}

bool can_place_on_right_rotated(const struct Domino d) {
  if (table_dominoes_size == 0 && is_mirror_domino(d)) return false;
  if (table_dominoes_size <= 0) return true;
  if (is_domino_special(d)) return true;
  if (specials && table_dominoes[table_dominoes_size - 1].right == 0) return true;

  return table_dominoes[table_dominoes_size - 1].right == d.right;
}

bool can_place_on_right(const struct Domino d) {
  return can_place_on_right_unrotated(d) || can_place_on_right_rotated(d);
}

/**
 * Can this domino be placed anywhere on the table?
 * @param d
 * @return bool
 */
bool can_place_on_table(const struct Domino d) {
  return can_place_on_left(d) || can_place_on_right(d);
}

struct Domino *valid_moves(void) {
  struct Domino *result = malloc(sizeof(struct Domino) * valid_moves_count());
  int result_index = 0;

  for (int i = 0; i < user_dominoes_size; i++) {
    if (can_place_on_table(user_dominoes[i])) {
      result[result_index] = user_dominoes[i];
      result_index++;
    }
  }

  return result;
}

int valid_moves_count(void) {
  if (user_dominoes_size <= 0) return 0;

  int count = 0;
  for (int i = 0; i < user_dominoes_size; i++) {
    if (can_place_on_table(user_dominoes[i])) count++;
  }

  return count;
}

int valid_moves_count_side(const bool left_side, struct Domino *valid_moves) {
  int count = 0;

  for (int i = 0; i < user_dominoes_size; i++) {
    const struct Domino domino = user_dominoes[i];
    if ((left_side && !can_place_on_left(domino)) || (!left_side && !can_place_on_right(domino)))
      continue;

    valid_moves[count] = domino;
    count++;
  }

//  valid_moves = realloc(valid_moves, sizeof(struct Domino) * count);

  return count;
}

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

struct Domino random_domino_from_universe(void) {
  return universe_dominoes[random_between(0, universe_dominoes_size - 1)];
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

  log_debug("Completed the acquisition of the dominoes. Got: %s", format_dominoes_for_table(dominoes, dominoes_size));

  printf("%s\n", process_challenge_1(dominoes, dominoes_size));

  return 0;
}

int run_challenge(void) {
  log_info("Running in challenge mode.");

  return run_challenge_1();
}

void populate_universe_dominoes(void) {
  int final_size = 0; // TODO calc this value without any for loop.
  for (int i = SMALLES_DOMINO_VALUE; i <= LARGEST_DOMINO_VALUE; i++)
    for (int j = i; j <= LARGEST_DOMINO_VALUE; j++)
      final_size++;

  if (specials) final_size += 3; // [0|0], [11|11], [12|21]

  universe_dominoes = malloc(sizeof(struct Domino) * final_size);

  universe_dominoes_size = 0;
  for (int i = SMALLES_DOMINO_VALUE; i <= LARGEST_DOMINO_VALUE; i++) {
    for (int j = i; j <= LARGEST_DOMINO_VALUE; j++) {
      const struct Domino d = {i, j};
//      char *formatted_domino = format_domino(d);
//      log_debug("Adding %s to universe.", formatted_domino);
//      free(formatted_domino);

      universe_dominoes_push(d);
    }
  }

  if (specials) {
    universe_dominoes_push((struct Domino) {0, 0});
    universe_dominoes_push((struct Domino) {11, 11});
    universe_dominoes_push((struct Domino) {12, 21});
  }

  log_debug("universe_dominoes_size is %d", universe_dominoes_size);
}

char *format_dominoes_for_table(const struct Domino *dominoes, const int size) {
  // TODO calc max size instead of using 100
  char *result = calloc(100, sizeof(char));

  for (int i = 0; i < size; i++) {
    char *formatted = format_domino(dominoes[i]);
    strcat(result, formatted);
    free(formatted);

    if (i < size - 1) strcat(result, " ");
  }

  return result;
}

char *format_table_dominoes(void) {
  return format_dominoes_for_table(table_dominoes, table_dominoes_size);
}

void print_table(void) {
  printf("Table:\n");
  if (table_dominoes_size == 0) {
    printf("<Table is empty>\n\n");
    return;
  }

  printf("%s", format_table_dominoes());

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

    const bool left = can_place_on_left(d);
    const bool right = can_place_on_right(d);
    const bool can_place = left || right;
    char *formatted_domino = format_domino(d);
    strcat(result, formatted_domino);
    free(formatted_domino);

    if (can_place) {
      strcat(result, "  ");

      if (left) {
        sprintf(accumulator, "(%dL)", i + 1);
        strcat(result, accumulator);
      }
      if (left && right) {
        strcat(result, "  /  ");
      }
      if (right) {
        sprintf(accumulator, "(%dR)", i + 1);
        strcat(result, accumulator);
      }
    }

    strcat(result, "\n");
  }

  return result;
}

char *format_user_dominoes() {
  return format_dominoes_with_valid_moves(user_dominoes, user_dominoes_size);
}

void print_user_dominoes(void) {
  printf("Your dominoes:\n");
  printf("%s", format_user_dominoes());
}

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

int calc_user_points(void) {
  return calc_points_from(table_dominoes, table_dominoes_size);
}

char *format_user_points_str(void) {
  char *result = calloc(strlen("Your points: 999999"), sizeof(char));
  sprintf(result, "Your points: %d", calc_user_points());
  return result;
}

void print_user_points(void) {
  printf("%s\n", format_user_points_str());
}

void print_last_command_feedback(void) {
  if (strlen(last_command) == 0 || is_help_command(last_command)) {
    printf("Welcome!\n"
           "Select the domino to place by specifying the domino's index and the side (Right/Left) where to place it.\n"
           "Example: \"1L\" will place the first domino on the table, to the left of the other dominos\n");
    return;
  }

  printf("%s\n", last_command_feedback);
}

void clean_screen(void) {
#ifdef _WIN64
  system("cls");
#elif __linux
  const int result = system("clear");
  log_debug("system(\"clear\") returned %d", result);
#else
  log_error("WARNING: CANNOT CLEAR SCREEN.");
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n")
#endif
}

void print_everything() {
  clean_screen();
  print_table();
  print_user_dominoes();

  for (int i = 0; i < table_dominoes_size; i++) printf("\n");

  printf("\n");
  if (calc_user_points() > 0) print_user_points();
  else printf("\n"); // Place a newline for consistency

  print_last_command_feedback();
}

void assign_user_random_dominoes(void) {
  while (user_dominoes_size < total_user_dominoes_valid()) {
    const struct Domino d = random_domino_from_universe();
    char *formatted_domino = format_domino(d);
    log_debug("Adding %s to user's hand", formatted_domino);
    free(formatted_domino);
    user_dominoes_push(d);
  }
}

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

bool can_be_put_on_table(const struct Domino domino, const bool left_side) {
  return left_side ? can_place_on_left(domino) : can_place_on_right(domino);
}

bool needs_to_be_rotated_before_putting_on_table(const struct Domino selected_domino, const bool is_left_side) {
  if (table_dominoes_size <= 0) return false;

  const struct Domino table_domino = is_left_side ? table_dominoes[0] : table_dominoes[table_dominoes_size - 1];

  if (is_left_side) return table_domino.left != selected_domino.right;

  return table_domino.right != selected_domino.left;
}

struct Domino rotate_domino(const struct Domino d) {
  return (struct Domino) {d.right, d.left};
}

struct Domino rotate_if_necessary(const struct Domino domino, const bool is_left_side) {
  return needs_to_be_rotated_before_putting_on_table(domino, is_left_side) ? rotate_domino(domino) : domino;
}

/**
 * @param index starting from 0
 * @param left_side boolean value
 * @return 0 if want to interrupt `last_command` processing.
 */
int put_on_table(const int user_dominoes_index, const bool left_side) {
  const struct Domino selected = user_dominoes[user_dominoes_index];
  const char *side_str = left_side ? "left" : "right";
  char *selected_str = format_domino(selected);
  // TODO calc size instead of 100
  last_command_feedback = calloc(100, sizeof(char));

//  log_debug("put_on_table | user_dominoes_index: %d | side: %s | domino: %s", user_dominoes_index, side_str, selected_str);

  if (!(can_be_put_on_table(selected, left_side))) {
    sprintf(last_command_feedback, "Domino %s cannot be put on %s side", selected_str, side_str);
    return 0;
  }

  table_dominoes_push(rotate_if_necessary(selected, left_side), left_side);

  user_dominoes_pop(user_dominoes_index);

  sprintf(last_command_feedback, "Domino %s has been put on %s side", selected_str, side_str);

  apply_special_dominoes_effect();

  free(selected_str);

  return 0;
}

void process_last_command(void) {
  log_debug("processing command \"%s\"", last_command);
  if (is_help_command(last_command)) return;

  int selected_domino_index = 0; // 0 is invalid as index
  int selected_domino_side = 0; // 0 is invalid as position
  guess_selection(last_command, &selected_domino_index, &selected_domino_side);

  log_debug("guess_selection returned | index %d | selected_domino_side %d", selected_domino_index,
            selected_domino_side);

  if (selected_domino_index == 0 || selected_domino_side == 0 || (selected_domino_index - 1) > user_dominoes_size) {
    log_info("user selected invalid index (%d) or position (%d) (command is: \"%s\")", selected_domino_index,
             selected_domino_side, last_command);

    last_command_feedback = "Invalid command.";
    return;
  }

  selected_domino_index--;

  if (put_on_table(selected_domino_index, selected_domino_side == LEFT_SIDE) == 0) return;

  /**
   * TODO:
   * - if user has invalid position or index, populate "last_command_feedback" with the error messages.
   * otherwise, select domino and try to put it on the table.
   * if it's a valid move, populate last_command_feedback with something like "Domino [x|y] has been put on <left/right> side"
   * otherwise, if it's not a valid move, let user know that it's not a valid move.
   *
   */

  last_command_feedback = "Invalid command.";
}

void acquire_command(void) {
  char tmp_command[MAX_COMMAND_SIZE];
  int N = 0;
  char ch;

  printf("$ ");
  ch = getchar();

  while (ch != '\n' && N < MAX_COMMAND_SIZE) {
    tmp_command[N++] = ch;
    ch = getchar();
  }

  free(last_command);
  last_command = calloc(N, sizeof(char));

  if (last_command == NULL) exit(EXIT_FAILURE);

  for (int i = 0; i < N; i++) last_command[i] = tmp_command[i];
}

/**
 * Apply effect of [11|11].
 * Add 1 to all the values of all dominoes on the table. If 6 + 1, set 1.
 */
void apply_special_sumall(const int sumall_index) {
  for (int j = 0; j < table_dominoes_size; j++) {
    if (j == sumall_index) continue;

    table_dominoes[j].left++;
    table_dominoes[j].right++;

    if (table_dominoes[j].left > 6) table_dominoes[j].left = 1;
    if (table_dominoes[j].right > 6) table_dominoes[j].right = 1;
  }

  if (sumall_index == 0) {
    table_dominoes[sumall_index].left = table_dominoes[1].left;
    table_dominoes[sumall_index].right = table_dominoes[1].left;
    return;
  }

  if (sumall_index == table_dominoes_size - 1) {
    table_dominoes[sumall_index].left = table_dominoes[table_dominoes_size - 2].right;
    table_dominoes[sumall_index].right = table_dominoes[table_dominoes_size - 2].right;
    return;
  }

  printf("invalid sumall_index %d at row %d\n", sumall_index, __LINE__);
  log_error("apply_special_sumall: sumall_index is %d, but it should be 0 or %d", sumall_index,
            table_dominoes_size - 1);
  exit(EXIT_FAILURE);
}

void apply_special_mirror(const int mirror_index) {
  if (mirror_index == 0) {
    table_dominoes[mirror_index].right = table_dominoes[1].left;
    table_dominoes[mirror_index].left = table_dominoes[1].right;
    return;
  }

  if (mirror_index == table_dominoes_size - 1) {
    const struct Domino last = table_dominoes[table_dominoes_size - 2];
    table_dominoes[mirror_index].right = last.left;
    table_dominoes[mirror_index].left = last.right;
    return;
  }

  log_error("apply_special_mirror: mirror_index is %d, but it should be 0 or %d", mirror_index,
            table_dominoes_size - 1);
  exit(EXIT_FAILURE);
}

void apply_special_dominoes_effect(void) {
  if (!specials) return;
  if (table_dominoes_size <= 1) return;

  for (int i = 0; i < table_dominoes_size; i++) {
    const struct Domino domino = table_dominoes[i];
    if (!is_domino_special(domino)) continue;
    if (domino.left == 0 && domino.right == 0) continue;

    if (domino.left == 11 && domino.right == 11) {
      apply_special_sumall(i);
      continue;
    }

    if ((domino.left == 12 && domino.right == 21) || (domino.right == 12 && domino.left == 21)) {
      apply_special_mirror(i);
      continue;
    }

    // Should never see this message
    printf("TODO manage special domino of type %s\n", format_domino(domino));
  }
}

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
int scenario_with(
    const struct Domino *user_arr,
    const int user_arr_size,

    const struct Domino *table_arr,
    const int table_arr_size,

    const struct Domino *table_arr_original,

    struct Domino **best_table_possible,
    int *best_table_possible_size,
    struct Domino **best_table_possible_original
) {
  set_table_dominoes(table_arr, table_arr_size);
  set_table_dominoes_original(table_arr_original, table_arr_size);

  set_user_dominoes(user_arr, user_arr_size);

  /* DEBUG */
//  struct Domino *valid_moves = calloc(user_dominoes_size, sizeof(struct Domino));
//  int valid_moves_count = valid_moves_count_side(false, valid_moves);
//  log_debug("SCENARIO_WITH | user_dominoes: %s | table_dominoes: %s | valid_moves: %s",
//            format_dominoes_for_table(user_dominoes, user_dominoes_size),
//            format_dominoes_for_table(table_dominoes, table_dominoes_size),
//            format_dominoes_for_table(valid_moves, valid_moves_count)
//  );
  /* /DEBUG */

  if (calc_points_from(*best_table_possible, *best_table_possible_size) < calc_user_points()) {
    *best_table_possible = malloc(sizeof(struct Domino) * table_arr_size);
    *best_table_possible_original = malloc(sizeof(struct Domino) * table_arr_size);
    *best_table_possible_size = table_arr_size;

    memcpy(*best_table_possible, table_arr, sizeof(struct Domino) * table_arr_size);
    memcpy(*best_table_possible_original, table_arr_original, sizeof(struct Domino) * table_arr_size);
  }

//  if (valid_moves_count == 0) {
//    log_debug("No more valid moves. | points: %d | Table: %s ", calc_user_points(),
//              format_dominoes_for_table(table_dominoes, table_dominoes_size)
//    );
//
//    return calc_user_points();
//  }

  int best_until_now = calc_user_points();

  for (int i = 0; i < user_arr_size; i++) {
    const struct Domino domino = user_arr[i];

    if (!(can_place_on_right(domino))) {
      /* DEBUG */
//      log_debug("Cannot place on right %s", format_domino(domino));
      /* /DEBUG */
      continue;
    }

    /* DEBUG */
//    log_debug("for has now domino = %s | Table is %s | user has %s", format_domino(domino),
//              format_dominoes_for_table(table_dominoes, table_dominoes_size),
//              format_dominoes_for_table(user_dominoes, user_dominoes_size));
    /* /DEBUG */

    int first_valid_move_index_in_user_arr = -1;
    for (int j = 0; j < user_dominoes_size; j++) {
      if (!dominoes_equal(user_dominoes[j], domino)) continue;

      first_valid_move_index_in_user_arr = j;
      break;
    }

    if (first_valid_move_index_in_user_arr == -1) {
      log_error("I was unable to find domino %s in user dominoes (%s)", format_domino(domino),
                format_dominoes_for_table(user_dominoes, user_dominoes_size));

      exit(EXIT_FAILURE);
    }

    bool was_put_unrotated = false;
    struct Domino *table_dominoes_before_special_effects;

    if (can_place_on_right_unrotated(domino)) {
      /* DEBUG */
//      log_debug("Putting domino %s NOT rotated on table %s",
//                format_domino(domino),
//                format_dominoes_for_table(table_dominoes, table_dominoes_size)
//      );
      /* /DEBUG */


      was_put_unrotated = true;
      table_dominoes_push(domino, false);
      user_dominoes_pop(first_valid_move_index_in_user_arr);
      table_dominoes_before_special_effects = calloc(table_dominoes_size, sizeof(struct Domino));
      memcpy(table_dominoes_before_special_effects, table_dominoes, sizeof(struct Domino) * table_dominoes_size);
      apply_special_dominoes_effect();

      const int current = scenario_with(
          user_dominoes,
          user_dominoes_size,
          table_dominoes,
          table_dominoes_size,
          table_dominoes_original,
          best_table_possible,
          best_table_possible_size,
          best_table_possible_original
      );

      if (best_until_now < current) {
        best_until_now = current;
      }

      set_table_dominoes(table_dominoes_before_special_effects, table_dominoes_size);
//      free(table_dominoes_before_special_effects);
      table_dominoes_pop(table_dominoes_size - 1);
    }

    if (domino.left != domino.right && can_place_on_right_rotated(domino)) {
      /* DEBUG */
//      log_debug("Putting domino %s rotated on table %s",
//                format_domino(rotate_domino(domino)),
//                format_dominoes_for_table(table_dominoes, table_dominoes_size)
//      );
      /* /DEBUG */

      table_dominoes_push(rotate_domino(domino), false);
      if (!was_put_unrotated) { user_dominoes_pop(first_valid_move_index_in_user_arr); }
      table_dominoes_before_special_effects = calloc(table_dominoes_size, sizeof(struct Domino));
      memcpy(table_dominoes_before_special_effects, table_dominoes, sizeof(struct Domino) * table_dominoes_size);
      apply_special_dominoes_effect();

      const int current = scenario_with(
          user_dominoes,
          user_dominoes_size,
          table_dominoes,
          table_dominoes_size,
          table_dominoes_original,
          best_table_possible,
          best_table_possible_size,
          best_table_possible_original
      );

      if (best_until_now < current) {
        best_until_now = current;
      }

      set_table_dominoes(table_dominoes_before_special_effects, table_dominoes_size);
    }

    set_table_dominoes(table_arr, table_arr_size);
    set_table_dominoes_original(table_arr_original, table_arr_size);
    set_user_dominoes(user_arr, user_arr_size);
    /* DEBUG */
//    log_debug("Ending block where domino was %s | Table domino is %s | User domino is %s",
//              format_domino(domino), format_dominoes_for_table(table_dominoes, table_dominoes_size),
//              format_dominoes_for_table(user_dominoes, user_dominoes_size));
    /* /DEBUG */
  }

  return best_until_now;
}

int best_scenario(
    const struct Domino *dominoes,
    const int dominoes_size,
    struct Domino **best_table_possible,
    int *best_table_possible_size,
    struct Domino **best_table_possible_original
) {
  return scenario_with(
      dominoes,
      dominoes_size,
      (struct Domino[]) {{0, 0}},
      0,
      (struct Domino[]) {{0, 0}},
      best_table_possible,
      best_table_possible_size,
      best_table_possible_original
  );
}

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
char *process_challenge_1(const struct Domino *dominoes, const int dominoes_size) {
  // TODO calc instead of static 100
  struct Domino *best_table = calloc(100, sizeof(struct Domino));
  struct Domino *best_table_original = calloc(100, sizeof(struct Domino));
  if (best_table == NULL) {
    log_error("Failed to allocate memory for best_table at line %d", __LINE__);
    exit(EXIT_FAILURE);
  }

  int best_table_size = 1;
  int best_points = best_scenario(dominoes, dominoes_size, &best_table, &best_table_size, &best_table_original);

  char *commands = format_dominoes_as_commands(best_table_original, best_table_size);
//  best_table = realloc(best_table, sizeof(struct Domino) * best_table_size);
  if (best_table == NULL) {
    log_error("Failed to reallocate memory for best_table at line %d", __LINE__);
    exit(EXIT_FAILURE);
  }

  log_debug("process_challenge_1: universe: %s | best table is %s | max points: %d | commands: %s",
            format_dominoes_for_table(dominoes, dominoes_size),
            format_dominoes_for_table(best_table, best_table_size),
            best_points,
            commands
  );

  return commands;
}

void run_terminal(void) {
  while (user_dominoes_size > 0 && valid_moves_count() > 0) {
    acquire_command();
    process_last_command();
    print_everything();
  }

  printf("Game over!\n");
  printf("You made %d points!\n", calc_user_points());
}

int run_interactive(void) {
  log_info("Running in interactive mode.");
  populate_universe_dominoes();
  assign_user_random_dominoes();

  print_everything();
  run_terminal();

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
  last_command = calloc(MAX_COMMAND_SIZE, sizeof(char));

  table_dominoes = calloc(1, sizeof(struct Domino));
  user_dominoes = calloc(1, sizeof(struct Domino));
//  last_command_feedback = calloc(100, sizeof(char));
//  universe_dominoes = calloc(1, sizeof(struct Domino));
}

/* ********************* TEST UTILS ********************* */
void empty_user_dominoes() {
  user_dominoes = calloc(1, sizeof(struct Domino));
  user_dominoes_size = 0;
}

void empty_table_dominoes() {
  table_dominoes = calloc(1, sizeof(struct Domino));
  table_dominoes_size = 0;
}

void set_user_dominoes(const struct Domino *arr, const int arr_size) {
  user_dominoes = malloc(sizeof(struct Domino) * arr_size);
  user_dominoes_size = arr_size;

  user_dominoes = memcpy(user_dominoes, arr, sizeof(struct Domino) * arr_size);
}

struct Domino *get_user_dominoes(void) {
  return user_dominoes;
}

int get_user_dominoes_size(void) {
  return user_dominoes_size;
}

void set_table_dominoes_original(const struct Domino *arr, const int arr_size) {
  table_dominoes_original = malloc(sizeof(struct Domino) * arr_size);
  table_dominoes_size = arr_size; // TODO is this an issue?

  table_dominoes_original = memcpy(table_dominoes_original, arr, sizeof(struct Domino) * arr_size);
}

void set_table_dominoes(const struct Domino *arr, const int arr_size) {
  table_dominoes = malloc(sizeof(struct Domino) * arr_size);
  table_dominoes_size = arr_size; // TODO is this an issue?

  table_dominoes = memcpy(table_dominoes, arr, sizeof(struct Domino) * arr_size);
}

struct Domino *get_table_dominoes(void) {
  return table_dominoes;
}

int get_table_dominoes_size(void) {
  return table_dominoes_size;
}

void set_last_command(char *new_last_command) {
  last_command = calloc(strlen(new_last_command), sizeof(char));

  last_command = new_last_command;
}

char *get_last_command(void) {
  return last_command;
}

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