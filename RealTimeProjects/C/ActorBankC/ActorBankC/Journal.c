//
//  Journal.c
//  ActorBankC
//
//  Created by Anussha on 23/10/26.
//
#include <stdio.h>
#include <string.h>
#include "Journal.h"

int journal_append(const char *path, int sequence, const char *id, int amount) {
    char line[128];
    snprintf(line, sizeof(line), "%d,%s,%d", sequence, id, amount);

    FILE *f = fopen(path, "a");
    if (f == NULL) return 0;
    fprintf(f, "%s\n", line);
    fclose(f);
    return 1;
}

int journal_parse(const char *line, int *seq, char *id, int *amount) {
    int s, a;
    char buf[32];
    if (sscanf(line, "%d,%31[^,],%d", &s, buf, &a) != 3) return 0;
    *seq = s;
    strcpy(id, buf);
    *amount = a;
    return 1;
}

static int find_or_add(char ids[][32], int balances[], int *count, int max, const char *id) {
    for (int i = 0; i < *count; i++) {
        if (strcmp(ids[i], id) == 0) return i;
    }
    if (*count >= max) return -1;
    strcpy(ids[*count], id);
    balances[*count] = 0;
    return (*count)++;
}

int journal_rebuild(const char *path, char ids[][32], int balances[], int max) {
    FILE *f = fopen(path, "r");
    if (f == NULL) return 0;

    char line[128];
    int count = 0;
    while (fgets(line, sizeof(line), f) != NULL) {
        char id[32];
        int value = 0;
        int seq = 0;

        if (sscanf(line, "open,%31[^,],%d", id, &value) == 2) {
            int idx = find_or_add(ids, balances, &count, max, id);
            if (idx >= 0) balances[idx] = value;
        } else if (sscanf(line, "%d,%31[^,],%d", &seq, id, &value) == 3) {
            int idx = find_or_add(ids, balances, &count, max, id);
            if (idx >= 0) balances[idx] += value;
        }
    }
    fclose(f);
    return count;
}

int journal_append_open(const char *path, const char *id, int balance) {
    FILE *f = fopen(path, "a");
    if (f == NULL) return 0;
    fprintf(f, "open,%s,%d\n", id, balance);
    fclose(f);
    return 1;
}

int journal_next_sequence(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) return 1;

    char line[128];
    int last = 0;
    int seq = 0;
    char id[32];
    int amt = 0;
    while (fgets(line, sizeof(line), f) != NULL) {
        if (journal_parse(line, &seq, id, &amt)) {
            last = seq;
        }
    }
    fclose(f);
    return last + 1;
}
