#include "test-framework/unity.h"
#include "domino.h"

void setUp(void) {}

void tearDown(void) {}

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
  char *result;

  str = "ciao";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str, result = malloc(sizeof(char) * strlen(str))), "CIAO", strlen(str));

  str = "banana";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str, result = malloc(sizeof(char) * strlen(str))), "BANANA", strlen(str));

  str = "1l";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str, result = malloc(sizeof(char) * strlen(str))), "1L", strlen(str));

  str = "1 l";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str, result = malloc(sizeof(char) * strlen(str))), "1 L", strlen(str));

  str = "1 left";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str, result = malloc(sizeof(char) * strlen(str))), "1 LEFT", strlen(str));

  str = "1 right";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str, result = malloc(sizeof(char) * strlen(str))), "1 RIGHT", strlen(str));

  str = "1 RIGHT";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str, result = malloc(sizeof(char) * strlen(str))), "1 RIGHT", strlen(str));

  str = "1 LEFT";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str, result = malloc(sizeof(char) * strlen(str))), "1 LEFT", strlen(str));

  str = "1 L";
  TEST_ASSERT_EQUAL_CHAR_ARRAY(upcase_str(str, result = malloc(sizeof(char) * strlen(str))), "1 L", strlen(str));

  free(result);
}

static void test_guess_selection(void) {
  int index;
  int position;

  guess_selection("5 l", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(1, position);

  guess_selection("5 LEFT", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(1, position);

  guess_selection("5 position", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(0, position);

  guess_selection("5 start", &index, &position);
  TEST_ASSERT_EQUAL(5, index);
  TEST_ASSERT_EQUAL(0, position);

  guess_selection("15", &index, &position);
  TEST_ASSERT_EQUAL(15, index);
  TEST_ASSERT_EQUAL(0, position);

  // RIGHT/DESTRO
  guess_selection("20 destra", &index, &position);
  TEST_ASSERT_EQUAL(20, index);
  TEST_ASSERT_EQUAL(2, position);

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

  guess_selection("25 DESTRO", &index, &position);
  TEST_ASSERT_EQUAL(25, index);
  TEST_ASSERT_EQUAL(0, position);
}

static void test_str_add_padding(void){
  char* str;
  char* result;

  str = "1";
  str_add_padding(str, result = malloc(sizeof(char) * (strlen(str) +2 )));
  TEST_ASSERT_EQUAL_CHAR_ARRAY(result, " 1 ", 3);

//  free(str);
  free(result);
}

int main(void) {
  UnityBegin("test_domino.c");

  RUN_TEST(test_first_number_from_string);
  RUN_TEST(test_is_string_in_string);
  RUN_TEST(test_upcase_str);
  RUN_TEST(test_guess_selection);
  RUN_TEST(test_str_add_padding);

  return UnityEnd();
}