#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

typedef enum {
    CMD_ADD,
    CMD_DELETE,
    CMD_VIEW,
    CMD_UPDATE,
    CMD_COMPLETE,
    CMD_FIND,
    CMD_HELP,
    CMD_EXIT,
    CMD_UNKNOWN
} CommandType;

typedef enum {
    PENDING,
    DONE
} TaskStatus;

typedef struct {
    int id;
    char description[100];
    TaskStatus status;
} Task;


int read_line(char *input, int size) {
    if(fgets(input, size, stdin) == NULL) {
        return 0;
    }

    int length = strlen(input);

    if(length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
        return 1;
    }

    int ch = getchar();

    if(ch == '\n' || ch == EOF) {
        return 1;
    }

    while(ch != '\n' && ch != EOF) {
        ch = getchar();
    }

    return -1;
}


bool get_task_id(int *id) {
    char input[100];
    char *endptr;

    printf("Enter task ID: ");

    int result = read_line(input, sizeof(input));

    if(result == 0) {
        return false;
    }

    if(result == -1) {
        printf("Input is too long!\n");
        return true;
    }

    errno = 0;
    long value = strtol(input, &endptr, 10);

    while(isspace((unsigned char)*endptr)) {
        endptr++;
    }

    if(input == endptr || *endptr != '\0' || errno == ERANGE ||
       value <= 0 || value > INT_MAX) {
        printf("Invalid task ID! Enter a positive integer.\n");
        return true;
    }

    *id = (int)value;
    return true;
}


CommandType get_command_type(char *commandStr) {
    for(int i = 0; commandStr[i] != '\0'; i++) {
        commandStr[i] = toupper((unsigned char)commandStr[i]);
    }

    if(strcmp(commandStr, "ADD") == 0) return CMD_ADD;
    if(strcmp(commandStr, "DELETE") == 0) return CMD_DELETE;
    if(strcmp(commandStr, "VIEW") == 0) return CMD_VIEW;
    if(strcmp(commandStr, "UPDATE") == 0) return CMD_UPDATE;
    if(strcmp(commandStr, "COMPLETE") == 0) return CMD_COMPLETE;
    if(strcmp(commandStr, "FIND") == 0) return CMD_FIND;
    if(strcmp(commandStr, "HELP") == 0) return CMD_HELP;
    if(strcmp(commandStr, "EXIT") == 0) return CMD_EXIT;

    return CMD_UNKNOWN;
}


int find_task(Task *tasks, int count, int id) {
    for(int i = 0; i < count; i++) {
        if(tasks[i].id == id) {
            return i;
        }
    }

    return -1;
}


bool add_task(Task **tasks, int *count, int *capacity) {
    int id;
    char description[100];

    if(!get_task_id(&id)) {
        return false;
    }

    if(find_task(*tasks, *count, id) != -1) {
        printf("Task ID %d already exists!\n", id);
        return true;
    }

    printf("Enter task description: ");

    int result = read_line(description, sizeof(description));

    if(result == 0) {
        return false;
    }

    if(result == -1) {
        printf("Description is too long!\n");
        return true;
    }

    if(strlen(description) == 0) {
        printf("Description cannot be empty!\n");
        return true;
    }

    if(*count == *capacity) {
        int new_capacity = (*capacity == 0) ? 2 : (*capacity * 2);

        Task *temp = realloc(*tasks, new_capacity * sizeof(Task));

        if(temp == NULL) {
            printf("Memory allocation failed!\n");
            return true;
        }

        *tasks = temp;
        *capacity = new_capacity;
    }

    (*tasks)[*count].id = id;
    strcpy((*tasks)[*count].description, description);
    (*tasks)[*count].status = PENDING;

    (*count)++;

    printf("Task %d added successfully!\n", id);

    return true;
}


bool delete_task(Task *tasks, int *count) {
    int id;

    if(!get_task_id(&id)) {
        return false;
    }

    if(*count == 0) {
        printf("No tasks available!\n");
        return true;
    }

    int index = find_task(tasks, *count, id);

    if(index == -1) {
        printf("Task %d not found!\n", id);
        return true;
    }

    for(int i = index; i < *count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    (*count)--;

    printf("Task %d deleted successfully!\n", id);

    return true;
}


void view_tasks(Task *tasks, int count) {
    if(count == 0) {
        printf("No tasks available!\n");
        return;
    }

    printf("\nTASK LIST\n");

    for(int i = 0; i < count; i++) {
        printf("\nTask ID: %d\n", tasks[i].id);
        printf("Description: %s\n", tasks[i].description);

        if(tasks[i].status == PENDING) {
            printf("Status: PENDING\n");
        }
        else {
            printf("Status: DONE\n");
        }
    }

    printf("\n");
}


bool update_task(Task *tasks, int count) {
    int id;
    char description[100];

    if(!get_task_id(&id)) {
        return false;
    }

    int index = find_task(tasks, count, id);

    if(index == -1) {
        printf("Task %d not found!\n", id);
        return true;
    }

    printf("Enter new description: ");

    int result = read_line(description, sizeof(description));

    if(result == 0) {
        return false;
    }

    if(result == -1) {
        printf("Description is too long!\n");
        return true;
    }

    if(strlen(description) == 0) {
        printf("Description cannot be empty!\n");
        return true;
    }

    strcpy(tasks[index].description, description);

    printf("Task %d updated successfully!\n", id);

    return true;
}


bool complete_task(Task *tasks, int count) {
    int id;

    if(!get_task_id(&id)) {
        return false;
    }

    int index = find_task(tasks, count, id);

    if(index == -1) {
        printf("Task %d not found!\n", id);
        return true;
    }

    tasks[index].status = DONE;

    printf("Task %d marked as DONE!\n", id);

    return true;
}


bool find_task_by_id(Task *tasks, int count) {
    int id;

    if(!get_task_id(&id)) {
        return false;
    }

    int index = find_task(tasks, count, id);

    if(index == -1) {
        printf("Task %d not found!\n", id);
        return true;
    }

    printf("\nTask ID: %d\n", tasks[index].id);
    printf("Description: %s\n", tasks[index].description);

    if(tasks[index].status == PENDING) {
        printf("Status: PENDING\n");
    }
    else {
        printf("Status: DONE\n");
    }

    return true;
}


void display_help() {
    printf("\nAVAILABLE COMMANDS\n");
    printf("ADD       - Add a new task\n");
    printf("DELETE    - Delete an existing task\n");
    printf("VIEW      - View all tasks\n");
    printf("UPDATE    - Update a task description\n");
    printf("COMPLETE  - Mark a task as done\n");
    printf("FIND      - Find a task using its ID\n");
    printf("HELP      - Display available commands\n");
    printf("EXIT      - Exit the program\n\n");
}


int main() {
    Task *tasks = NULL;
    int count = 0;
    int capacity = 0;

    char input[100];
    CommandType cmd;

    while(1) {
        printf("\nEnter command (ADD, DELETE, VIEW, UPDATE, COMPLETE, FIND, HELP, EXIT): ");

        int result = read_line(input, sizeof(input));

        if(result == 0) {
            printf("\nInput ended. Exiting program.\n");
            break;
        }

        if(result == -1) {
            printf("Command is too long!\n");
            continue;
        }

        cmd = get_command_type(input);

        switch(cmd) {
            case CMD_ADD: {
                if(!add_task(&tasks, &count, &capacity)) {
                    printf("\nInput ended. Exiting program.\n");
                    free(tasks);
                    return 0;
                }
                break;
            }

            case CMD_DELETE: {
                if(!delete_task(tasks, &count)) {
                    printf("\nInput ended. Exiting program.\n");
                    free(tasks);
                    return 0;
                }
                break;
            }

            case CMD_VIEW: {
                view_tasks(tasks, count);
                break;
            }

            case CMD_UPDATE: {
                if(!update_task(tasks, count)) {
                    printf("\nInput ended. Exiting program.\n");
                    free(tasks);
                    return 0;
                }
                break;
            }

            case CMD_COMPLETE: {
                if(!complete_task(tasks, count)) {
                    printf("\nInput ended. Exiting program.\n");
                    free(tasks);
                    return 0;
                }
                break;
            }

            case CMD_FIND: {
                if(!find_task_by_id(tasks, count)) {
                    printf("\nInput ended. Exiting program.\n");
                    free(tasks);
                    return 0;
                }
                break;
            }

            case CMD_HELP: {
                display_help();
                break;
            }

            case CMD_EXIT: {
                printf("Exiting program.\n");
                free(tasks);
                return 0;
            }

            default: {
                printf("Unknown command!\n");
                break;
            }
        }
    }

    free(tasks);

    return 0;
}