#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Enum to track the type of data stored
typedef enum {
    TYPE_EMPTY,
    INT, FLOAT, CHAR, STRING
} DataType;

// Union to store different types of data
typedef union {
    int int_value;
    float float_value;
    char char_value;
    char *string_value;
} Data;

// Struct to encapsulate the data and its type
typedef struct {
    Data data;
    DataType type;
} Container;

// Function declarations
void container_init(Container *c);
void container_clear(Container *c);
void set_int(Container *c, int value);
void set_float(Container *c, float value);
void set_char(Container *c, char value);
bool set_string(Container *c, const char *value);
int get_int(const Container *c, int *value);
int get_float(const Container *c, float *value);
int get_char(const Container *c, char *value);
int get_string(const Container *c, const char **value);
void container_print(const Container *c);
const char* container_type_name(DataType type);
bool container_copy(Container *destination, const Container *source);

void container_init(Container *c) {
    if(c == NULL){
        return;
    }

    c->type = TYPE_EMPTY;
    c->data.string_value = NULL;
}

void container_clear(Container *c) {
    if(c == NULL){
        return;
    }

    if(c->type == STRING){
        free(c->data.string_value);
    }

    c->type = TYPE_EMPTY;
    c->data.string_value = NULL;
}

void set_int(Container *c, int value) {
    if(c == NULL){
        return;
    }

    container_clear(c);

    c->data.int_value = value;
    c->type = INT;
}

void set_float(Container *c, float value) {
    if(c == NULL){
        return;
    }

    container_clear(c);

    c->data.float_value = value;
    c->type = FLOAT;
}

void set_char(Container *c, char value) {
    if(c == NULL){
        return;
    }

    container_clear(c);

    c->data.char_value = value;
    c->type = CHAR;
}

bool set_string(Container *c, const char *value) {
    if(c == NULL || value == NULL){
        return false;
    }

    size_t length = strlen(value);

    if(length == SIZE_MAX){
        return false;
    }

    char *new_string = malloc((length + 1) * sizeof(char));

    if(new_string == NULL){
        return false;
    }

    memcpy(new_string, value, length + 1);

    container_clear(c);

    c->data.string_value = new_string;
    c->type = STRING;

    return true;
}

int get_int(const Container *c, int *value) {
    if(c == NULL || value == NULL || c->type != INT){
        return 0;
    }

    *value = c->data.int_value;
    return 1;
}

int get_float(const Container *c, float *value) {
    if(c == NULL || value == NULL || c->type != FLOAT){
        return 0;
    }

    *value = c->data.float_value;
    return 1;
}

int get_char(const Container *c, char *value) {
    if(c == NULL || value == NULL || c->type != CHAR){
        return 0;
    }

    *value = c->data.char_value;
    return 1;
}

int get_string(const Container *c, const char **value) {
    if(c == NULL || value == NULL || c->type != STRING){
        return 0;
    }

    *value = c->data.string_value;
    return 1;
}

const char* container_type_name(DataType type) {
    switch(type){
        case TYPE_EMPTY: return "EMPTY";
        case INT: return "INTEGER";
        case FLOAT: return "FLOAT";
        case CHAR: return "CHARACTER";
        case STRING: return "STRING";
        default: return "UNKNOWN";
    }
}

void container_print(const Container *c) {
    if(c == NULL){
        printf("Invalid Container!\n");
        return;
    }

    switch(c->type){
        case TYPE_EMPTY:{
            printf("Empty Container\n");
            break;
        }

        case INT:{
            printf("%d\n", c->data.int_value);
            break;
        }

        case FLOAT:{
            printf("%.2f\n", c->data.float_value);
            break;
        }

        case CHAR:{
            printf("%c\n", c->data.char_value);
            break;
        }

        case STRING:{
            if(c->data.string_value != NULL){
                printf("%s\n", c->data.string_value);
            }
            else{
                printf("Invalid String!\n");
            }
            break;
        }

        default:{
            printf("Invalid Data Type!\n");
            break;
        }
    }
}

bool container_copy(Container *destination, const Container *source) {
    if(destination == NULL || source == NULL){
        return false;
    }

    if(destination == source){
        return true;
    }

    if(source->type == STRING){
        if(source->data.string_value == NULL){
            return false;
        }

        size_t length = strlen(source->data.string_value);

        if(length == SIZE_MAX){
            return false;
        }

        char *new_string = malloc((length + 1) * sizeof(char));

        if(new_string == NULL){
            return false;
        }

        memcpy(new_string, source->data.string_value, length + 1);

        container_clear(destination);

        destination->data.string_value = new_string;
        destination->type = STRING;
    }
    else{
        if(source->type != TYPE_EMPTY &&
           source->type != INT &&
           source->type != FLOAT &&
           source->type != CHAR){
            return false;
        }

        Data temp = source->data;
        DataType type = source->type;

        container_clear(destination);

        destination->data = temp;
        destination->type = type;
    }

    return true;
}

int main() {
    Container c;

    container_init(&c);

    set_int(&c, 10);

    int int_value;

    if(get_int(&c, &int_value)){
        printf("Int: %d\n", int_value);
    }

    set_float(&c, 3.14);

    float float_value;

    if(get_float(&c, &float_value)){
        printf("Float: %.2f\n", float_value);
    }

    set_char(&c, 'A');

    char char_value;

    if(get_char(&c, &char_value)){
        printf("Char: %c\n", char_value);
    }

    set_string(&c, "Hello, World!");

    const char *string_value;

    if(get_string(&c, &string_value)){
        printf("String: %s\n", string_value);
    }

    // Free any allocated memory
    container_clear(&c);

    Container records[6];

    for(int i = 0; i < 6; i++){
        container_init(&records[i]);
    }

    set_int(&records[0], 10);
    set_float(&records[1], 3.14);
    set_char(&records[2], 'A');
    set_string(&records[3], "Hello");
    set_int(&records[4], 0);
    set_string(&records[5], "Data");

    printf("\nMIXED TYPE CONTAINERS\n");

    for(int i = 0; i < 6; i++){
        printf("\nIndex: %d\n", i);
        printf("Type: %s\n", container_type_name(records[i].type));
        printf("Value: ");
        container_print(&records[i]);
    }

    printf("\nDEEP COPY DEMONSTRATION\n");

    Container copied_record;
    container_init(&copied_record);

    if(container_copy(&copied_record, &records[5])){
        printf("Copied Value: ");
        container_print(&copied_record);
    }

    set_string(&records[5], "Changed Data");

    printf("Original Value: ");
    container_print(&records[5]);

    printf("Copied Value: ");
    container_print(&copied_record);

    printf("\nTYPE TRANSITION TESTS\n");

    set_string(&c, "First String");
    printf("STRING: ");
    container_print(&c);

    set_int(&c, 100);
    printf("INT: ");
    container_print(&c);

    set_string(&c, "Second String");
    printf("STRING: ");
    container_print(&c);

    set_float(&c, 45.5);
    printf("FLOAT: ");
    container_print(&c);

    printf("\nEDGE CASE TESTS\n");

    printf("\nInteger Zero Test:\n");

    set_int(&c, 0);

    if(get_int(&c, &int_value)){
        printf("Value: %d\n", int_value);
    }

    printf("\nWrong Type Getter Test:\n");

    if(!get_string(&c, &string_value)){
        printf("Cannot get string from an integer container!\n");
    }

    printf("\nEmpty Container Test:\n");

    container_clear(&c);
    container_print(&c);

    printf("\nEmpty String Test:\n");

    if(set_string(&c, "")){
        printf("Empty string stored successfully!\n");
        container_print(&c);
    }

    printf("\nRepeated Clear Test:\n");

    container_clear(&c);
    container_clear(&c);

    printf("Container cleared safely!\n");

    printf("\nString Replacement Test:\n");

    set_string(&c, "Same String");

    const char *same_string = c.data.string_value;

    if(set_string(&c, same_string)){
        printf("String replaced successfully!\n");
        container_print(&c);
    }

    printf("\nSelf Copy Test:\n");

    if(container_copy(&c, &c)){
        printf("Self copy handled successfully!\n");
        container_print(&c);
    }

    container_clear(&c);

    for(int i = 0; i < 6; i++){
        container_clear(&records[i]);
    }

    container_clear(&copied_record);

    printf("\nAll containers cleared successfully!\n");

    return 0;
}
