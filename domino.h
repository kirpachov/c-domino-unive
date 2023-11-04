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

#define MAX_COMMAND_SIZE 10

#define TOTAL_USER_DOMINOES 11

struct Domino {
  int left: 24;
  int right: 24;
};

void user_dominoes_push(struct Domino);
void table_dominoes_push(struct Domino);
void universe_dominoes_push(struct Domino);
int char_to_int(const char);
bool is_char_a_number(const char);
void str_to_int_array(const char*, int*);
char* upcase_str(const char *, char*);
bool is_string_in_string(const char*, const char*);
int first_number_from_string(const char*);
int valid_move(const struct Domino);
int valid_moves_count(void);
void str_add_padding(const char* original, char* result);
int random_between(const int, const int);
struct Domino random_domino_from_universe(void);
int run_challenge(void);
void populate_universe_dominoes(void);
void print_domino(const struct Domino);
void print_table(void);
void print_user_dominoes(void);
bool is_help_command(const char*);
void print_last_command_feedback(void);
void clean_screen(void);
void print_everything(void);
void assign_user_random_dominoes(void);
void guess_selection(const char*, int*, int*);
void process_last_command(void);
void acquire_command(void);
void run_terminal(void);
int run_interactive(void);
void parse_params(const int, const char**);
void initialize(void);
void complete(void);

#endif //C_DOMINO_UNIVE_DOMINO_H
