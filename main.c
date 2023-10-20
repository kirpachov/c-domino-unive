// 902835

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
// #include "log.c"

/**
 * Max number of dominoes that the user can have.
 */
#define USER_MAX_DOMINOES_COUNT 100
#define DOMINOES_UNIVERSE_COUNT 28
#define USER_COMMAND_MAX_LENGTH 100
#define COMMAND_HELP "help"
#define COMMAND_EXIT "exit"

enum DominoStatus {
    DOMINO_IN_USERS_HAND = 1,
    DOMINO_ON_TABLE = 2
};


struct Domino {
    int left_value;
    int right_value;
};

/**
 * The dominoes that the user has.
 */
struct Domino user_dominoes[USER_MAX_DOMINOES_COUNT];
int user_dominoes_count = 5; // TODO make this a input (--dominoes 5) parameter.

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

int table_dominoes_count = 0;

/**
 * The last command that the user has typed.
 */
char last_command[USER_COMMAND_MAX_LENGTH] = "";


void remove_user_domino(int index) {
    for (int i = index; i < user_dominoes_count - 1; i++) {
        user_dominoes[i] = user_dominoes[i + 1];
    }

    user_dominoes_count--;
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

void log_table_status() {
    printf("\n");
    if (table_dominoes_count == 0) {
        printf("<Il tavolo da gioco è vuoto.>");
    } else
        for (int i = 0; i < table_dominoes_count; i++) {
            printf("[%d|%d]", table_dominoes[i].left_value, table_dominoes[i].right_value);
        }

    printf("\n\n");
}

bool can_put_domino_on_table(const struct Domino domino) {
    if (table_dominoes_count == 0) return true;

    const struct Domino last_domino_on_table = table_dominoes[table_dominoes_count - 1];

    return last_domino_on_table.left_value == domino.left_value ||
           last_domino_on_table.left_value == domino.right_value ||
           last_domino_on_table.right_value == domino.left_value ||
           last_domino_on_table.right_value == domino.right_value;
}

int count_avaliable_dominoes() {
    int count = 0;
    for (int i = 0; i < user_dominoes_count; i++) {
        if (can_put_domino_on_table(user_dominoes[i])) count++;
    }

    log_debug("count_avaliable_dominoes: %d", count);

    return count;
}

void put_domino_on_table(int index_from_user_dominoes) {
    if (!can_put_domino_on_table(user_dominoes[index_from_user_dominoes])) {
        log_error("Cannot put domino on table.");
        return;
    }

    table_dominoes[table_dominoes_count] = user_dominoes[index_from_user_dominoes];
    remove_user_domino(index_from_user_dominoes);
    table_dominoes_count++;
}

void log_user_dominoes() {
    printf("Seleziona il domino che vuoi mettere sul tavolo usando il suo indice; conferma con <invio>\n");

    for (int i = 0; i < user_dominoes_count; i++) {
        if (can_put_domino_on_table(user_dominoes[i])) printf("%i)", i);
        else printf("-)");

        printf(" [%d|%d]", user_dominoes[i].left_value, user_dominoes[i].right_value);
        printf("\n");
    }

    for (int i = 0; i < table_dominoes_count; i++) {
        printf("\n");
    }
}


bool is_last_command(const char *command) {
    return strcmp(last_command, command) == 0;
}

int potenza(const int initial, const int elevate_at) {
    if (elevate_at <= 0) return 1;

    int final = initial;
    for (int i = 0; i < elevate_at - 1; i++) final *= initial;
    return final;
}

bool str_equals_int(const char *str, const int num, const int str_length) {
    int str_int = 0;
    for (int i = 0; i < str_length; i++) {
        const int current = (((char) str[i]) - 48) * potenza(10, (str_length - 1) - i);
        str_int += current;
    }

    return str_int == num;
}


void exec_command() {
    log_debug("exec_command: %s\n", last_command);
    for (int i = 0; i < user_dominoes_count; i++) {
        const char command_index[2] = {i, '\0'};

        const bool user_is_selecting_this_domino = str_equals_int(last_command, i, 1);

        if (user_is_selecting_this_domino) {
            log_debug("Putting down domino at index %d", i);
            put_domino_on_table(i);
            return;
        } else
            log_debug("Skipping domino at index %d ; Requested is %s", i, command_index);
    }
}


void log_last_command_status() {
    if (is_last_command("") || is_last_command(COMMAND_HELP) || is_last_command("?")) {
        printf("Seleziona le tessere disponibili scrivendo il loro indice e poi clicca invio\n");
    }
}

void log_status() {
    printf("\e[1;1H\e[2J");
//    printf("CLEARED\n");

    log_table_status();
    log_user_dominoes();
    printf("\n");

    // if (!is_last_command("")) printf("$ %s\n", last_command);
    // log_last_command_status();
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

void log_game_end_reason() {
    if (user_dominoes_count == 0) {
        log_info("Game has ended because user has no more dominoes.");
        return;
    }

    if (is_last_command(COMMAND_EXIT)) {
        log_info("Game has ended because user has typed exit.");
        return;
    }

    if (count_avaliable_dominoes() <= 0) {
        log_info("Game has ended because user has no more valid moves.");
        return;
    }

    log_error("Game has ended for an unknown reason.");
}

void start_game() {
    log_status();
    while ((!is_last_command(COMMAND_EXIT)) && user_dominoes_count > 0 && count_avaliable_dominoes() > 0) {
        printf("$ ");
        if (scanf("%s", last_command) != 1) {
            log_error("scanf returned an error.");
            continue;
        }

        exec_command();
        log_status();
    }
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
    log_game_end_reason();
    complete();
    return 0;
}