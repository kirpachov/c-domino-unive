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

bool is_exit_command(char *command);
int points_from_tree(struct Node *root);
void print_points(struct Node *root);
void resize_dominoes_array(struct Domino **arr, const int new_size);
void push(const struct Domino domino, struct Domino *domino_arr, const int domino_arr_size);
void unshift(struct Domino domino, struct Domino *domino_arr, const int before_size);
void pop(const int index, struct Domino *domino_arr, const int domino_arr_size);
int char_to_int(const char ch);
char int_to_char(const int i);
bool is_mirror_domino(const struct Domino domino);
bool is_addone_domino(const struct Domino domino);
bool is_glue_domino(const struct Domino domino);
bool is_domino_special(const struct Domino domino);
char *format_domino(const struct Domino d);
int random_between(const int lower, const int upper);
char *format_dominoes_inline(const struct Domino *dominoes, const int size);
void print_table(struct Node *root_node);
void clean_screen(void);
bool node_in_array(struct Node **arr, const int size, struct Node *node);
void tree_to_array_rec(struct Node *node, struct Node ***result, int *result_size);
void tree_to_array(struct Node *root_node, struct Node ***result, int *result_size);
bool can_place_domino_on_node(const struct Domino domino, const struct Node *node);
bool can_place_domino_in_tree(const struct Domino domino, struct Node *root_node);
int valid_moves_count(struct Node *root_node, const struct Domino *dominoes, const int dominoes_size);
void print_dominoes_with_valid_moves(
    struct Node *root_node,
    const struct Domino *dominoes,
    const int dominoes_size
);
void add_moves_to_tree(struct Node *root, struct Domino d);
void remove_moves_from_tree(struct Node *root);
int str_to_i(const char *str);
int select_position(
    struct Node *root_node,
    struct Domino domino,
    struct Node **to_attach_node
);
int select_domino(
    struct Node *root_node,
    struct Domino *user_dominoes,
    const int user_dominoes_size
);
struct Domino rotate_domino(const struct Domino d);
void acquire_command(char **last_command);
struct Domino *random_special_domino(void);
struct Domino *random_domino(bool specials);
struct Domino *init_random_dominoes_arr(int size, bool specials);
bool select_orientation(struct Node *root_node, struct Domino selected_domino);
void display_welcome_message(void);
int run_interactive(void);
void initialize(void);
struct Node *create_node(struct Domino *domino, bool is_vert);
struct Domino *create_domino(const int left, const int right);
void rotate_if_necessary(struct Node *parent, struct Node *child, int side);
void link_nodes(struct Node *parent, struct Node *child, int side);
void write_node(
    const struct Node *node,
    char **matrix,
    const int x,
    const int y,
    const bool rtl
);
void add_empty_columns_before(char **matrix, const int quantity, const int matrix_width,
                              const int matrix_height);
void add_empty_columns_after(char **matrix, const int quantity, const int matrix_width,
                             const int matrix_height);
void add_empty_rows_after(char ***matrix, const int quantity, const int matrix_width, const int matrix_height);
char *matrix_to_str(char **matrix, const int width, const int height);
void print_matrix(char **matrix, const int width, const int height);
void print_tree(struct Node *root);
void write_nodes(
    char **matrix,
    struct Node *node,
    const int x,
    const int y,
    int *matrix_width,
    int *matrix_height
);
void add_node_to_written(unsigned long int **written, const int written_length, const unsigned long int node_id);
bool is_long_into_arr(const unsigned long int needle, const unsigned long int *haystack,
                      const unsigned long int haystack_length);
int max2(const int first, const int second);
int max4(const int first, const int second, const int third, const int fourth);
int count_digits(int number);
int add_moves_informations(char **matrix, struct Node *node, int x, const int y, int *matrix_width,
                           int *matrix_height);
void write_nodes_rec(
    char **matrix,
    struct Node *node,
    int x,
    const int y,
    int *matrix_width,
    int *matrix_height,

    // Ids of nodes already written. Avoid infinite loops.
    unsigned long int **nodes_already_written,
    int written_length,

    const bool rtl
);
struct Node *get_most_left_node(struct Node *node);
struct Node *get_most_right_node(struct Node *node);
char **init_matrix(const int width, const int height);
void free_matrix(char **matrix, const int height);

#endif //C_DOMINO_UNIVE_DOMINO_H
