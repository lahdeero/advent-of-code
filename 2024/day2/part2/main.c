#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct arrStruct {
    int **array;
    int rows;
    int *columns;
};

enum Direction {
    UNKNOWN,
    DESCENDING,
    ASCENDING
};

struct arrStruct fileTo2dArray(char fileName[]) {
    FILE *file;
    char buffer[256];
    int **array = NULL;
    int rows = 0;
    int *columns = NULL;

    file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        int *row = malloc(256 * sizeof(int));
        if (row == NULL) {
            perror("Memory allocation error");
        }

        int numColumns = 0;
        char *token = strtok(buffer, " \n");
        while (token != NULL) {
            row[numColumns] = atoi(token);  // Add value to row array
            numColumns++;
            token = strtok(NULL, " \n");
        }
        columns = realloc(columns, (rows + 1) * sizeof(int));
        columns[rows] = numColumns;

        array = realloc(array, (rows + 1) * sizeof(int *));
        if (array == NULL) {
            perror("Memory allocation error");
        }
        array[rows] = row;
        rows++;
    }
    fclose(file);

    struct arrStruct arr;
    arr.array = array;
    arr.rows = rows;
    arr.columns = columns;
    return arr;
}

bool isSafe(int *arr, int length, int skip) {
    bool safe = true;
    int previous = 0;
    enum Direction direction = UNKNOWN;
    for (int i = 0; i < length; i++) {
        if (i == skip) {
            continue;
        }

        int value = arr[i];
        if (i == 0 || (skip == 0 && i == 1)) {
            previous = value;
            continue;
        }

        if (value > previous) {
            if (direction == DESCENDING) {
                safe = false;
                break;
            }
            direction = ASCENDING;
        } else if (value < previous) {
            if (direction == ASCENDING) {
                safe = false;
                break;
            }
            direction = DESCENDING;
        }

        int diff = abs(value - previous);
        if (diff < 1 || diff > 3) {
            safe = false;
            break;
        }
        previous = value;
    }
    return safe;
}

int main() {
    struct arrStruct st = fileTo2dArray("input.txt");

    int total = 0;
    for (int i = 0; i < st.rows; i++) {
        for (int j = 0; j < st.columns[i]; j++) {
            if (isSafe(st.array[i], st.columns[i], j)) {
                total++;
                break;
            }
        }
    }

    printf("%d\n", total);
    return EXIT_SUCCESS;
}
