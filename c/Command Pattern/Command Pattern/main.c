//
//  main.c
//  Command Pattern
//
//  Created by Anussha on 04/09/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The receiver
typedef struct {
    char text[256];
} TextBuffer;

void bufferInsert(TextBuffer* buf, const char* s) {
    strcat(buf->text, s);
}

void bufferDelete(TextBuffer* buf, size_t count) {
    size_t len = strlen(buf->text);
    buf->text[len - count] = '\0';
}

// Forward declaration - Command needs to point to itself indirectly via function pointers
typedef struct Command Command;

struct Command {
    void (*execute)(Command* self);
    void (*undo)(Command* self);
    void* commandData;   // opaque pointer to whatever data this concrete command needs
};

// Data for InsertCommand
typedef struct {
    TextBuffer* buffer;
    char textToInsert[256];
} InsertData;

void insertExecute(Command* self) {
    InsertData* data = (InsertData*)self->commandData;
    bufferInsert(data->buffer, data->textToInsert);
}

void insertUndo(Command* self) {
    InsertData* data = (InsertData*)self->commandData;
    bufferDelete(data->buffer, strlen(data->textToInsert));
}

Command makeInsertCommand(TextBuffer* buffer, const char* text) {
    InsertData* data = malloc(sizeof(InsertData));
    data->buffer = buffer;
    strcpy(data->textToInsert, text);
    
    Command cmd;
    cmd.execute = insertExecute;
    cmd.undo = insertUndo;
    cmd.commandData = data;
    return cmd;
}

// Data for DeleteCommand
typedef struct {
    TextBuffer* buffer;
    size_t count;
    char deletedText[256];   // captured at execute() time, same wrinkle as Swift/C++ versions
} DeleteData;

void deleteExecute(Command* self) {
    DeleteData* data = (DeleteData*)self->commandData;
    size_t len = strlen(data->buffer->text);
    strncpy(data->deletedText, data->buffer->text + (len - data->count), data->count);
    data->deletedText[data->count] = '\0';
    bufferDelete(data->buffer, data->count);
}

void deleteUndo(Command* self) {
    DeleteData* data = (DeleteData*)self->commandData;
    bufferInsert(data->buffer, data->deletedText);
}

Command makeDeleteCommand(TextBuffer* buffer, size_t count) {
    DeleteData* data = malloc(sizeof(DeleteData));
    data->buffer = buffer;
    data->count = count;
    
    Command cmd;
    cmd.execute = deleteExecute;
    cmd.undo = deleteUndo;
    cmd.commandData = data;
    return cmd;
}

#define MAX_HISTORY 10

typedef struct {
    Command history[MAX_HISTORY];
    int count;   // acts as the stack pointer - top of stack is history[count - 1]
} Invoker;

void invokerInit(Invoker* inv) {
    inv->count = 0;
}

void invokerRun(Invoker* inv, Command cmd) {
    cmd.execute(&cmd);
    inv->history[inv->count] = cmd;
    inv->count++;
}

void invokerUndoLast(Invoker* inv) {
    if (inv->count == 0) {
        printf("Nothing to undo\n");
        return;
    }
    inv->count--;
    Command* cmd = &inv->history[inv->count];
    cmd->undo(cmd);
    free(cmd->commandData);   // matches the malloc in makeInsertCommand/makeDeleteCommand
}
int main(void) {
    TextBuffer buffer;
    buffer.text[0] = '\0';   // empty C string to start
    
    Invoker invoker;
    invokerInit(&invoker);
    
    invokerRun(&invoker, makeInsertCommand(&buffer, "Hello"));
    printf("%s\n", buffer.text);   // "Hello"
    
    invokerRun(&invoker, makeInsertCommand(&buffer, " World"));
    printf("%s\n", buffer.text);   // "Hello World"
    
    invokerRun(&invoker, makeDeleteCommand(&buffer, 6));
    printf("%s\n", buffer.text);   // "Hello"   (deleted " World")
    
    invokerUndoLast(&invoker);
    printf("%s\n", buffer.text);   // "Hello World"   (undid the delete)
    
    invokerUndoLast(&invoker);
    printf("%s\n", buffer.text);   // "Hello"   (undid second insert)
    
    invokerUndoLast(&invoker);
    printf("%s\n", buffer.text);   // ""   (undid first insert)
    
    invokerUndoLast(&invoker);   // "Nothing to undo" - stack empty
    
    return 0;
}
