#include "domino.h"
#include "test-framework/unity.h"

void setUp(void) {
  initialize();
}

void tearDown(void) {
  empty_table_dominoes();
  empty_user_dominoes();
  set_last_command("");
}

static void test_first_number_from_string(void) {
  TEST_ASSERT_EQUAL(first_number_from_string("10 banana"), 10);
  TEST_ASSERT_EQUAL(first_number_from_string("1 banana"), 1);
  TEST_ASSERT_EQUAL(first_number_from_string("1"), 1);
  TEST_ASSERT_EQUAL(first_number_from_string("100"), 100);
  TEST_ASSERT_EQUAL(first_number_from_string("102"), 102);
  TEST_ASSERT_EQUAL(first_number_from_string("gang 102"), 102);
  TEST_ASSERT_EQUAL(first_number_from_string("banana gang 102"), 102);
  TEST_ASSERT_EQUAL(first_number_from_string("banana \n gang 102"), 102);
  TEST_ASSERT_EQUAL(first_number_from_string("banana \n gang 1"), 1);
  TEST_ASSERT_EQUAL(first_number_from_string("10 20 30"), 10);
  TEST_ASSERT_EQUAL(first_number_from_string("wassa 20 30"), 20);
  TEST_ASSERT_EQUAL(first_number_from_string("lorem ipsum 20 si dolor"), 20);
  TEST_ASSERT_EQUAL(first_number_from_string("1 2 3 4"), 1);
}

static void test_is_string_in_string(void) {
  UNITY_TEST_ASSERT(is_string_in_string("ciao", "ao"), __LINE__, "String \"ciao\" should contain \"ao\"");
  UNITY_TEST_ASSERT(is_string_in_string("ciao", "ciao"), __LINE__, "String \"ciao\" should contain \"ciao\".");
  UNITY_TEST_ASSERT(is_string_in_string("ciao", "c"), __LINE__, "String \"ciao\" should contain \"c\".");
  UNITY_TEST_ASSERT(is_string_in_string("ciao", "i"), __LINE__, "String \"ciao\" should contain \"i\".");
  UNITY_TEST_ASSERT(is_string_in_string("ciao", "a"), __LINE__, "String \"ciao\" should contain \"a\".");
  UNITY_TEST_ASSERT(is_string_in_string("ciao", "o"), __LINE__, "String \"ciao\" should contain \"o\".");
  UNITY_TEST_ASSERT(is_string_in_string("ciao", ""), __LINE__, "String \"ciao\" should contain \"\".");
  UNITY_TEST_ASSERT(is_string_in_string("", ""), __LINE__, "String \"\" should contain \"\".");

  UNITY_TEST_ASSERT(is_string_in_string("ciao", "1") == 0, __LINE__, "String \"ciao\" should not contain \"1\"");
  UNITY_TEST_ASSERT(is_string_in_string("ciao", "ciao1") == 0, __LINE__,
                    "String \"ciao\" should not contain \"ciao1\"");
  UNITY_TEST_ASSERT(is_string_in_string("ciao", "ao1") == 0, __LINE__, "String \"ciao\" should not contain \"ao1\"");
}

static void test_upcase_str(void) {
  char *str;

  str = "ciao";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str), "CIAO", strlen(str));

  str = "banana";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str), "BANANA", strlen(str));

  str = "1l";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str), "1L", strlen(str));

  str = "1 l";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str), "1 L", strlen(str));

  str = "1 left";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str), "1 LEFT", strlen(str));

  str = "1 right";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str), "1 RIGHT", strlen(str));

  str = "1 RIGHT";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str), "1 RIGHT", strlen(str));

  str = "1 LEFT";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str), "1 LEFT", strlen(str));

  str = "1 L";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str), "1 L", strlen(str));
}

static void test_guess_selection(void) {
  int index;
  int position;

  /**
   * LEFT
   */
  guess_selection("5 l", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(1, position);

  guess_selection("5 LEFT", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(1, position);

  guess_selection("L5", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(1, position);

  guess_selection("5L", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(1, position);

  guess_selection("5l", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(1, position);

  guess_selection("l5", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(1, position);

  /**
   * RIGHT
   */
  guess_selection("20 right", &index, &position);
  TEST_ASSERT_EQUAL(20, index);
  TEST_ASSERT_EQUAL(2, position);

  guess_selection("2 right", &index, &position);
  TEST_ASSERT_EQUAL(2, index);
  TEST_ASSERT_EQUAL(2, position);

  guess_selection("2 r", &index, &position);
  TEST_ASSERT_EQUAL(2, index);
  TEST_ASSERT_EQUAL(2, position);

  guess_selection("5 RIGHT", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(2, position);

  guess_selection("5r", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(2, position);

  guess_selection("5R", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(2, position);

  guess_selection("R5", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(2, position);

  guess_selection("r5", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(2, position);

  /**
   * Invalid positions
   */
  guess_selection("15", &index, &position);
  TEST_ASSERT_EQUAL(15, index);
  TEST_ASSERT_EQUAL(0, position);

  guess_selection("5 position", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(0, position);

  guess_selection("5 start", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(0, position);

  guess_selection("5l a", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(0, position);

  guess_selection("l5a", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(0, position);

  guess_selection("l5 bubi", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(0, position);
}

static void test_str_add_padding(void) {
  char *str;

  str = "BANANA";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(str_add_padding(str), " BANANA ", 8);
  TEST_ASSERT_EQUAL(strlen(str_add_padding(str)), 8);

  str = "1";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(str_add_padding(str), " 1 ", 3);
  TEST_ASSERT_EQUAL(strlen(str_add_padding(str)), 3);

  str = "1l";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(str_add_padding(str), " 1l ", 4);
  TEST_ASSERT_EQUAL(strlen(str_add_padding(str)), 4);

  str = " 1l";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(str_add_padding(str), "  1l ", 5);
  TEST_ASSERT_EQUAL(strlen(str_add_padding(str)), 5);

  str = " 1L ";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(str_add_padding(str), "  1L  ", 6);
  TEST_ASSERT_EQUAL(strlen(str_add_padding(str)), 6);
}

static void test_rotate_domino(void) {
  TEST_ASSERT_EQUAL(rotate_domino((struct Domino) {1, 2}).right, 1);
  TEST_ASSERT_EQUAL(rotate_domino((struct Domino) {1, 2}).left, 2);

  TEST_ASSERT_EQUAL(rotate_domino((struct Domino) {1, 1}).left, 1);
  TEST_ASSERT_EQUAL(rotate_domino((struct Domino) {1, 1}).right, 1);

  TEST_ASSERT_EQUAL(rotate_domino((struct Domino) {4, 5}).right, 4);
  TEST_ASSERT_EQUAL(rotate_domino((struct Domino) {4, 5}).left, 5);
}

static void test_get_and_set_user_dominoes(void) {
  set_user_dominoes((struct Domino[2]) {{1, 2},
                                        {3, 4}}, 2);

  struct Domino *dominoes = get_user_dominoes();
  TEST_ASSERT_EQUAL(dominoes[0].left, 1);
  TEST_ASSERT_EQUAL(dominoes[0].right, 2);
  TEST_ASSERT_EQUAL(dominoes[1].left, 3);
  TEST_ASSERT_EQUAL(dominoes[1].right, 4);
}

static void test_needs_to_be_rotated_before_putting_on_table(void) {
  set_table_dominoes((struct Domino[2]) {{1, 2},
                                         {3, 4}}, 2);

  // Table is [1|2] [3|4].
  // Putting [2|1] on the left should be ok, because:
  // [2|1] <-> [1|2] [3|4] ==> OK!
  TEST_ASSERT_FALSE(needs_to_be_rotated_before_putting_on_table((struct Domino) {2, 1}, true));
  // Putting [1|2] should require rotation, because:
  // [1|2] <-> [1|2] [3|4] ==> NOT OK!
  TEST_ASSERT_TRUE(needs_to_be_rotated_before_putting_on_table((struct Domino) {1, 2}, true));

  TEST_ASSERT_TRUE(needs_to_be_rotated_before_putting_on_table((struct Domino) {3, 4}, false));
  TEST_ASSERT_FALSE(needs_to_be_rotated_before_putting_on_table((struct Domino) {4, 3}, false));
}

static void test_table_dominoes_push(void) {
  set_table_dominoes((struct Domino[1]) {
      {1, 1},
  }, 1);

  struct Domino *user_dominoes = (struct Domino[2]) {
      {2, 2},
      {2, 2},
  };

  set_user_dominoes(user_dominoes, 2);

  table_dominoes_push((struct Domino) {3, 3}, true);

  struct Domino *current_table_dominoes = (struct Domino[2]) {
      {3, 3},
      {1, 1},
  };

  TEST_ASSERT_EQUAL_CHAR_ARRAY(format_dominoes_for_table(current_table_dominoes, 2), format_table_dominoes(),
                               strlen(format_dominoes_for_table(current_table_dominoes, 2)));

  TEST_ASSERT_EQUAL_CHAR_ARRAY(format_dominoes_with_valid_moves(user_dominoes, 2), format_user_dominoes(),
                               strlen(format_dominoes_with_valid_moves(user_dominoes, 2)));

  TEST_ASSERT_EQUAL(2, get_table_dominoes_size());
  TEST_ASSERT_EQUAL(2, get_user_dominoes_size());
}

static void test_interactive_0(void) {
  const bool verbose = false;

  /**
   * Initial situation:
   * Table:
   * [2|6] [6|1] [1|4] [4|6] [1|6]
   *
   * Your dominoes:
   * [6|1]  (1R)
   * [4|4]
   * [4|4]
   * [2|5]  (4L)
   * [2|4]  (5L)
   *
   */
  set_table_dominoes((struct Domino[5]) {
      {2, 6},
      {6, 1},
      {1, 4},
      {4, 6},
      {1, 6},
  }, 5);

  set_user_dominoes((struct Domino[5]) {
      {6, 1},
      {4, 4},
      {4, 4},
      {2, 5},
      {2, 4},
  }, 5);

  /**
   * situation after "1r"
   *
   * Table:
   * [2|6] [6|1] [1|4] [4|6] [1|6] [6|1]
   *
   * Your dominoes:
   * [4|4]
   * [4|4]
   * [2|5]  (3L)
   * [2|4]  (4L)
   *
   */

  if (verbose) print_everything();
  set_last_command("1 r");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  struct Domino *domino_arr = (struct Domino[4]) {{4, 4},
                                                  {4, 4},
                                                  {2, 5},
                                                  {2, 4}};
  TEST_ASSERT_EQUAL_CHAR_ARRAY(format_dominoes_with_valid_moves(domino_arr, 4), format_user_dominoes(),
                               strlen(format_dominoes_with_valid_moves(domino_arr, 4)));

  domino_arr = (struct Domino[6]) {{2, 6},
                                   {6, 1},
                                   {1, 4},
                                   {4, 6},
                                   {1, 6},
                                   {6, 1}};

  TEST_ASSERT_EQUAL_CHAR_ARRAY(format_dominoes_for_table(domino_arr, 6), format_table_dominoes(),
                               strlen(format_dominoes_for_table(domino_arr, 6)));

  /**
   * UPDATED SITUATION:
   *
   *
   * Table:
   * [2|6] [6|1] [1|4] [4|6] [1|6] [6|1]
   *
   * Your dominoes:
   * 1) [4|4]
   * 2) [4|4]
   * 3) [2|5] (L)
   * 4) [2|4] (L)
   *
   * Domino [6|1] has been put on right side
   *
   */

  set_last_command("3 l");

  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  if (verbose) log_debug("AFTER 3l\n\n\n");
  if (verbose) printf("AFTER 3l \n\n\n");
  process_last_command();
  if (verbose) print_everything();
  TEST_ASSERT_EQUAL(7, get_table_dominoes_size());
  TEST_ASSERT_EQUAL(3, get_user_dominoes_size());

  domino_arr = (struct Domino[]) {{5, 2},
                                  {2, 6},
                                  {6, 1},
                                  {1, 4},
                                  {4, 6},
                                  {1, 6},
                                  {6, 1}};

  TEST_ASSERT_EQUAL_CHAR_ARRAY(format_dominoes_for_table(domino_arr, 7), format_table_dominoes(),
                               strlen(format_dominoes_for_table(domino_arr, 7)));

  domino_arr = (struct Domino[]) {{4, 4},
                                  {4, 4},
                                  {2, 4}};

  TEST_ASSERT_EQUAL_CHAR_ARRAY(format_dominoes_with_valid_moves(domino_arr, 3), format_user_dominoes(),
                               strlen(format_dominoes_with_valid_moves(domino_arr, 3)));
}

static void test_best_scenario(void) {
  struct Domino *best_table = calloc(1, sizeof(struct Domino));
  int best_table_size = 1;

  TEST_ASSERT_EQUAL(1 + 3 + 3 + 3 + 6 + 3, best_scenario((struct Domino[]) {
      {1, 3},
      {6, 3},
      {3, 3},
  }, 3, &best_table, &best_table_size));

  best_table = calloc(1, sizeof(struct Domino));
  best_table_size = 1;

  TEST_ASSERT_EQUAL(1 + 3 + 3 + 3 + 6 + 3, best_scenario((struct Domino[]) {
      {1, 3},
      {3, 3},
      {6, 3},
  }, 3, &best_table, &best_table_size));

  best_table = calloc(1, sizeof(struct Domino));
  best_table_size = 1;

  TEST_ASSERT_EQUAL(28, best_scenario((struct Domino[]) {
      {1, 6},
      {3, 6},
      {1, 4},
      {3, 4},
  }, 4, &best_table, &best_table_size));

  best_table = calloc(1, sizeof(struct Domino));
  best_table_size = 1;

  TEST_ASSERT_EQUAL(28, best_scenario((struct Domino[]) {
      {3, 6},
      {1, 6},
      {1, 4},
      {3, 4},
  }, 4, &best_table, &best_table_size));

  best_table = calloc(1, sizeof(struct Domino));
  best_table_size = 1;

  TEST_ASSERT_EQUAL(1 + 3 + 3 + 4 + 4 + 5, best_scenario((struct Domino[]) {{1, 3},
                                                                            {3, 4},
                                                                            {4, 5},
                                                                            {2, 2}}, 4,
                                                         &best_table, &best_table_size));

  best_table = calloc(1, sizeof(struct Domino));
  best_table_size = 1;

  TEST_ASSERT_EQUAL(6 + 6, best_scenario((struct Domino[4]) {{6, 6},
                                                             {1, 1},
                                                             {2, 2},
                                                             {3, 3}}, 4,
                                         &best_table, &best_table_size));

  best_table = calloc(1, sizeof(struct Domino));
  best_table_size = 1;

  TEST_ASSERT_EQUAL(6 + 6 + 6 + 1, best_scenario((struct Domino[4]) {{6, 6},
                                                                     {6, 1},
                                                                     {2, 2},
                                                                     {3, 3}}, 4,
                                                 &best_table, &best_table_size));

  best_table = calloc(1, sizeof(struct Domino));
  best_table_size = 1;

  TEST_ASSERT_EQUAL(57, best_scenario((struct Domino[]) {
      {6, 6},
      {6, 6},
      {6, 6},
      {3, 6},
      {1, 6},
      {2, 6},
      {1, 4},
  }, 7, &best_table, &best_table_size));

  best_table = calloc(1, sizeof(struct Domino));
  best_table_size = 1;

  TEST_ASSERT_EQUAL(60, best_scenario((struct Domino[]) {
      {6, 6},
      {6, 6},
      {6, 6},
      {3, 6},
      {1, 6},
      {2, 6},
      {3, 4},
  }, 7, &best_table, &best_table_size));
}

static void test_format_dominoes_as_commands(void) {
  TEST_ASSERT_EQUAL_STRING(format_dominoes_as_commands((struct Domino[]) {{1, 3},
                                                                          {3, 5},
                                                                          {5, 7},
                                                                          {7, 9}}, 4), "S 1 3 R 3 5 R 5 7 R 7 9");

  TEST_ASSERT_EQUAL_STRING(format_dominoes_as_commands((struct Domino[]) {{1, 3},
                                                                          {3, 4},
                                                                          {4, 5},}, 3), "S 1 3 R 3 4 R 4 5");
}

static void test_format_domino(void) {
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {1, 2}), "[1|2]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {2, 3}), "[2|3]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {10, 20}), "[10|20]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {15, 200}), "[15|200]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {4, 1}), "[4|1]");
}

static void test_process_challenge_1(void) {
  char *challenge;

  challenge = process_challenge_1((struct Domino[]) {
      {6, 6},
      {6, 6},
      {6, 6},
      {3, 6},
      {1, 6},
      {2, 6},
      {1, 4},
      {3, 4},
      {4, 4},
      {4, 5},
  }, 10);
  TEST_ASSERT_EQUAL_STRING("S 5 4 R 4 4 R 4 3 R 3 6 R 6 6 R 6 6 R 6 6 R 6 1 R 1 4", challenge);
  free(challenge);

  challenge = process_challenge_1((struct Domino[]) {
      {1, 2},
      {2, 3}
  }, 2);
  TEST_ASSERT_EQUAL_STRING("S 1 2 R 2 3", challenge);
  free(challenge);

  challenge = process_challenge_1((struct Domino[]) {
      {2, 2},
      {2, 3}
  }, 2);
  TEST_ASSERT_EQUAL_STRING(challenge, "S 3 2 R 2 2");
  free(challenge);

  challenge = process_challenge_1((struct Domino[]) {{1, 1},
                                                     {2, 3}}, 2);
  TEST_ASSERT_EQUAL_STRING(challenge, "S 2 3");
  free(challenge);

  challenge = process_challenge_1((struct Domino[]) {{3, 4},
                                                     {1, 3},
                                                     {4, 5},
                                                     {2, 2}}, 4);
  TEST_ASSERT_EQUAL_STRING(challenge, "S 1 3 R 3 4 R 4 5");
  free(challenge);
}

int main(void) {
  UnityBegin("test_domino.c");

  RUN_TEST(test_process_challenge_1);
//  return 0;
  RUN_TEST(test_best_scenario);
  RUN_TEST(test_format_domino);
  RUN_TEST(test_format_dominoes_as_commands);
  RUN_TEST(test_interactive_0);
  RUN_TEST(test_table_dominoes_push);
  RUN_TEST(test_first_number_from_string);
  RUN_TEST(test_is_string_in_string);
  RUN_TEST(test_upcase_str);
  RUN_TEST(test_guess_selection);
  RUN_TEST(test_str_add_padding);
  RUN_TEST(test_rotate_domino);
  RUN_TEST(test_get_and_set_user_dominoes);
  RUN_TEST(test_needs_to_be_rotated_before_putting_on_table);

  return UnityEnd();
}