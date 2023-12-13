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
  TEST_ASSERT_EQUAL(first_number_from_string("1 2 3"), 1);
  TEST_ASSERT_EQUAL(first_number_from_string("3 2 3"), 3);
  TEST_ASSERT_EQUAL(first_number_from_string("3l"), 3);
  TEST_ASSERT_EQUAL(first_number_from_string("3r"), 3);
  TEST_ASSERT_EQUAL(first_number_from_string("r3"), 3);
  TEST_ASSERT_EQUAL(first_number_from_string("R3"), 3);
  TEST_ASSERT_EQUAL(first_number_from_string("L3"), 3);
  TEST_ASSERT_EQUAL(first_number_from_string("LEFT3"), 3);
  TEST_ASSERT_EQUAL(first_number_from_string("LEFT 3"), 3);
  TEST_ASSERT_EQUAL(first_number_from_string("3LEFT "), 3);
  TEST_ASSERT_EQUAL(first_number_from_string("3 LEFT "), 3);
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

static void test_interactive_1(void) {
  const bool verbose = false;

  set_table_dominoes((struct Domino[]) {
      {1, 1},
  }, 1);

  set_user_dominoes((struct Domino[]) {
      {0, 0},
      {2, 2},
  }, 2);

  TEST_ASSERT_EQUAL(calc_user_points(), 2);

  if (verbose) print_everything();
  set_last_command("1 r");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(calc_user_points(), 2);

  if (verbose) print_everything();
  set_last_command("1 r");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(calc_user_points(), 2 * 3);
}

static void test_interactive_2(void) {
  const bool verbose = false;

  set_table_dominoes((struct Domino[]) {
      {1, 1},
  }, 1);

  set_user_dominoes((struct Domino[]) {
      {11, 11},
      {2,  2},
  }, 2);

  TEST_ASSERT_EQUAL(2, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 r");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(2 * 4, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 l");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(2 * 6, calc_user_points());
}

static void test_interactive_3(void) {
  const bool verbose = false;

  set_table_dominoes((struct Domino[]) {
      {6, 6},
  }, 1);

  set_user_dominoes((struct Domino[]) {
      {11, 11},
      {1,  1},
  }, 2);

  TEST_ASSERT_EQUAL(12, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 r");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(1 * 4, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 r");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(1 * 6, calc_user_points());
}

static void test_interactive_4(void) {
  const bool verbose = false;

  set_table_dominoes((struct Domino[]) {
      {2, 3},
  }, 1);

  set_user_dominoes((struct Domino[]) {
      {12, 21},
      {2,  3},
      {11, 11}
  }, 3);

  TEST_ASSERT_EQUAL(5, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 r");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(2 + 3 + 3 + 2, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 r");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(2 + 3 + 3 + 2 + 2 + 3, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 l");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();
  TEST_ASSERT_EQUAL(3 + 3 + 3 + 4 + 4 + 3 + 3 + 4, calc_user_points());
}

static void test_interactive_5(void) {
  const bool verbose = false;

  set_table_dominoes((struct Domino[]) {
      {2, 3},
  }, 1);

  set_user_dominoes((struct Domino[]) {
      {11, 11},
      {4,  5},
      {11, 11}
  }, 3);

  TEST_ASSERT_EQUAL(5, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 l");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(3 + 3 + 3 + 4, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 r");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(3 + 3 + 3 + 4 + 4 + 5, calc_user_points());

  if (verbose) print_everything();
  set_last_command("1 l");
  if (verbose) printf("<<< COMMAND: \"%s\" >>>\n", get_last_command());
  process_last_command();
  if (verbose) print_everything();

  TEST_ASSERT_EQUAL(4 + 4 + 4 + 4 + 4 + 5 + 5 + 6, calc_user_points());
}

static void test_scenario_with(void) {
  int points;
  struct Domino *best_table = calloc(1, sizeof(struct Domino));
  struct Domino *best_table_original = calloc(1, sizeof(struct Domino));
  int best_table_size = 1;

  points = scenario_with(
      (struct Domino[]) {
          {2, 1},
          {2, 4},
          {2, 3}
      },
      3,
      (struct Domino[]) {{0, 0}},
      0,
      (struct Domino[]) {{0, 0}},
      &best_table,
      &best_table_size,
      &best_table_original
  );

  TEST_ASSERT_EQUAL(3 + 2 + 2 + 4, points);
}

static void shuffle_dominoes(struct Domino *dominoes, const int dominoes_size) {
  for (int i = 0; i < dominoes_size; i++) {
    struct Domino tmp = dominoes[i];
    int random_index = random_between(0, dominoes_size - 1);
    dominoes[i] = dominoes[random_index];
    dominoes[random_index] = tmp;
  }
}

static void
test_best_scenario_case(const int expected_best_points, const struct Domino *dominoes, const int dominoes_size,
                        const int callee_row) {
  struct Domino *best_table_possible = calloc(1, sizeof(struct Domino));
  struct Domino *best_table_possible_original = calloc(1, sizeof(struct Domino));
  int best_table_possible_size = 1;
  const int actual_best_points = best_scenario(dominoes, dominoes_size, &best_table_possible,
                                               &best_table_possible_size, &best_table_possible_original);

  char *error_message = calloc(1000, sizeof(char));

  sprintf(error_message, "Best table: %s | Original dominoes: %s",
          format_dominoes_for_table(best_table_possible, best_table_possible_size),
          format_dominoes_for_table(best_table_possible_original, best_table_possible_size));

  UNITY_TEST_ASSERT_EQUAL_INT((expected_best_points), (actual_best_points), callee_row, error_message);

  free(best_table_possible);
  free(error_message);
}

static void
test_best_scenario_shuffle(const int expected_best_points, const struct Domino *dominoes, const int dominoes_size,
                           const int callee_row) {

//  printf("Dominoes: %s\n", format_dominoes_for_table(dominoes, dominoes_size));

  test_best_scenario_case(expected_best_points, dominoes, dominoes_size, callee_row);

  for (int i = 0; i < 10; i++) {
    struct Domino *dominoes_shuffle = calloc(dominoes_size, sizeof(struct Domino));
    memcpy(dominoes_shuffle, dominoes, dominoes_size * sizeof(struct Domino));
    shuffle_dominoes((struct Domino *) dominoes_shuffle, dominoes_size);

    test_best_scenario_case(expected_best_points, dominoes_shuffle, dominoes_size, callee_row);
    free(dominoes_shuffle);
  }
}

static void test_best_scenario_long(void) {
  test_best_scenario_case(103, (struct Domino[]) {
      {1,  2},
      {12, 21},
      {3,  3},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11}, // should not use one [11|11]
  }, 9, __LINE__);

  test_best_scenario_case(91, (struct Domino[]) {
      {1,  2},
      {12, 21},
      {3,  3},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11},
  }, 8, __LINE__);

  test_best_scenario_case(57, (struct Domino[]) {
      {6, 6},
      {6, 6},
      {6, 6},
      {3, 6},
      {1, 6},
      {2, 6},
      {1, 4},
  }, 7, __LINE__);

  test_best_scenario_case(60, (struct Domino[]) {
      {6, 6},
      {6, 6},
      {6, 6},
      {3, 6},
      {1, 6},
      {2, 6},
      {3, 4},
  }, 7, __LINE__);

  test_best_scenario_case(6 * 10, (struct Domino[]) {
      {12, 21},
      {3,  3},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11}
  }, 6, __LINE__);

  test_best_scenario_case(83, (struct Domino[]) {
      {1,  2},
      {12, 21},
      {3,  3},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11},
  }, 7, __LINE__);

  test_best_scenario_case(6 * 10, (struct Domino[]) {
      {1,  2},
      {12, 21},
      {3,  3},
      {11, 11},
      {11, 11},
      {11, 11}
  }, 6, __LINE__);

  test_best_scenario_case(6 * 10, (struct Domino[]) {
      {1,  2},
      {12, 21},
      {3,  3},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11}
  }, 6, __LINE__);
}

static void test_best_scenario_with_specials(void) {
  /**
   * Card [0|0] can be placed everywhere. Will allow two incompatible dominoes to sit nearby.
   * E.g: [1|2][0|0][5|6]
   */

  test_best_scenario_shuffle(1 + 1 + 6 + 6, (struct Domino[]) {
      {1, 1},
      {6, 6},
      {0, 0},
  }, 3, __LINE__);

  test_best_scenario_shuffle(1 + 1 + 1 + 1, (struct Domino[]) {
      {1, 1},
      {1, 1},
      {0, 0},
  }, 3, __LINE__);

  test_best_scenario_shuffle(1 + 1 + 6 + 6, (struct Domino[]) {
      {1, 1},
      {0, 0},
      {6, 6},
  }, 3, __LINE__);

  test_best_scenario_shuffle(1 + 1 + 6 + 6, (struct Domino[]) {
      {0, 0},
      {1, 1},
      {6, 6},
  }, 3, __LINE__);

  test_best_scenario_shuffle(1 + 1 + 6 + 6, (struct Domino[]) {
      {6, 6},
      {0, 0},
      {1, 1},
  }, 3, __LINE__);

  test_best_scenario_shuffle(6 + 6 + 6 + 6, (struct Domino[]) {
      {6, 6},
      {0, 0},
      {6, 6},
  }, 3, __LINE__);

  /**
   * CARD [11|11]: will add 1 to the closest domino. If it's 6, will become 1.
   * E.g:  [1|6][6|3][11|11] => [2|1][1|4][4|4]
   *
   */

  // [1|6][6|3][11|11] (16) => [2|1][1|4][4|4] (16)
  test_best_scenario_shuffle(16, (struct Domino[]) {
      {1,  6},
      {6,  3},
      {11, 11},
  }, 3, __LINE__);

  // [5|5][5|5][11|11] (20) => [6|6][6|6][6|6] (36)
  test_best_scenario_shuffle(6 * 6, (struct Domino[]) {
      {5,  5},
      {5,  5},
      {11, 11},
  }, 3, __LINE__);

  test_best_scenario_shuffle(6 * 6, (struct Domino[]) {
      {5,  5},
      {11, 11},
      {5,  5},
  }, 3, __LINE__);

  test_best_scenario_shuffle(6 * 6, (struct Domino[]) {
      {11, 11},
      {5,  5},
      {5,  5},
  }, 3, __LINE__);

  // [4|4][11|11] (8) => [5|5][5|5] (20) => [5|5][5|5][5|5] (30)
  test_best_scenario_shuffle(30, (struct Domino[]) {
      {11, 11},
      {4,  4},
      {5,  5},
  }, 3, __LINE__);

  test_best_scenario_shuffle(5 * 4, (struct Domino[]) {
      {11, 11},
      {4,  4},
      {6,  6},
  }, 3, __LINE__);

  test_best_scenario_shuffle(6 * 4, (struct Domino[]) {
      {11, 11},
      {3,  3},
      {4,  4},
  }, 3, __LINE__);

  test_best_scenario_shuffle(6 * 4, (struct Domino[]) {
      {3,  3},
      {11, 11},
      {4,  4},
  }, 3, __LINE__);

  /**
   * CARD [12|21]: will copy mirror the closest domino.
   * E.g: [1|2][2|3][12|21] => [1|2][2|3][3|2]
   */

  test_best_scenario_shuffle(3 + 3 + 3 + 4 + 4 + 3, (struct Domino[]) {
      {3,  3},
      {12, 21},
      {3,  4},
  }, 3, __LINE__);

  test_best_scenario_shuffle(6 * 4, (struct Domino[]) {
      {6,  6},
      {12, 21},
      {3,  4},
  }, 3, __LINE__);

  // [12|21]
  // [12|21][6|1] => [1|6][6|1]
  // [1|6][6|1][1|2] (17)
  test_best_scenario_shuffle(17, (struct Domino[]) {
      {1,  2},
      {12, 21},
      {1,  6},
  }, 3, __LINE__);

  test_best_scenario_shuffle(17, (struct Domino[]) {
      {12, 21},
      {1,  2},
      {1,  6},
  }, 3, __LINE__);

  test_best_scenario_shuffle(17, (struct Domino[]) {
      {1,  6},
      {1,  2},
      {12, 21},
  }, 3, __LINE__);

  /**
   * Combinations between specials
   */
  test_best_scenario_shuffle(6 * 4 + 5 + 5, (struct Domino[]) {
      {6,  6}, // Highest to be mirror-copied
      {12, 21}, // mirror-copy
      {5,  5}, // another high value domino. will be added thanks to the neutral
      {0,  0} // neutral element.
  }, 4, __LINE__);


  // [5|5][5|5][5|5][5|5]
  test_best_scenario_shuffle(5 * 8, (struct Domino[]) {
      {1,  2},
      {12, 21},
      {3,  3},
      {11, 11},
      {11, 11}
  }, 5, __LINE__);
}

static void test_best_scenario(void) {
  test_best_scenario_shuffle(20, (struct Domino[]) {
      {3, 4},
      {1, 3},
      {4, 5},
      {2, 2},
  }, 4, __LINE__);

  test_best_scenario_shuffle(20, (struct Domino[]) {
      {4, 3},
      {3, 1},
      {4, 5},
      {2, 2},
  }, 4, __LINE__);

  test_best_scenario_shuffle(20, (struct Domino[]) {
      {1, 3},
      {4, 5},
      {2, 2},
      {3, 4},
  }, 4, __LINE__);

  test_best_scenario_shuffle(20, (struct Domino[]) {
      {1, 3},
      {4, 5},
      {3, 4},
      {2, 2},
  }, 4, __LINE__);

  test_best_scenario_shuffle(20, (struct Domino[]) {
      {1, 3},
      {3, 4},
      {4, 5},
      {2, 2},
  }, 4, __LINE__);

  test_best_scenario_shuffle(20, (struct Domino[]) {
      {3, 4},
      {1, 3},
      {4, 5},
      {2, 2},
  }, 4, __LINE__);

  test_best_scenario_shuffle(3 + 2 + 2 + 4, (struct Domino[]) {
      {2, 1},
      {3, 2},
      {2, 4},
  }, 3, __LINE__);

  test_best_scenario_shuffle(3 + 2 + 2 + 4, (struct Domino[]) {
      {3, 2},
      {2, 1},
      {2, 4},
  }, 3, __LINE__);

  test_best_scenario_shuffle(1 + 3 + 3 + 3 + 6 + 3, (struct Domino[]) {
      {1, 3},
      {6, 3},
      {3, 3},
  }, 3, __LINE__);

  test_best_scenario_shuffle(1 + 3 + 3 + 3 + 6 + 3, (struct Domino[]) {
      {1, 3},
      {3, 3},
      {6, 3},
  }, 3, __LINE__);

  test_best_scenario_shuffle(28, (struct Domino[]) {
      {1, 6},
      {3, 6},
      {1, 4},
      {3, 4},
  }, 4, __LINE__);

  test_best_scenario_shuffle(28, (struct Domino[]) {
      {3, 6},
      {1, 6},
      {1, 4},
      {3, 4},
  }, 4, __LINE__);

  test_best_scenario_shuffle(1 + 3 + 3 + 4 + 4 + 5, (struct Domino[]) {
      {1, 3},
      {3, 4},
      {4, 5},
      {2, 2}
  }, 4, __LINE__);

  test_best_scenario_shuffle(6 + 6, (struct Domino[]) {
      {6, 6},
      {1, 1},
      {2, 2},
      {3, 3}
  }, 4, __LINE__);

  test_best_scenario_shuffle(6 + 6 + 6 + 1, (struct Domino[]) {
      {6, 6},
      {6, 1},
      {2, 2},
      {3, 3}
  }, 4, __LINE__);

  test_best_scenario_shuffle(12, (struct Domino[]) {
      {6,  6},
      {11, 11},
      {11, 11}
  }, 3, __LINE__);

  test_best_scenario_shuffle(6 * 2 * 4, (struct Domino[]) {
      {3,  3},
      {11, 11},
      {11, 11},
      {11, 11},
      {11, 11},
  }, 5, __LINE__);

  test_best_scenario_case(6 * 2 * 2, (struct Domino[]) {
      {5,  5},
      {11, 11},
      {11, 11},
  }, 3, __LINE__);

  test_best_scenario_case(6 * 2 * 2, (struct Domino[]) {
      {5,  5},
      {11, 11},
      {11, 11},
      {11, 11},
  }, 4, __LINE__);

  test_best_scenario_shuffle(6 * 2, (struct Domino[]) {
      {6,  6},
      {11, 11},
  }, 2, __LINE__);

  test_best_scenario_shuffle(6 * 2 * 2, (struct Domino[]) {
      {5,  5},
      {11, 11},
      {11, 11},
  }, 3, __LINE__);

  test_best_scenario_case(1 + 2 + 2 + 1, (struct Domino[]) {
      {1,  2},
      {12, 21},
  }, 2, __LINE__);

  test_best_scenario_case(2 + 3 + 3 + 3, (struct Domino[]) {
      {1,  2},
      {11, 11},
  }, 2, __LINE__);

  test_best_scenario_case(17, (struct Domino[]) {
      {1,  2},
      {12, 21},
      {11, 11},
  }, 3, __LINE__);
}

static void test_best_scenario_with_specials_2(void) {
  test_best_scenario_shuffle(1 + 6 + 6 + 6 + 6 + 6, (struct Domino[]) {
      {5,  6},
      {12, 21},
      {11, 11},
  }, 3, __LINE__);

  test_best_scenario_shuffle(6 + 6 + 6 + 6, (struct Domino[]) {
      {6,  6},
      {12, 21},
      {11, 11},
  }, 3, __LINE__);

  test_best_scenario_shuffle(3 * 2 * 5, (struct Domino[]) {
      {6,  6},
      {12, 21},
      {11, 11},
      {11, 11},
      {11, 11},
  }, 5, __LINE__);

  test_best_scenario_shuffle(2 + 2 + 1 + 1 + 6 + 6 + 6 + 6, (struct Domino[]) {
      {5,  5},
      {0,  0},
      {1,  1},
      {11, 11},
  }, 4, __LINE__);

  test_best_scenario_shuffle(6 + 6 + 1 + 1 + 6 + 6 + 6 + 6, (struct Domino[]) {
      {5,  5},
      {0,  0},
      {5,  5},
      {11, 11},
  }, 4, __LINE__);
}

static void test_format_dominoes_as_commands(void) {

  TEST_ASSERT_EQUAL_STRING(format_dominoes_as_commands((struct Domino[]) {
      {1, 3},
      {3, 5},
      {5, 7},
      {7, 9}
  }, 4), "S 1 3 R 3 5 R 5 7 R 7 9");

  TEST_ASSERT_EQUAL_STRING(format_dominoes_as_commands((struct Domino[]) {
      {1, 3},
      {3, 4},
      {4, 5}
  }, 3), "S 1 3 R 3 4 R 4 5");

  TEST_ASSERT_EQUAL_STRING(format_dominoes_as_commands((struct Domino[]) {
      {1, 3},
      {0, 0},
      {4, 5}
  }, 3), "S 1 3 R 0 0 R 4 5");

  TEST_ASSERT_EQUAL_STRING(format_dominoes_as_commands((struct Domino[]) {
      {1,  3},
      {11, 11},
      {4,  5}
  }, 3), "S 1 3 R 11 11 R 4 5");

  TEST_ASSERT_EQUAL_STRING(format_dominoes_as_commands((struct Domino[]) {
      {1,  3},
      {11, 11},
      {4,  5}
  }, 3), "S 1 3 R 11 11 R 4 5");

  TEST_ASSERT_EQUAL_STRING(format_dominoes_as_commands((struct Domino[]) {
      {1,  3},
      {3,  5},
      {11, 11}
  }, 3), "S 1 3 R 3 5 R 11 11");
}

static void test_format_domino(void) {
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {1, 2}), "[1|2]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {2, 3}), "[2|3]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {10, 20}), "[10|20]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {15, 200}), "[15|200]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {4, 1}), "[4|1]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {11, 11}), "[11|11]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {0, 0}), "[0|0]");
  TEST_ASSERT_EQUAL_STRING(format_domino((struct Domino) {12, 21}), "[12|21]");
}

static void test_format_dominoes_for_table(void) {
  TEST_ASSERT_EQUAL_STRING(format_dominoes_for_table((struct Domino[]) {
      {1, 3},
      {3, 5},
      {5, 7},
      {7, 9}
  }, 4), "[1|3] [3|5] [5|7] [7|9]");

  TEST_ASSERT_EQUAL_STRING(format_dominoes_for_table((struct Domino[]) {
      {1, 3},
      {0, 0},
      {5, 7},
      {7, 9}
  }, 4), "[1|3] [0|0] [5|7] [7|9]");

  TEST_ASSERT_EQUAL_STRING(format_dominoes_for_table((struct Domino[]) {
      {1, 3},
      {3, 5},
  }, 2), "[1|3] [3|5]");
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
  TEST_ASSERT_EQUAL_STRING("S 4 1 R 1 6 R 6 6 R 6 6 R 6 6 R 6 3 R 3 4 R 4 4 R 4 5", challenge);
  free(challenge);

  challenge = process_challenge_1((struct Domino[]) {
      {6, 6},
      {6, 6},
      {6, 6},
      {3, 6},
      {1, 6},
      {2, 6},
      {1, 4},
      {4, 5},
      {3, 4},
      {4, 4},
  }, 10);
  TEST_ASSERT_EQUAL_STRING("S 4 1 R 1 6 R 6 6 R 6 6 R 6 6 R 6 3 R 3 4 R 4 4 R 4 5", challenge);
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
  TEST_ASSERT_EQUAL_STRING("S 2 2 R 2 3", challenge);
  free(challenge);

  challenge = process_challenge_1((struct Domino[]) {{1, 1},
                                                     {2, 3}}, 2);
  TEST_ASSERT_EQUAL_STRING("S 2 3", challenge);
  free(challenge);

  challenge = process_challenge_1((struct Domino[]) {{3, 4},
                                                     {1, 3},
                                                     {4, 5},
                                                     {2, 2}}, 4);
  TEST_ASSERT_EQUAL_STRING("S 1 3 R 3 4 R 4 5", challenge);
  free(challenge);
}

int main(void) {
  UnityBegin("test_domino.c");

  RUN_TEST(test_process_challenge_1);
  RUN_TEST(test_interactive_1);
  RUN_TEST(test_interactive_2);
  RUN_TEST(test_interactive_3);
  RUN_TEST(test_interactive_4);
  RUN_TEST(test_interactive_5);
  RUN_TEST(test_scenario_with);
  RUN_TEST(test_format_domino);
  RUN_TEST(test_format_dominoes_as_commands);
  RUN_TEST(test_table_dominoes_push);
  RUN_TEST(test_first_number_from_string);
  RUN_TEST(test_is_string_in_string);
  RUN_TEST(test_upcase_str);
  RUN_TEST(test_guess_selection);
  RUN_TEST(test_str_add_padding);
  RUN_TEST(test_rotate_domino);
  RUN_TEST(test_get_and_set_user_dominoes);
  RUN_TEST(test_needs_to_be_rotated_before_putting_on_table);
  RUN_TEST(test_format_dominoes_for_table);
  RUN_TEST(test_best_scenario);
  RUN_TEST(test_best_scenario_with_specials);
  RUN_TEST(test_best_scenario_with_specials_2);
  RUN_TEST(test_best_scenario_long);

  return UnityEnd();
}
