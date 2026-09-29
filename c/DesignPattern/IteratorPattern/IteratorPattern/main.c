//
//  main.c
//  IteratorPattern
//
//  Created by Anussha on 10/09/26.
//

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    void** items;
    int count;
    int currentIndex;
} PlaylistIterator;

bool hasNext(PlaylistIterator* it) {
    return it->currentIndex < it->count;
}

void* next(PlaylistIterator* it) {
    void* item = it->items[it->currentIndex];
    it->currentIndex++;
    return item;
}

PlaylistIterator makeIterator(void** items, int count) {
    PlaylistIterator it;
    it.items = items;
    it.count = count;
    it.currentIndex = 0;
    return it;
}

int main(void) {
    const char* songs[] = {
        "Bohemian Rhapsody",
        "Stairway to Heaven",
        "Hotel California"
    };
    int count = 3;

    void* items[3];
    for (int i = 0; i < count; i++) {
        items[i] = (void*)songs[i];
    }

    PlaylistIterator iterator = makeIterator(items, count);

    while (hasNext(&iterator)) {
        const char* song = (const char*)next(&iterator);
        printf("%s\n", song);
    }
    
    int nums[] = {1, 2, 3};
    int count2 = 3;

    void* intItems[3];
    for (int i = 0; i < count2; i++) {
        intItems[i] = (void*)&nums[i];
    }

    PlaylistIterator intIterator = makeIterator(intItems, count2);

    while (hasNext(&intIterator)) {
        int* num = (int*)next(&intIterator);
        printf("%d\n", *num);
    }

    return 0;
}
