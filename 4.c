// Structures - 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    int roll_number;
    char name[50];
    int age;
    float gpa;
} Student;

Student* create_student_list(int n) {
    Student *studs = (Student*) malloc(n * sizeof(Student));
    return studs;
}

void add_student(Student *studs, int i,int roll_number, char *name, int age, float gpa) {
    strcpy(studs[i].name, name);
    studs[i].roll_number = roll_number;
    studs[i].age = age;
    studs[i].gpa = gpa;
}

void display_students(Student *students, int size) {

    if(size == 0){
        printf("No Record to Display!\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        printf("Roll Number: %d, Name: %s, Age: %d, GPA: %.2f\n", students[i].roll_number, students[i].name, students[i].age, students[i].gpa);
    }

}

void free_student_list(Student *students) {
    free(students);
}

bool check_roll_number_exists(Student *students,int size, int roll_number){
    for(int i = 0; i < size; i++){
        if(students[i].roll_number == roll_number){
            return true;
        }
    }
    return false;
}

int read_student_name(char name[50]){
    printf("Enter Name: ");

    if (fgets(name, 50, stdin) == NULL) {
        return 0;
    }

    char *newline = strchr(name, '\n');

    if (newline != NULL) {
        *newline = '\0';
    }
    else {
        int c = getchar();

        if (c != '\n' && c != EOF) {

            // Discard remaining characters
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            printf("Name cannot exceed 49 characters!\n");
            return 0;
        }
    }

    if (strlen(name) == 0) {
        printf("Name cannot be empty!\n");
        return 0;
    }

    return 1;
}

int compare_students(const void *a, const void *b){
    const Student *student1 = (const Student *)a;
    const Student *student2 = (const Student *)b;

    if (student1->gpa < student2->gpa) {
        return 1;
    }

    if (student1->gpa > student2->gpa) {
        return -1;
    }

    if (student1->roll_number < student2->roll_number) {
        return -1;
    }

    if (student1->roll_number > student2->roll_number) {
        return 1;
    }

    return 0;
}

void display_sorted(Student *students, int size){
    if (size == 0) {
        printf("No student records available!\n");
        return;
    }

    // Created a copy of the array and applied sort to it

    Student *sorted_students = malloc(size * sizeof(Student));

    if (sorted_students == NULL) {
        printf("Memory Allocation Failed!\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        sorted_students[i] = students[i];
    }

    qsort(sorted_students, size, sizeof(Student), compare_students);

    display_students(sorted_students, size);

    free_student_list(sorted_students);

}

int main() {
    int allocated_capacity;

    printf("Enter initial capacity of students: ");
    if(scanf("%d", &allocated_capacity) != 1){
        printf("Invalid Input!\n");
        return 1;
    }

    if(allocated_capacity < 1){
        printf("Invalid Input\n");
        return 1;
    }

    Student *student_list = create_student_list(allocated_capacity);

    if(student_list == NULL){
        printf("Memory Allocation Failed!\n");
        return 2;
    }

    int query;
    int record_count = 0;

    do{
        printf("\nWhat do you wish to do?\n");
        printf("1. Add A Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search By Roll Number\n");
        printf("4. Update A Record\n");
        printf("5. Delete A Record\n");
        printf("6. Display GPA Summary\n");
        printf("7. Display Sorted Records\n");
        printf("8. Exit\n");

        if(scanf("%d", &query) != 1 ){
            printf("Invalid Choice!\n");
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            continue;
        }
        

        switch (query)
        {
        case 1:{

            if(record_count == allocated_capacity){
                Student *temp = realloc(student_list, (allocated_capacity*2) * sizeof(Student));

                if (temp == NULL){
                    printf("Memory Allocation Failed!\n");
                    continue;
                }

                student_list = temp;
                allocated_capacity*=2;

            }

            int roll_number, age;
            float gpa;
            char name[50];

            printf("Enter Roll Number: ");
            if(scanf("%d", &roll_number) != 1){
                printf("Invalid Input!\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                continue;
            }

            if(roll_number <=0){
                printf("Roll Number should be POSITIVE!\n");
                continue;
            }

            if(check_roll_number_exists(student_list, record_count, roll_number)){
                printf("Student with roll number %d already exists!\n", roll_number);
                continue;
            }

            int c;

            while ((c = getchar()) != '\n' && c != EOF);

            if (!read_student_name(name)) {
                continue;
            }

            printf("Enter Age: ");
            if(scanf("%d", &age) != 1){
                printf("Invalid Input!\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                continue;
            }
            
            if(age < 16 || age > 100){
                printf("Age should be 16-100\n");
                continue;
            }

            printf("Enter GPA: ");
            if(scanf("%f", &gpa) != 1){
                printf("Invalid Input!\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                continue;
            }

            if(!isfinite(gpa) || gpa < 0.0f || gpa > 4.0f){
                printf("GPA should be 0.00 - 4.00\n");
                continue;
            }

            add_student(student_list, record_count, roll_number, name, age, gpa);
            
            printf("Student Added Successfully!\n");
            record_count++;

            break;
        }

        case 2:{

            display_students(student_list, record_count);
            
            break;
        }

        case 3:{

            int roll_number;

            printf("Enter roll number to search for: ");
            if(scanf("%d", &roll_number) != 1){
                printf("Invalid Input!\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                continue;
            }

            if(check_roll_number_exists(student_list, record_count, roll_number)){
                for(int i = 0; i < record_count; i++){
                    if(student_list[i].roll_number == roll_number){
                        printf("Roll Number: %d, Name: %s, Age: %d, GPA: %.2f\n", student_list[i].roll_number, student_list[i].name, student_list[i].age, student_list[i].gpa);
                        break;
                    }
                }
            }
            else{
                printf("Roll Number Does Not Exist!\n");
                continue;
            }

            break;
        }

        case 4:{

            int roll_number;

            printf("Enter roll number to update record for: ");
            if(scanf("%d", &roll_number) != 1){
                printf("Invalid Input!\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                continue;
            }

            if(check_roll_number_exists(student_list, record_count, roll_number)){
                int age;
                float gpa;
                char name[50];

                int c;

                while ((c = getchar()) != '\n' && c != EOF);

                if (!read_student_name(name)){
                    continue;
                }

                printf("Enter New Age: ");
                if(scanf("%d", &age) != 1){
                    printf("Invalid Input!\n");

                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);

                    continue;
                }
                
                if(age < 16 || age > 100){
                    printf("Age should be 16-100\n");
                    continue;
                }

                printf("Enter New GPA: ");
                if(scanf("%f", &gpa) != 1){
                    printf("Invalid Input!\n");

                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);

                    continue;
                }

                if(!isfinite(gpa) || gpa < 0.0f || gpa > 4.0f){
                    printf("GPA should be 0.00 - 4.00\n");
                    continue;
                }
                for(int i = 0; i < record_count; i++){
                    if(student_list[i].roll_number == roll_number){
                        strcpy(student_list[i].name, name);
                        student_list[i].age = age;
                        student_list[i].gpa = gpa;

                        printf("Record Updated Successfully!\n");
                        break;
                    }
                }
            }
            else{
                printf("Roll Number Does Not Exist!\n");
                continue;
            }

            break;
        }

        case 5:{

            int roll_number;
            int index;

            printf("Enter Roll Number To Delete Record: ");
            if(scanf("%d", &roll_number) != 1){
                printf("Invalid Input!\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                continue;
            }

            if(check_roll_number_exists(student_list, record_count, roll_number)){
                for(int i = 0; i < record_count; i++){
                    if(student_list[i].roll_number == roll_number){
                        index = i;
                        break;
                    }
                }
                for(int i = index; i < record_count - 1; i++){
                    student_list[i] = student_list[i+1];
                }
                printf("Record Deleted Successfully!\n");
                record_count--;
            }
            else{
                printf("Roll Number Does Not Exist!\n");
                continue;
            }

            break;
        }

        case 6:{

            if(record_count == 0){
                printf("No student records available!\n");
                continue;
            }

            float total_gpa = 0;
            int highest_index = 0;
            int lowest_index = 0;

            for(int i = 0; i < record_count; i++){

                total_gpa += student_list[i].gpa;

                if(student_list[i].gpa > student_list[highest_index].gpa){
                    highest_index = i;
                }

                if(student_list[i].gpa < student_list[lowest_index].gpa){
                    lowest_index = i;
                }
            }

            float average_gpa = total_gpa / record_count;

            printf("\n------GPA SUMMARY------\n");

            printf("\nStudent with Highest GPA:\n");
            printf("Roll Number: %d, Name: %s, Age: %d, GPA: %.2f\n",
                student_list[highest_index].roll_number,
                student_list[highest_index].name,
                student_list[highest_index].age,
                student_list[highest_index].gpa);

            printf("\nStudent with Lowest GPA:\n");
            printf("Roll Number: %d, Name: %s, Age: %d, GPA: %.2f\n",
                student_list[lowest_index].roll_number,
                student_list[lowest_index].name,
                student_list[lowest_index].age,
                student_list[lowest_index].gpa);

            printf("\nAverage GPA: %.2f\n", average_gpa);

            break;
        }

        case 7:{

            display_sorted(student_list, record_count);

            break;
        }

        case 8:{
            break;
        }
        
        default:
            printf("Invalid Choice!\n");
            break;
        }

    }while(query != 8);

    free_student_list(student_list);

    return 0;
}
