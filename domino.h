//
// Created by sasha on 04/11/23.
//

#ifndef C_DOMINO_UNIVE_DOMINO_H
#define C_DOMINO_UNIVE_DOMINO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "log.h"

#define SMALLES_DOMINO_VALUE 1
#define LARGEST_DOMINO_VALUE 6

#define LEFT_SIDE 1
#define RIGHT_SIDE 2

#define MAX_COMMAND_SIZE 10

#define TOTAL_USER_DOMINOES 11

struct Domino {
  int left: 24;
  int right: 24;
};


//bool have_anything_in_common(const struct Domino, const struct Domino);

/* ********************* TEST UTILS ********************* */
void empty_user_dominoes(void);
void empty_table_dominoes(void);
void set_user_dominoes(const struct Domino* arr, const int arr_size);
struct Domino* get_user_dominoes(void);
int get_user_dominoes_size(void);
void set_table_dominoes(const struct Domino* arr, const int arr_size);
struct Domino* get_table_dominoes(void);
int get_table_dominoes_size(void);
void set_last_command(char* new_last_command);
char* get_last_command(void);
/* ********************* TEST UTILS ********************* */

bool can_place_on_left(const struct Domino d);
bool can_place_on_right(const struct Domino d);
bool can_place_on_table(const struct Domino d);
//char* where_can_place(const struct Domino d);
struct Domino *allocate_memory_for_dominoes(struct Domino **arr, int size);
void resize_dominoes_array(struct Domino **arr, const int new_size);
void push(struct Domino domino, struct Domino* domino_arr, int* domino_arr_size);
void unshift(struct Domino domino, struct Domino* domino_arr, int* domino_arr_size);
struct Domino *add_domino_on_start(struct Domino domino, const struct Domino *domino_arr, int *domino_arr_size);
void pop(const int index, struct Domino* domino_arr, int* domino_arr_size);
int total_user_dominoes_valid(void);
void user_dominoes_push(struct Domino);
void user_dominoes_pop(const int);
void table_dominoes_push(struct Domino domino, const bool left_side);
void universe_dominoes_push(struct Domino);
int char_to_int(const char);
bool is_char_a_number(const char);
void str_to_int_array(const char*, int*);
char* upcase_str(const char *original);
bool is_string_in_string(const char*, const char*);
int first_number_from_string(const char*);
//int valid_move(const struct Domino);
int valid_moves_count(void);
struct Domino* valid_moves(void);
char* str_add_padding(const char* original);
int random_between(const int, const int);
struct Domino random_domino_from_universe(void);
int run_challenge(void);
void populate_universe_dominoes(void);
char* format_dominoes_for_table(const struct Domino*, const int);
char* format_table_dominoes(void);
void print_table(void);
char* format_dominoes_with_valid_moves(const struct Domino*, const int);
char* format_user_dominoes();
void print_user_dominoes(void);
bool is_help_command(const char*);
int calc_points_from(const struct Domino* arr, const int arr_size);
int calc_user_points(void);
char* format_user_points_str(void);
void print_user_points(void);
void print_last_command_feedback(void);
void clean_screen(void);
void print_everything(void);
void assign_user_random_dominoes(void);
void guess_selection(const char*, int*, int*);
char* format_domino(const struct Domino);
char *side_str(const bool is_left_side);
bool can_be_put_on_table(const struct Domino, const bool left_side);
bool needs_to_be_rotated_before_putting_on_table(struct Domino selected_domino, bool is_left_side);
struct Domino rotate_domino(const struct Domino d);
struct Domino rotate_if_necessary(const struct Domino domino, const bool is_left_side);
struct Domino rotate_if_necessary(const struct Domino domino, const bool is_left_side);
int put_on_table(const int index, const bool left_side);
void process_last_command(void);
void acquire_command(void);
int scenario_with(const struct Domino *user_arr, const int user_arr_size, const struct Domino *table_arr,
                  const int table_arr_size, struct Domino** best_table_possible, int *best_table_possible_size);
struct Domino *dominoes_without_element(
    const struct Domino *dominoes,
    const int size,
    const int index_of_element_to_remove);
bool dominoes_equal(const struct Domino,const struct Domino);
int domino_index_in(const struct Domino* arr, const int arr_size, const struct Domino to_find);
//void list_all_scenarios(const struct Domino *dominoes, const int dominoes_size);
int best_scenario(const struct Domino *dominoes, const int dominoes_size, struct Domino ** best_table_possible, int* best_table_possible_size);
void run_terminal(void);
int run_interactive(void);
void parse_params(const int, const char**);
void initialize(void);
void complete(void);

#endif //C_DOMINO_UNIVE_DOMINO_H
