// Structures - 2

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    int id;
    float width;
    float height;
    float area;
    float perimeter;
} Rectangle;


void calculate_area(Rectangle *r) {
    r->area = r->width * r->height;
}

void calculate_perimeter(Rectangle *r) {
    r->perimeter = 2 * (r->width + r->height);
}

Rectangle* create_rectangle_array(int size) {
    Rectangle *rectangles = (Rectangle*) malloc(size * sizeof(Rectangle));
    return rectangles;
}

void add_rectangle(Rectangle *rectangles, int i, int id, float width, float height) {
    rectangles[i].id = id;
    rectangles[i].width = width;
    rectangles[i].height = height;

    calculate_area(&rectangles[i]);
    calculate_perimeter(&rectangles[i]);
}

void display_rectangles(Rectangle *rectangles, int size) {

    if(size == 0) {
        printf("No Rectangles Available!\n");
        return;
    }

    for(int i = 0; i < size; i++) {

        printf("\nRectangle ID: %d\n", rectangles[i].id);
        printf("Width: %.2f\n", rectangles[i].width);
        printf("Height: %.2f\n", rectangles[i].height);
        printf("Area: %.2f\n", rectangles[i].area);
        printf("Perimeter: %.2f\n", rectangles[i].perimeter);

        if(fabs(rectangles[i].width - rectangles[i].height) <=
           0.000001 * fmax(1.0, fmax(rectangles[i].width, rectangles[i].height))) {
            printf("Type: Square\n");
        }
        else {
            printf("Type: Non-Square\n");
        }
    }
}

void free_rectangle_array(Rectangle *rectangles) {
    free(rectangles);
}

bool check_rectangle_id_exists(Rectangle *rectangles, int size, int id) {

    for(int i = 0; i < size; i++) {
        if(rectangles[i].id == id) {
            return true;
        }
    }

    return false;
}

void clear_input_buffer() {

    int c;

    while((c = getchar()) != '\n' && c != EOF) {
    }

}

int check_valid_dimensions(float width, float height) {

    if(!isfinite(width) || !isfinite(height)) {
        return 0;
    }

    if(width <= 0 || height <= 0) {
        return 0;
    }

    if(!isfinite(width * height) || !isfinite(2 * (width + height))) {
        return 0;
    }

    return 1;
}


int compare_rectangles(const void *a, const void *b) {

    const Rectangle *rectangle1 = (const Rectangle*) a;
    const Rectangle *rectangle2 = (const Rectangle*) b;

    if(rectangle1->area < rectangle2->area) {
        return -1;
    }

    if(rectangle1->area > rectangle2->area) {
        return 1;
    }

    if(rectangle1->id < rectangle2->id) {
        return -1;
    }

    if(rectangle1->id > rectangle2->id) {
        return 1;
    }

    return 0;
}


void display_sorted(Rectangle *rectangles, int size) {

    if(size == 0) {
        printf("No Rectangle Records Available!\n");
        return;
    }

    // Created a copy of the array and applied sort to it

    Rectangle *sorted_rectangles = malloc(size * sizeof(Rectangle));

    if(sorted_rectangles == NULL) {
        printf("Memory Allocation Failed!\n");
        return;
    }

    for(int i = 0; i < size; i++) {
        sorted_rectangles[i] = rectangles[i];
    }

    qsort(sorted_rectangles, size, sizeof(Rectangle), compare_rectangles);

    display_rectangles(sorted_rectangles, size);

    free_rectangle_array(sorted_rectangles);
}


void search_rectangle(Rectangle *rectangles, int size) {

    int id;

    printf("Enter Rectangle ID to Search: ");

    if(scanf("%d", &id) != 1) {
        printf("Invalid Input!\n");
        clear_input_buffer();
        return;
    }

    if(check_rectangle_id_exists(rectangles, size, id)) {

        for(int i = 0; i < size; i++) {

            if(rectangles[i].id == id) {

                printf("\nRectangle ID: %d\n", rectangles[i].id);
                printf("Width: %.2f\n", rectangles[i].width);
                printf("Height: %.2f\n", rectangles[i].height);
                printf("Area: %.2f\n", rectangles[i].area);
                printf("Perimeter: %.2f\n", rectangles[i].perimeter);

                break;
            }
        }
    }
    else {
        printf("Rectangle ID Does Not Exist!\n");
    }
}


void update_rectangle(Rectangle *rectangles, int size) {

    int id;

    printf("Enter Rectangle ID to Update: ");

    if(scanf("%d", &id) != 1) {
        printf("Invalid Input!\n");
        clear_input_buffer();
        return;
    }

    if(check_rectangle_id_exists(rectangles, size, id)) {

        float width, height;

        printf("Enter New Width: ");

        if(scanf("%f", &width) != 1) {
            printf("Invalid Input!\n");
            clear_input_buffer();
            return;
        }

        printf("Enter New Height: ");

        if(scanf("%f", &height) != 1) {
            printf("Invalid Input!\n");
            clear_input_buffer();
            return;
        }

        if(!check_valid_dimensions(width, height)) {
            printf("Invalid Dimensions!\n");
            return;
        }

        for(int i = 0; i < size; i++) {

            if(rectangles[i].id == id) {

                rectangles[i].width = width;
                rectangles[i].height = height;

                calculate_area(&rectangles[i]);
                calculate_perimeter(&rectangles[i]);

                printf("Rectangle Updated Successfully!\n");

                break;
            }
        }
    }
    else {
        printf("Rectangle ID Does Not Exist!\n");
    }
}


void delete_rectangle(Rectangle *rectangles, int *size) {

    int id;
    int index;

    printf("Enter Rectangle ID to Delete: ");

    if(scanf("%d", &id) != 1) {
        printf("Invalid Input!\n");
        clear_input_buffer();
        return;
    }

    if(check_rectangle_id_exists(rectangles, *size, id)) {

        for(int i = 0; i < *size; i++) {

            if(rectangles[i].id == id) {
                index = i;
                break;
            }
        }

        for(int i = index; i < *size - 1; i++) {
            rectangles[i] = rectangles[i + 1];
        }

        (*size)--;

        printf("Rectangle Deleted Successfully!\n");
    }
    else {
        printf("Rectangle ID Does Not Exist!\n");
    }
}


void display_statistics(Rectangle *rectangles, int size) {

    if(size == 0) {
        printf("No Rectangle Records Available!\n");
        return;
    }

    double total_area = 0;
    int largest_index = 0;
    int smallest_index = 0;

    for(int i = 0; i < size; i++) {

        total_area += rectangles[i].area;

        if(rectangles[i].area > rectangles[largest_index].area) {
            largest_index = i;
        }

        if(rectangles[i].area < rectangles[smallest_index].area) {
            smallest_index = i;
        }
    }

    double average_area = total_area / size;

    printf("\n------RECTANGLE STATISTICS------\n");

    printf("\nRectangle with Largest Area:\n");
    printf("ID: %d, Area: %.2f\n",
           rectangles[largest_index].id,
           rectangles[largest_index].area);

    printf("\nRectangle with Smallest Area:\n");
    printf("ID: %d, Area: %.2f\n",
           rectangles[smallest_index].id,
           rectangles[smallest_index].area);

    printf("\nTotal Area: %.2f\n", total_area);
    printf("Average Area: %.2f\n", average_area);
}


void filter_rectangles(Rectangle *rectangles, int size) {

    float threshold;
    int found = 0;

    printf("Enter Area Threshold: ");

    if(scanf("%f", &threshold) != 1) {
        printf("Invalid Input!\n");
        clear_input_buffer();
        return;
    }

    if(!isfinite(threshold) || threshold < 0) {
        printf("Invalid Threshold!\n");
        return;
    }

    for(int i = 0; i < size; i++) {

        if(rectangles[i].area > threshold) {

            printf("\nRectangle ID: %d\n", rectangles[i].id);
            printf("Width: %.2f\n", rectangles[i].width);
            printf("Height: %.2f\n", rectangles[i].height);
            printf("Area: %.2f\n", rectangles[i].area);

            found = 1;
        }
    }

    if(found == 0) {
        printf("No Rectangles Found Above This Area!\n");
    }
}


int main() {

    int allocated_capacity;

    printf("Enter Initial Capacity of Rectangles: ");

    if(scanf("%d", &allocated_capacity) != 1) {
        printf("Invalid Input!\n");
        return 1;
    }

    if(allocated_capacity < 1 || allocated_capacity > 1000) {
        printf("Invalid Input! Capacity should be between 1 and 1000.\n");
        return 1;
    }

    Rectangle *rectangle_list = create_rectangle_array(allocated_capacity);

    if(rectangle_list == NULL) {
        printf("Memory Allocation Failed!\n");
        return 2;
    }

    int query;
    int record_count = 0;

    do {

        printf("\nWhat do you wish to do?\n");
        printf("1. Add A Rectangle\n");
        printf("2. Display All Rectangles\n");
        printf("3. Search By ID\n");
        printf("4. Update A Rectangle\n");
        printf("5. Delete A Rectangle\n");
        printf("6. Display Rectangle Statistics\n");
        printf("7. Display Sorted Rectangles\n");
        printf("8. Filter Rectangles By Area\n");
        printf("9. Exit\n");

        if(scanf("%d", &query) != 1) {
            printf("Invalid Choice!\n");
            clear_input_buffer();
            continue;
        }

        switch(query) {

        case 1: {

            if(record_count == allocated_capacity) {

                if(allocated_capacity == 1000) {
                    printf("Maximum Rectangle Limit Reached!\n");
                    continue;
                }

                int new_capacity = allocated_capacity * 2;

                if(new_capacity > 1000) {
                    new_capacity = 1000;
                }

                Rectangle *temp = realloc(rectangle_list, new_capacity * sizeof(Rectangle));

                if(temp == NULL) {
                    printf("Memory Allocation Failed!\n");
                    continue;
                }

                rectangle_list = temp;
                allocated_capacity = new_capacity;
            }

            int id;
            float width, height;

            printf("Enter Rectangle ID: ");

            if(scanf("%d", &id) != 1) {
                printf("Invalid Input!\n");
                clear_input_buffer();
                continue;
            }

            if(id <= 0) {
                printf("Rectangle ID Should Be POSITIVE!\n");
                continue;
            }

            if(check_rectangle_id_exists(rectangle_list, record_count, id)) {
                printf("Rectangle With ID %d Already Exists!\n", id);
                continue;
            }

            printf("Enter Width: ");

            if(scanf("%f", &width) != 1) {
                printf("Invalid Input!\n");
                clear_input_buffer();
                continue;
            }

            printf("Enter Height: ");

            if(scanf("%f", &height) != 1) {
                printf("Invalid Input!\n");
                clear_input_buffer();
                continue;
            }

            if(!check_valid_dimensions(width, height)) {
                printf("Width and Height Must Be Positive and Finite!\n");
                continue;
            }

            add_rectangle(rectangle_list, record_count, id, width, height);

            printf("Rectangle Added Successfully!\n");

            record_count++;

            break;
        }


        case 2: {

            display_rectangles(rectangle_list, record_count);

            break;
        }


        case 3: {

            search_rectangle(rectangle_list, record_count);

            break;
        }


        case 4: {

            update_rectangle(rectangle_list, record_count);

            break;
        }


        case 5: {

            delete_rectangle(rectangle_list, &record_count);

            break;
        }


        case 6: {

            display_statistics(rectangle_list, record_count);

            break;
        }


        case 7: {

            display_sorted(rectangle_list, record_count);

            break;
        }


        case 8: {

            filter_rectangles(rectangle_list, record_count);

            break;
        }


        case 9: {

            break;
        }


        default:

            printf("Invalid Choice!\n");
            break;
        }

    } while(query != 9);


    free_rectangle_array(rectangle_list);

    return 0;
}