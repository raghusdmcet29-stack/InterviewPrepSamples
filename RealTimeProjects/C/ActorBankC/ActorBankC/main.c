//
//  main.c
//  ActorBankC
//
//  Created by Anussha on 23/10/26.
//
#include <stdio.h>
#include "Bank.h"

int main(void) {
    const char *path = "/Users/anussha/ActorBankData/journal-c-bank.txt";
    remove(path);

    Bank bank1;
    bank_init(&bank1, path);
    bank_open(&bank1, "A", 100);
    bank_deposit(&bank1, "A", 20);
    bank_deposit(&bank1, "A", 5);
    bank_destroy(&bank1);

    Bank bank2;
    int bal = 0;
    bank_init(&bank2, path);
    bank_load(&bank2);
    bank_balance(&bank2, "A", &bal);
    printf("%d\n", bal);                            /* expect 125 */
    printf("%d\n", bank_deposit(&bank2, "A", 1));   /* expect 1 */
    bank_balance(&bank2, "A", &bal);
    printf("%d\n", bal);                            /* expect 126 */
    bank_destroy(&bank2);
    return 0;
}
