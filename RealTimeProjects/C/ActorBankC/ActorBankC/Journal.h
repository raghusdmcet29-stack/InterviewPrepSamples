//
//  Journal.h
//  ActorBankC
//
//  Created by Anussha on 23/10/26.
//

#ifndef JOURNAL_H
#define JOURNAL_H

int journal_append(const char *path, int sequence, const char *id, int amount);
int journal_append_open(const char *path, const char *id, int balance);
int journal_parse(const char *line, int *seq, char *id, int *amount);
int journal_rebuild(const char *path, char ids[][32], int balances[], int max);
int journal_next_sequence(const char *path);
#endif
