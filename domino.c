// 902835
// Source is available at
// https://github.com/kirpachov/c-domino-unive

#include "domino.h"

const char *LEFT_IDENTIFIERS[] = {" LEFT ", " L "};
const unsigned LEFT_IDENTIFIERS_COUNT = 2;

const char *RIGHT_IDENTIFIERS[] = {" RIGHT ", " R "};
const unsigned RIGHT_IDENTIFIERS_COUNT = 2;

void resize_dominoes_array(struct Domino **arr, const int new_size) {
  if (new_size <= 0) {
//    log_error("error! Tried to allocate memory with size %d. Function resize_dominoes_array(arr: %p, new_size: %d)",
//              new_size, arr, new_size);
    return;
  }

  *arr = realloc(*arr, sizeof(struct Domino) * new_size);
}

void push(const struct Domino domino, struct Domino *domino_arr, int *domino_arr_size) {
  domino_arr[*domino_arr_size] = domino;
  (*domino_arr_size)++;
}

void unshift(struct Domino domino, struct Domino *domino_arr, int *domino_arr_size) {
  for (int i = *domino_arr_size; i > 0; i--) domino_arr[i] = domino_arr[i - 1];

  domino_arr[0] = domino;
  (*domino_arr_size)++;
}

void pop(const int index, struct Domino *domino_arr, int *domino_arr_size) {
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
  pop(index, user_dominoes, &user_dominoes_size);
  resize_dominoes_array(&user_dominoes, user_dominoes_size - 1);
}

/**
 * Dominoes currently on table
 */
struct Domino *table_dominoes;
int table_dominoes_size = 0;

void table_dominoes_push(const struct Domino d, const bool left_side) {
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

int run_challenge_1(void){
  log_debug("***** RUNNING CHALLENGE 1 *****");
  int dominoes_size;
  log_debug("scanf returned %d", scanf("%d", &dominoes_size));
  log_debug("We'll have %d dominoes.", dominoes_size);
  struct Domino* dominoes = calloc(dominoes_size, sizeof(struct Domino));
  for(int i = 0; i < dominoes_size; i++){
    int left;
    int right;

    log_debug("scanf returned %d", scanf("%d", &left));
    log_debug("scanf returned %d", scanf("%d", &right));

    const struct Domino domino = (struct Domino){left, right};
    char* formatted_domino = format_domino(domino);
    log_debug("Adding %s to list of dominoes", formatted_domino);
    free(formatted_domino);

    dominoes[i] = domino;
  }

  log_debug("Completed the acquisition of the dominoes. Got: %s", format_dominoes_for_table(dominoes, dominoes_size));

  printf("%s", process_challenge_1(dominoes, dominoes_size));

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

  log_debug("universe_dominoes_size is %d", universe_dominoes_size);
}

char *format_dominoes_for_table(const struct Domino *dominoes, const int size) {
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

int calc_points_from(const struct Domino *arr, const int arr_size) {
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
int put_on_table(const int index, const bool left_side) {
  const struct Domino selected = user_dominoes[index];
  const char *side_str = left_side ? "left" : "right";
  char *selected_str = format_domino(selected);
  last_command_feedback = calloc(100, sizeof(char));

//  log_debug("put_on_table | index: %d | side: %s | domino: %s", index, side_str, selected_str);

  if (!(can_be_put_on_table(selected, left_side))) {
    sprintf(last_command_feedback, "Domino %s cannot be put on %s side", selected_str, side_str);
    return 0;
  }

  table_dominoes_push(rotate_if_necessary(selected, left_side), left_side);

  user_dominoes_pop(index);

  sprintf(last_command_feedback, "Domino %s has been put on %s side", selected_str, side_str);

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


struct Domino *dominoes_without_element(
    const struct Domino *dominoes,
    const int size,
    const int index_of_element_to_remove) {

  struct Domino *dominoes_without_domino = calloc(size, sizeof(struct Domino));
  int dominoes_without_domino_size = size;
  for (int j = 0; j < size; j++) {
    dominoes_without_domino[j] = dominoes[j];
  }

  pop(index_of_element_to_remove, dominoes_without_domino, &dominoes_without_domino_size);
  return dominoes_without_domino;
}

bool dominoes_equal(const struct Domino a, const struct Domino b) {
  return (a.left == b.left && a.right == b.right) || (a.right == b.left && a.left == b.right);
}

int domino_index_in(const struct Domino *arr, const int arr_size, const struct Domino to_find) {
  for (int i = 0; i < arr_size; i++) {
    const struct Domino domino = arr[i];
    if (dominoes_equal(domino, to_find)) return i;
  }

  return -1;
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
 * @param best_table_possible where the best solution will be stored.
 * @param best_table_possible_size size of the best solution array.
 * @return
 */
int scenario_with(
    const struct Domino *user_arr,
    const int user_arr_size,
    const struct Domino *table_arr,
    const int table_arr_size,
    struct Domino **best_table_possible,
    int *best_table_possible_size
) {


//  initialize();

  set_user_dominoes(user_arr, user_arr_size);
  set_table_dominoes(table_arr, table_arr_size);

  int current_points = calc_user_points();

//  log_debug("scenario_with | user_arr(%d): %s | table_arr(%d): %s | current_points: %d | valid_moves(%d): %s",
//            user_arr_size,
//            format_dominoes_for_table(user_arr, user_arr_size),
//            table_arr_size,
//            format_dominoes_for_table(table_arr, table_arr_size),
//            current_points,
//            valid_moves_count(),
//            format_dominoes_for_table(valid_moves(), valid_moves_count())
//  );

  if (valid_moves_count() == 0) {
//    log_debug("Found solution: table is %s | points are: %d\n\n",
//              format_dominoes_for_table(table_arr, table_arr_size), current_points);
//    const int current_best_table_points = ;
    if (calc_points_from(*best_table_possible, *best_table_possible_size) < current_points) {
      *best_table_possible = malloc(sizeof(struct Domino) * table_arr_size);
      *best_table_possible_size = table_arr_size;

       memcpy(*best_table_possible, table_arr, sizeof(struct Domino) * table_arr_size);
    }

    return current_points;
  }

  int max_sum = current_points;

  for (int i = 0; i < user_dominoes_size; i++) {
    if (!can_place_on_right(user_dominoes[i])) continue;

    set_user_dominoes(user_arr, user_arr_size);
    set_table_dominoes(table_arr, table_arr_size);

    put_on_table(i, false);

    int tmp_sum = scenario_with(
        user_dominoes,
        user_dominoes_size,
        table_dominoes,
        table_dominoes_size,
        best_table_possible,
        best_table_possible_size
    );

    if (tmp_sum > max_sum) max_sum = tmp_sum;
  }

  return max_sum;
}

/**
 * Function best_scenario is used to find which are the best moves to do to get the maximum possible points.
 * @param dominoes universe of dominoes
 * @param dominoes_size size of the universe of dominoes
 * @param best_table_possible memory where to store the best combination possible, as array of Dominoes. Already ordered.
 * @param best_table_possible_size size of the array with the best combination possible.
 * @return max points you can get with that dominoes.
 */
int best_scenario(const struct Domino *dominoes, const int dominoes_size, struct Domino **best_table_possible,
                  int *best_table_possible_size) {
  int best = dominoes[0].left + dominoes[0].right;

  for (int i = 0; i < dominoes_size; i++) {
    const struct Domino domino = dominoes[i];
    const struct Domino *dominoes_without_current = dominoes_without_element(dominoes, dominoes_size, i);
    const int current_domino_best_scenario = scenario_with(
        dominoes_without_current,
        dominoes_size - 1,
        (struct Domino[1]) {domino},
        1,
        best_table_possible,
        best_table_possible_size
    );

    if (current_domino_best_scenario > best) { best = current_domino_best_scenario; }

    if (dominoes_size - *best_table_possible_size <= 0) {
      log_debug(
          "Exiting for since all of the dominoes have been used. If they all have been used, we cannot do better. We would find only different combinations, but same points.");
      break;
    }
  }

  log_debug(
      "Best scenario: universe is %s | best scenario is: %s | points: %d | not-included dominoes count: %d",
      format_dominoes_for_table(dominoes, dominoes_size),
      format_dominoes_for_table(*best_table_possible, *best_table_possible_size),
      best,
      dominoes_size - *best_table_possible_size
  );

  return best;
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
  struct Domino *best_table = calloc(100, sizeof(struct Domino));
  if (best_table == NULL) {
    log_error("Failed to allocate memory for best_table at line %d", __LINE__);
    exit(EXIT_FAILURE);
  }

  int best_table_size = 1;
  int best_points = best_scenario(dominoes, dominoes_size, &best_table, &best_table_size);

  char *commands = format_dominoes_as_commands(best_table, best_table_size);
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
    }else{
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