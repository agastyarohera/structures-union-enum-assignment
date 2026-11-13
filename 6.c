// Unions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

typedef union {
    int i;
    float f;
    char str[20];
} UnionData;

typedef struct {
    int i;
    float f;
    char str[20];
} StructData;

typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_TEXT
} ValueType;

typedef struct {
    UnionData value;
    ValueType type;
} TaggedData;


bool set_integer(TaggedData *record, int value) {

    record->value.i = value;
    record->type = VALUE_INT;

    return true;
}


bool set_float(TaggedData *record, float value) {

    if(!isfinite(value)) {
        return false;
    }

    record->value.f = value;
    record->type = VALUE_FLOAT;

    return true;
}


bool set_text(TaggedData *record, const char *value) {

    if(value == NULL) {
        return false;
    }

    if(strlen(value) >= sizeof(record->value.str)) {
        return false;
    }

    strcpy(record->value.str, value);
    record->type = VALUE_TEXT;

    return true;
}


void display_tagged_record(TaggedData *record, int index) {

    printf("Record %d: ", index);

    switch(record->type) {

        case VALUE_INT: {
            printf("Integer = %d\n", record->value.i);
            break;
        }

        case VALUE_FLOAT: {
            printf("Float = %.2f\n", record->value.f);
            break;
        }

        case VALUE_TEXT: {
            printf("Text = %s\n", record->value.str);
            break;
        }

        default: {
            printf("Invalid Value Type!\n");
            break;
        }
    }
}


void display_all_records(TaggedData *records, int size) {

    if(size == 0) {
        printf("No Records Available!\n");
        return;
    }

    for(int i = 0; i < size; i++) {
        display_tagged_record(&records[i], i + 1);
    }
}


void display_memory_information() {

    UnionData udata;
    StructData sdata;

    printf("\nMEMORY ADDRESSES\n");

    printf("\nMemory Addresses - Union:\n");

    printf("udata.i: %p\n", (void*)&udata.i);
    printf("udata.str: %p\n", (void*)&udata.str);
    printf("udata.f: %p\n", (void*)&udata.f);


    printf("\nMemory Addresses - Struct:\n");

    printf("sdata.i: %p\n", (void*)&sdata.i);
    printf("sdata.str: %p\n", (void*)&sdata.str);
    printf("sdata.f: %p\n", (void*)&sdata.f);


    printf("\nSTRUCTURE OFFSETS\n");

    printf("Offset of i: %zu bytes\n", offsetof(StructData, i));
    printf("Offset of f: %zu bytes\n", offsetof(StructData, f));
    printf("Offset of str: %zu bytes\n", offsetof(StructData, str));


    printf("\nSIZE AND ALIGNMENT\n");

    printf("Size of Union: %zu bytes\n", sizeof(UnionData));
    printf("Alignment of Union: %zu bytes\n", _Alignof(UnionData));

    printf("\nSize of Structure: %zu bytes\n", sizeof(StructData));
    printf("Alignment of Structure: %zu bytes\n", _Alignof(StructData));


    printf("\nMEMORY FOR 100 RECORDS\n");

    printf("100 Union Records: %zu bytes\n", 100 * sizeof(UnionData));
    printf("100 Structure Records: %zu bytes\n", 100 * sizeof(StructData));

}


int main() {

    UnionData udata;

    StructData sdata;

    // Structure values

    sdata.i = 10;
    strcpy(sdata.str, "Struct Programming");
    sdata.f = 220.5;


    // Union values - one at a time

    udata.i = 10;
    printf("udata.i: %d\n", udata.i);

    strcpy(udata.str, "Union Programming");
    printf("udata.str: %s\n", udata.str);

    udata.f = 220.5;
    printf("udata.f: %.2f\n\n", udata.f);


    // Now see the values after overwriting

    printf("After overwriting:\n");

    udata.i = 10;
    printf("udata.i: %d\n", udata.i);

    strcpy(udata.str, "Union Programming");
    printf("udata.str: %s\n", udata.str);

    udata.f = 220.5;
    printf("udata.f: %.2f\n\n", udata.f);


    display_memory_information();


    // Tagged Records

    printf("\nTAGGED RECORDS\n");

    TaggedData records[5];

    if(!set_integer(&records[0], 10) ||
       !set_float(&records[1], 220.5) ||
       !set_text(&records[2], "Union Programming") ||
       !set_integer(&records[3], 50) ||
       !set_text(&records[4], "Structure")) {

        printf("Error creating records!\n");
        return 1;
    }

    display_all_records(records, 5);


    printf("\nTAGGED RECORD SIZE\n");

    printf("Size of TaggedData: %zu bytes\n", sizeof(TaggedData));
    printf("Size of UnionData: %zu bytes\n", sizeof(UnionData));
    printf("Size of StructData: %zu bytes\n", sizeof(StructData));


    // Edge Case Tests

    printf("\nEDGE CASE TESTS\n");

    TaggedData test_record;


    printf("\nMaximum Length String Test:\n");

    if(set_text(&test_record, "1234567890123456789")) {
        printf("String stored successfully!\n");
        display_tagged_record(&test_record, 1);
    }
    else {
        printf("String could not be stored!\n");
    }


    printf("\nOverlength String Test:\n");

    if(set_text(&test_record, "12345678901234567890")) {
        printf("String stored successfully!\n");
    }
    else {
        printf("String exceeds maximum length of 19 characters!\n");
    }


    printf("\nSwitching Union Members Test:\n");

    set_integer(&test_record, 100);
    display_tagged_record(&test_record, 1);

    set_text(&test_record, "Hello");
    display_tagged_record(&test_record, 1);

    set_float(&test_record, 45.5);
    display_tagged_record(&test_record, 1);


    printf("\nInvalid Tag Test:\n");

    test_record.type = (ValueType)99;

    display_tagged_record(&test_record, 1);


    printf("\nInvalid Float Test:\n");

    if(!set_float(&test_record, INFINITY)) {
        printf("Invalid float value rejected!\n");
    }


    return 0;
}

/*

OBSERVATIONS:

Union: All members share the same memory location. Assigning a new value overwrites the previous value. Its size is sufficient to hold its largest member, with possible alignment padding.

Structure: Every member has separate memory. All members can hold their values simultaneously. Padding may be added by the compiler to satisfy alignment requirements.

Offsets: offsetof() gives the byte position of each structure member relative to the beginning of the structure.

Alignment and Padding: The compiler may insert padding bytes between structure members so that each member starts at a suitably aligned memory address. The final structure size may also include padding.

Memory Efficiency: A union saves memory when only one of several data types needs to be stored at a time.

Tagged Union: The ValueType variable identifies which union member currently contains a valid value. This prevents accessing inactive members.

String Validation: str[20] can store a maximum of 19 characters because one byte is required for the null terminator.

*/