// 902835

#include <stdio.h>
#include <stdlib.h>
#include "log.h"

/**
 * Max number of dominoes that the user can have.
 */
#define USER_MAX_DOMINOES_COUNT 100
#define DOMINOES_UNIVERSE_COUNT 28

int user_dominoes_count = 5; // TODO make this a input (--dominoes 5) parameter.
int seed = 0; // TODO make this a input (--seed 1) parameter.

struct Domino {
    int left_value;
    int right_value;
};

/**
 * The dominoes that the user has.
 */
struct Domino user_dominoes[USER_MAX_DOMINOES_COUNT];

/**
 * The dominoes that the user can have.
 *
 * Note: user can have twice the same domino.
 */
struct Domino dominoes_universe[DOMINOES_UNIVERSE_COUNT];

/**
 * The dominoes that are on the table.
 */
struct Domino table_dominoes[USER_MAX_DOMINOES_COUNT];

int dominoes_on_table = 0;

void put_domino_on_table(int index_from_user_dominoes) {
    printf("TODO put_domino_on_table figa oh");
    dominoes_on_table++;
    // TODO remove that domino from user's hand
    // TODO add this domino to table_dominoes
}

void populate_dominoes_universe() {
    int i = 0;
    for (int left_value = 0; left_value <= 6; left_value++) {
        for (int right_value = left_value; right_value <= 6; right_value++) {
            dominoes_universe[i].left_value = left_value;
            dominoes_universe[i].right_value = right_value;
            log_debug("Adding [%d|%d] to dominoes_universe at index %d;", left_value, right_value, i);
            i++;
        }
    }
}

int random_between(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}

struct Domino random_domino() {
    return dominoes_universe[random_between(0, DOMINOES_UNIVERSE_COUNT - 1)];
}

void assign_user_random_dominoes() {
    for (int i = 0; i < user_dominoes_count; i++) {
        user_dominoes[i] = random_domino();
    }

    // Just logging
    for (int i = 0; i < user_dominoes_count; i++) {
        log_debug("User has dominoes [%d|%d] at index %d;", user_dominoes[i].left_value, user_dominoes[i].right_value,
                  i);
    }
}

//char* domino_to_string(){
//    char* domino_string;
////    char* domino_string = malloc(5 * sizeof(char));
//    sprintf(domino_string, "[%d|%d]", random_domino().left_value, random_domino().right_value);
////    sprintf(domino_string, "[%d|%d]", domino.left_value, domino.right_value);
//    return domino_string;
//}

void log_table_status() {
    printf("\n");
    if (dominoes_on_table == 0){
        printf("<Il tavolo da gioco è vuoto. Butta il primo domino selezionandolo dalla lista. Puoi usare le freccette, WASD, oppure i numeri. Per confermare il domino selezionato, clicca invio.>\n");
        return;
    }

    for(int i = 0; i < dominoes_on_table; i++){
        printf("[%d|%d]", user_dominoes[i].left_value, user_dominoes[i].right_value);
    }

    printf("\n");
}

/**
 * User can select dominoes with arrows.
 * So we need to store the current indes he's selecting.
 */
int current_domino_selection = 0;

void log_user_dominoes(){
    printf("\n");

    for(int i = 0; i < user_dominoes_count; i++){
        printf("%i) [%d|%d]\n", i, user_dominoes[i].left_value, user_dominoes[i].right_value);
    }
}

void log_status() {
    log_table_status();
    log_user_dominoes();
    printf("\n");
}

/**
 * Function called on the beginning of the execution.
 */
void initialize() {
    log_add_fp(fopen("game.log", "a"), LOG_DEBUG);
    srand(time(0));
    populate_dominoes_universe();
    log_info("Initialized.");
}

void start_game() {
    log_status();
}

/**
 * Function called on the end of the execution.
 */
void complete() {
    log_info("Completed.\n");
}

int main() {
    initialize();
    assign_user_random_dominoes();
    start_game();
    complete();
    return 0;
}