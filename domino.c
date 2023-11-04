// 902835

#include "domino.h"

const char* LEFT_IDENTIFIERS[] = {"LEFT"," L "};
const unsigned LEFT_IDENTIFIERS_COUNT = 2;

const char* RIGHT_IDENTIFIERS[] = {"RIGHT", " R "};
const unsigned RIGHT_IDENTIFIERS_COUNT = 2;


/**
 * Smallest and biggest value domino cards can contain.
 * These values are used to populate the universe of dominoes.
 * [ 1 | 6 ]
 */


int total_user_dominoes = 0;  // TODO --hand=X | -c=X (??)

/**
 * Dominoes owned by user
 */
struct Domino *user_dominoes;
int user_dominoes_size = 0;

void user_dominoes_push(const struct Domino d) {
  user_dominoes[user_dominoes_size] = d;
  user_dominoes_size++;
}

/**
 * Dominoes currently on table
 */
struct Domino *table_dominoes;
int table_dominoes_size = 0;

void table_dominoes_push(const struct Domino d) {
  table_dominoes[table_dominoes_size] = d;
  table_dominoes_size++;
}

/**
 * All available dominoes
 */
struct Domino *universe_dominoes;
int universe_dominoes_size = 0;

void universe_dominoes_push(const struct Domino d) {
  universe_dominoes[universe_dominoes_size] = d;
  universe_dominoes_size++;
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

char *upcase_str(const char *str, char *result) {
  for (unsigned i = 0; i < strlen(str); i++)
    result[i] = (char) toupper((int) str[i]);

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
 * Is this domino a valid move?
 * @input const struct Domino d
 * @return:
 * 0 if cannot be placed;
 * 1 if can be put on left side;
 * 2 if can be put on right side;
 * 3 if can be put on both sides;
 */
int valid_move(const struct Domino d) {
  if (table_dominoes_size == 0) return 3;
  int sum = 0;

  // Can be put on left side
  if (table_dominoes[0].left == d.right || table_dominoes[0].left == d.left) sum += 1;
  if (table_dominoes[table_dominoes_size].right == d.left || table_dominoes[table_dominoes_size].right == d.right)
    sum += 2;

  return sum;
}

int valid_moves_count(void) {
  if (user_dominoes_size <= 0) return 0;

  int count = 0;
  for (int i = 0; i < user_dominoes_size; i++) {
    if (valid_move(user_dominoes[i])) count++;
  }

  return count;
}

void str_add_padding(const char* str, char* result){
  result[0] = ' ';
  result[strlen(str)] = ' ';

  for(unsigned i = 1; i <= strlen(str); i++) result[i] = str[i];
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

//  printf("final_size: %d\n", final_size);
  universe_dominoes = malloc(sizeof(struct Domino) * final_size);

  universe_dominoes_size = 0;
  for (int i = SMALLES_DOMINO_VALUE; i <= LARGEST_DOMINO_VALUE; i++) {
    for (int j = i; j <= LARGEST_DOMINO_VALUE; j++) {
      log_debug("Adding [%d|%d] to universe.", i, j);
      universe_dominoes_push((struct Domino) {i, j});
    }
  }

  log_debug("universe_dominoes_size is %d", universe_dominoes_size);
}

void print_domino(const struct Domino d) {
  printf("[ %d | %d ]", d.left, d.right);
}

void print_table(void) {
  if (table_dominoes_size == 0) {
    printf("<Table is empty>\n\n");
    return;
  }

  for (int i = 0; i < table_dominoes_size; i++) {
    print_domino(table_dominoes[i]);
    printf(" ");
  }

  printf("\n");
}

void print_user_dominoes(void) {
  printf("Your dominoes:\n");
  for (int i = 0; i < user_dominoes_size; i++) {
    const struct Domino d = user_dominoes[i];
    const int valid = valid_move(d);
//    log_debug("valid_move [%d|%d]: %d", d.left, d.right, valid);

    if (valid == 0) printf("-");
    else printf("%d) ", i + 1);

    if (valid == 1) printf("(L) ");
    if (valid == 2) printf("(R) ");
    if (valid == 3) printf("(L / R) ");

    print_domino(d);
    printf("\n");
  }
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
  printf("\n\n\n\n\n\n\n\n\n");
}

void print_everything() {
  clean_screen();
  print_table();
  print_user_dominoes();
  print_last_command_feedback();
}

void assign_user_random_dominoes(void) {
  int size = total_user_dominoes;
  if (size <= 0) size = TOTAL_USER_DOMINOES;

  user_dominoes = malloc(sizeof(struct Domino) * size);

  while (user_dominoes_size < size) {
    const struct Domino d = random_domino_from_universe();
    log_debug("Adding [%d|%d] to user's hand", d.left, d.right);
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
void guess_selection(const char* command, int *result_index, int *position) {
  *result_index = first_number_from_string(command);

  if (*result_index < 0 || *result_index > (int)strlen(command) * 10) *result_index = 0;

  char *upcase_tmp = malloc(sizeof(char) * strlen(command));

  *position = 0;

  for (unsigned i = 0; i < LEFT_IDENTIFIERS_COUNT; i++) {
    if (is_string_in_string(upcase_str(command, upcase_tmp), LEFT_IDENTIFIERS[i])) {
       *position = 1;
      return;
    }
  }

  for (unsigned i = 0; i < RIGHT_IDENTIFIERS_COUNT; i++) {
    if (is_string_in_string(upcase_str(command, upcase_tmp), RIGHT_IDENTIFIERS[i])) {
       *position = 2;
      return;
    }
  }

  free(upcase_tmp);
}

void process_last_command(void) {
  log_debug("processing command \"%s\"", last_command);
  if (is_help_command(last_command)) return;


//  int selected_domino_index = -1;
//
//  selected_domino_index = first_number_from_string(last_command);
//  for (int i = 0; i < LEFT_IDENTIFIERS_COUNT; i++) {
//    if (is_string_in_string(upcase_str(last_command, upcase_tmp), LEFT_IDENTIFIERS[i])) selected_domino_index = 'L';
//
//    if (is_string_in_string(upcase_str(last_command, upcase_tmp), RIGHT_IDENTIFIERS[i])) selected_domino_index = 'R';
//  }



//  for (int i = 0; i < user_dominoes_size; i++) {
//    char *domino_command = malloc(sizeof(char) * 15);
//    const struct Domino d = user_dominoes[i];
//    sprintf(domino_command, "%d %c", i + 1, 'l');
//    log_debug("domino_command [%d|%d]: \"%s\"", d.left, d.right, domino_command);
//    log_debug("\"%s\" == \"%s\": %d", last_command, domino_command, strcmp(last_command, domino_command) == 0);
//  }

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

//void acquire_command() {
//  char tmp_command[MAX_COMMAND_SIZE];
//  int i, N = 0;
//  char ch;
//
//  printf("$ ");
//  ch = getchar();
//
//  while (ch != '\n' && N < MAX_COMMAND_SIZE) {
//    tmp_command[N++] = ch;
//    ch = getchar();
//  }
//
////  for (i = 0; i < N; i++)
////    putchar(tmp_command[i]);
//
//  free(last_command);
//  last_command = malloc(sizeof(char) * N);
//
//  if (last_command == NULL) {
//    log_error("Realloc failed.");
//    exit(EXIT_FAILURE);
//  }
//
//  for (int j = 0; j < MAX_COMMAND_SIZE; j++) {
//    tmp_command[j] = j < N ? tmp_command[j] : ' ';
//    if (j < N) last_command[j] = tmp_command[j];
//  }
//
//  log_debug("tmp_command: %s | size: %d", tmp_command, N);
//  log_debug("last_command: %s", last_command);
//}

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
}

void complete(void) {
//  free(last_command);
}

/**
* MAIN FUNCTION IS INSIDE src/main.c
 * for testing purposes.
 * Run this file with
 * $ make run
*/