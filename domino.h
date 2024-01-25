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

#define MAX_COMMAND_SIZE 100

#define TOTAL_USER_DOMINOES 11

struct Domino {
  int left: 24;
  int right: 24;
};


struct Node {
  struct Domino* domino;

  bool is_vert; // true if vertical, false if horizontal

  unsigned long int id; // Incremental id

  /**
   * Attached nodes.
   */
  struct Node* top_left;
  struct Node* top_right;
  struct Node* bottom_left;
  struct Node* bottom_right;

  /**
   * Most of the time will be some invalid value.
   * When valid numbers tho, will be the index of the position.
   */
  int top_left_move;
  int top_right_move;
  int bottom_left_move;
  int bottom_right_move;
};

#define TOP_LEFT 1
#define TOP_RIGHT 2
#define BOTTOM_LEFT 3
#define BOTTOM_RIGHT 4

#define WELCOME_MESSAGE \
"Benvenuto al domino 2d di Oleksandr Kirpachov (Sasha)!\n\
Per giocare, devi inserire dei comandi nel terminale e premere invio.\n\
I comandi disponibili vengono mostrati in ogni fase del gioco.\n\
In breve, il flow del gioco è il seguente:\n\
1. Seleziona il domino da mettere sul tavolo tra quelli disponibili.\n\
2. Seleziona la posizione dove vuoi mettere il domino.\n\
3. Seleziona come vuoi mettere quel domino (orizzontale o verticale).\n\
4. Se hai altre mosse disponibili, ripeti dal punto 1. Altrimenti il gioco finisce.\n\
Il punteggio viene calcolato come la somma dei numeri dei domino che sono sul tavolo.\n\
Per iniziare a giocare, premi qualsiasi tasto e poi invio.\n"

void add_moves_to_tree(struct Node *root, struct Domino d);
void print_tree(struct Node *root);
void tree_to_array(struct Node* root_node, struct Node ***result, int *result_size);
bool is_exit_command(char *command);
void free_matrix(char **matrix, const int height);
struct Node *create_node(struct Domino *, bool is_vert);
struct Domino *create_domino(const int, const int);
void link_nodes(struct Node *parent, struct Node *child, int side);
void write_node(const struct Node *, char **, const int, const int, const bool);
void add_empty_columns_before(char **, const int, const int, const int);
void add_empty_columns_after(char **, const int, const int, const int);
char *matrix_to_str(char **matrix, const int width, const int height);
void print_matrix(char **, const int, const int);
void add_node_to_written(unsigned long int **written, const int written_length, const unsigned long int node_id);
void write_nodes(
    char **, struct Node *,
    const int, const int,
    int *, int *);
void write_nodes_rec(
    char **, struct Node *,
    const int, const int,
    int *, int *, unsigned long int **, int, const bool);
char** init_matrix(const int width, const int height);
char** init_matrix_with(const int width, const int height, const char ch);
struct Node *get_most_left_node(struct Node *);
struct Node *get_most_right_node(struct Node *);
bool is_mirror_domino(const struct Domino domino);
bool is_addone_domino(const struct Domino domino);
bool is_glue_domino(const struct Domino domino);
bool is_domino_special(const struct Domino d);
bool can_place_on_table(const struct Domino d);
struct Domino *allocate_memory_for_dominoes(struct Domino **arr, int size);
void resize_dominoes_array(struct Domino **arr, const int new_size);
void pop(const int index, struct Domino* domino_arr, const int domino_arr_size);
struct Domino *add_domino_on_start(struct Domino domino, const struct Domino *domino_arr, int *domino_arr_size);
int char_to_int(const char);
bool is_char_a_number(const char);
char* upcase_str(const char *original);
bool is_string_in_string(const char*, const char*);
int first_number_from_string(const char*);
char* str_add_padding(const char* original);
int random_between(const int, const int);
struct Domino random_domino_from_universe(void);
int run_challenge_1(void);
int run_challenge(void);
char *format_dominoes_inline(const struct Domino *, const int);
char* format_user_points_str(void);
void clean_screen(void);
char* format_domino(const struct Domino);
struct Domino rotate_domino(const struct Domino d);
void acquire_command(char** last_command);
int run_interactive(void);
void initialize(void);

#endif //C_DOMINO_UNIVE_DOMINO_H
