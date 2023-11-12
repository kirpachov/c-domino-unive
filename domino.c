// 902835
// Source is available at
// https://github.com/kirpachov/c-domino-unive

#include "domino.h"

const char *LEFT_IDENTIFIERS[] = {" LEFT ", " L "};
const unsigned LEFT_IDENTIFIERS_COUNT = 2;

const char *RIGHT_IDENTIFIERS[] = {" RIGHT ", " R "};
const unsigned RIGHT_IDENTIFIERS_COUNT = 2;

void resize_dominoes_array(struct Domino **arr, const int new_size) {
  *arr = realloc(*arr, sizeof(struct Domino) * new_size);
}

void push(const struct Domino domino, struct Domino *domino_arr, int *domino_arr_size) {
  domino_arr[*domino_arr_size] = domino;
  (*domino_arr_size)++;
}

void unshift(struct Domino domino, struct Domino *domino_arr, int *domino_arr_size) {
  char* array_str = format_dominoes_for_table(domino_arr, *domino_arr_size);
  log_debug("Writing into array %s | domino: %s | domino_arr_size: %d", array_str, format_domino(domino),
            *domino_arr_size);

  for (int i = *domino_arr_size; i > 0; i--) {
    log_debug("unshift | domino_arr[%d]: %s | domino_arr[%d - 1]: %s", i, format_domino(domino_arr[i]), i, format_domino(domino_arr[i]));
    domino_arr[i] = domino_arr[i - 1];
  }

  domino_arr[0] = domino;
  (*domino_arr_size)++;
}

void pop(int index, struct Domino *domino_arr, int *domino_arr_size) {
  for (int i = index; i < *domino_arr_size - 1; i++) domino_arr[i] = domino_arr[i + 1];
  (*domino_arr_size)--;
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

  push(d, user_dominoes, &user_dominoes_size);
}

void user_dominoes_pop(const int index) {
  log_debug("user_dominoes_pop | index: %d", index);
  pop(index, user_dominoes, &user_dominoes_size);
}

/**
 * Dominoes currently on table
 */
struct Domino *table_dominoes;
int table_dominoes_size = 0;

void table_dominoes_push(const struct Domino d, const bool left_side) {
  log_debug("table_dominoes_push | domino: %s | side: \"%s\" | table_dominoes_size: %d", format_domino(d), left_side ? "left" : "right", table_dominoes_size);

  resize_dominoes_array(&table_dominoes, table_dominoes_size + 1);

  if (left_side) unshift(d, table_dominoes, &table_dominoes_size);
  else push(d, table_dominoes, &table_dominoes_size);
}

/**
 * All available dominoes
 */
struct Domino *universe_dominoes;
int universe_dominoes_size = 0;

void universe_dominoes_push(const struct Domino d) {
  push(d, universe_dominoes, &universe_dominoes_size);
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

bool is_char_a_number(const char ch) {
  return ((int) ch >= 48) && ((int) ch <= 57);
}

/**
 * Transforms a string with numbers to array of numbers.
 * Separator between numbers can be literally anything than a number.
 * @param str "10 20 30"
 * @param integers {10, 20, 30}
 * @return void
 *
 * How it works:
 * It iterates param str and when it finds a number char,
 * it writes the value of the char converted into a integer inside the param integers
 * at the `integer_index` value. When it finds multiple chars that are numbers consecutively,
 * it multiplies the value of the current value of that index of our array of numbers `integers` by 10 and
 * then it adds the value of the current char converted in integer.
 * Then, when it fids a non-number char, it increases the value of `integer_index`.
 */
void str_to_int_array(const char *str, int *integers) {
  const int str_length = (int) strlen(str);
  int integer_index = 0;
  int last_was_integer = false;
  for (int i = 0; i < str_length + 1; i++) {
    const bool is_last_run = i == str_length;
    const char ch = is_last_run ? ' ' : str[i];
    const bool is_number = is_char_a_number(ch);

    if (is_number) {
      int integer = char_to_int(ch);
      if (last_was_integer) integers[integer_index] = integers[integer_index] * 10 + integer;
      else integers[integer_index] = integer;

      last_was_integer = true;
    } else {
      if (last_was_integer) integer_index++;
      last_was_integer = false;
    }
  }
}

char *upcase_str(const char *original) {
  char *result = calloc(strlen(original), sizeof(char));
  for (unsigned i = 0; i < strlen(original); i++)
    result[i] = (char) toupper((int) original[i]);

  return result;
}

bool is_string_in_string(const char *str, const char *to_find) {
  return strstr(str, to_find) != NULL;

  // TODO fix or remove this alg
//  printf("is_string_in_string? | str: %s | to_find: %s | strlen(str): %d | strlen(to_find) : %d\n", str, to_find, (int) strlen(str), (int) strlen(to_find));
//
//  if (strcmp(str, to_find) || strcmp(to_find, "") || (strcmp("", str))) return true;
//
//  // Something smaller cannot contain something bigger
//  if (strlen(to_find) > strlen(str)) return false;
//
//  unsigned str_index = 0;
//  while (str_index < strlen(str)) {
//    unsigned to_find_index = 0;
//    while (to_find_index < strlen(to_find)) {
//      const char s = str[str_index];
//      const char f = to_find[to_find_index];
//
//      printf("str[%d]: %c | to_find[%d]: %c | ", str_index, s, to_find_index, f);
//      printf("s != f : %d | ", s != f);
//      printf("\n");
//
//      if (s != f) break;
//
//      to_find_index++;
//      str_index++;
//
//      if (to_find_index >= strlen(to_find) - 1) return true;
//    }
//
//    printf("Increasing str_index from %d\n", str_index);
//    str_index++;
//  }
//
//  return false;
}

int first_number_from_string(const char *string) {
  int *int_arr = malloc(sizeof(int) * strlen(string));
  str_to_int_array(string, int_arr);
  int result = int_arr[0];
  free(int_arr);
  return result;
}

/**
 * Can domino be placed on left side on the table?
 * @param d Domino
 * @return bool
 */
bool can_place_on_left(const struct Domino d) {
  if (table_dominoes_size <= 0) return true;

  return table_dominoes[0].left == d.left || table_dominoes[0].left == d.right;
}

/**
 * Can domino be placed on right side of the table?
 * @param d Domino
 * @return bool
 */
bool can_place_on_right(const struct Domino d) {
  if (table_dominoes_size <= 0) return true;

  const struct Domino table = table_dominoes[table_dominoes_size - 1];

  return table.right == d.left || table.right == d.right;
}

/**
 * Can this domino be placed anywhere on the table?
 * @param d
 * @return bool
 */
bool can_place_on_table(const struct Domino d) {
  return can_place_on_left(d) || can_place_on_right(d);
}

int valid_moves_count(void) {
  if (user_dominoes_size <= 0) return 0;

  int count = 0;
  for (int i = 0; i < user_dominoes_size; i++) {
    if (can_place_on_table(user_dominoes[i])) count++;
  }

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
  sprintf(str, "[%d|%d]", d.left, d.right);
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

int run_challenge(void) {
  log_info("Running in challenge mode.");
  return 0;
}

void populate_universe_dominoes(void) {
  int final_size = 0; // TODO calc this value without any for loop.
  for (int i = SMALLES_DOMINO_VALUE; i <= LARGEST_DOMINO_VALUE; i++)
    for (int j = i; j <= LARGEST_DOMINO_VALUE; j++)
      final_size++;

  universe_dominoes = malloc(sizeof(struct Domino) * final_size);

  universe_dominoes_size = 0;
  for (int i = SMALLES_DOMINO_VALUE; i <= LARGEST_DOMINO_VALUE; i++) {
    for (int j = i; j <= LARGEST_DOMINO_VALUE; j++) {
      const struct Domino d = {i, j};
      log_debug("Adding %s to universe.", format_domino(d));
      universe_dominoes_push(d);
    }
  }

  log_debug("universe_dominoes_size is %d", universe_dominoes_size);
}

char *format_dominoes_for_table(const struct Domino *dominoes, const int size) {
  char *result = calloc(100, sizeof(char));

  for (int i = 0; i < size; i++) {
    strcat(result, format_domino(dominoes[i]));

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
    strcat(result, format_domino(d));

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

void print_last_command_feedback(void) {
  // create some space
  printf("\n");

  if (strlen(last_command) == 0 || is_help_command(last_command)) {
    printf("Welcome!\n"
           "Select the domino to place by specifying the domino's index and the side (Right/Left) where to place it.\n"
           "Example: \"1L\" will place the first domino on the table, to the left of the other dominos\n");
    return;
  }

  printf("%s\n", last_command_feedback);
}

void clean_screen(void) {
  // printf("\e[1;1H\e[2J"); // non-ISO-standard escape sequence
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void print_everything() {
  clean_screen();
  print_table();
  print_user_dominoes();
  print_last_command_feedback();
}

void assign_user_random_dominoes(void) {
  while (user_dominoes_size < total_user_dominoes_valid()) {
    const struct Domino d = random_domino_from_universe();
    log_debug("Adding %s to user's hand", format_domino(d));
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
int put_on_table(const int index, const bool left_side) {
  const struct Domino selected = user_dominoes[index];
  const char *side_str = left_side ? "left" : "right";
  const char *selected_str = format_domino(selected);
  last_command_feedback = calloc(100, sizeof(char));

  log_debug("put_on_table | index: %d | side: %s | domino: %s", index, side_str, selected_str);

  if (!(can_be_put_on_table(selected, left_side))) {
    sprintf(last_command_feedback, "Domino %s cannot be put on %s side", selected_str, side_str);
    return 0;
  }

  table_dominoes_push(rotate_if_necessary(selected, left_side), left_side);

  user_dominoes_pop(index);

  sprintf(last_command_feedback, "Domino %s has been put on %s side", selected_str, side_str);
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
   * - if user has invalid position or index, popoulate "last_command_feedback" with the error messages.
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

void run_terminal(void) {
  while (user_dominoes_size > 0 && valid_moves_count() > 0) {
    acquire_command();
    process_last_command();
    print_everything();
  }
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
    if (strcmp(argv[i], "--challenge") != 0) is_challenge = true;

    log_debug("argv[%d]: %s", i, argv[i]);
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

void set_table_dominoes(const struct Domino *arr, const int arr_size) {
  table_dominoes = malloc(sizeof(struct Domino) * arr_size);
  table_dominoes_size = arr_size;

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