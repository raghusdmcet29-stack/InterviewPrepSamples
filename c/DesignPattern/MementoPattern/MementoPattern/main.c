//
//  main.c
//  MementoPattern
//
//  Created by Anussha on 21/09/26.
//

#include <stdio.h>
#include <string.h>

#define MAX_TEXT 256
#define MAX_HISTORY 100

typedef struct {
    char content[MAX_TEXT];
} TextEditorMemento;

typedef struct {
    char content[MAX_TEXT];
} TextEditor;

void editor_init(TextEditor* editor) {
    editor->content[0] = '\0';
}

void editor_type(TextEditor* editor, const char* text) {
    strncat(editor->content, text, MAX_TEXT - strlen(editor->content) - 1);
}

TextEditorMemento editor_save(const TextEditor* editor) {
    TextEditorMemento memento;
    strcpy(memento.content, editor->content);
    return memento;
}

void editor_restore(TextEditor* editor, const TextEditorMemento* memento) {
    strcpy(editor->content, memento->content);
}

typedef struct {
    TextEditorMemento history[MAX_HISTORY];
    int count;
} Caretaker;

void caretaker_init(Caretaker* caretaker) {
    caretaker->count = 0;
}

void caretaker_save(Caretaker* caretaker, TextEditorMemento memento) {
    if (caretaker->count < MAX_HISTORY) {
        caretaker->history[caretaker->count] = memento;
        caretaker->count++;
    }
}

int caretaker_undo(Caretaker* caretaker, TextEditorMemento* outMemento) {
    if (caretaker->count == 0) return 0;
    caretaker->count--;
    *outMemento = caretaker->history[caretaker->count];
    return 1;
}

int main(void) {
    TextEditor editor1;
    editor_init(&editor1);

    editor_type(&editor1, "Hello");
    TextEditorMemento saved = editor_save(&editor1);
    printf("After typing 'Hello': %s\n", editor1.content);

    editor_type(&editor1, " World");
    printf("After typing ' World': %s\n", editor1.content);

    editor_restore(&editor1, &saved);
    printf("After restore: %s\n", editor1.content);
    
    TextEditor editor;
        editor_init(&editor);

        Caretaker caretaker;
        caretaker_init(&caretaker);

        editor_type(&editor, "A");
        caretaker_save(&caretaker, editor_save(&editor));   // checkpoint 1: "A"

        editor_type(&editor, "B");
        caretaker_save(&caretaker, editor_save(&editor));   // checkpoint 2: "AB"

        editor_type(&editor, "C");
        printf("Before undo: %s\n", editor.content);   // ABC

        TextEditorMemento memento;
        if (caretaker_undo(&caretaker, &memento)) {
            editor_restore(&editor, &memento);
        }
        printf("After 1 undo: %s\n", editor.content);  // AB

        if (caretaker_undo(&caretaker, &memento)) {
            editor_restore(&editor, &memento);
        }
        printf("After 2nd undo: %s\n", editor.content); // A

    return 0;
}
