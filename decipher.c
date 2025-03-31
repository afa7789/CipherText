#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAIRS 256  // Maximum number of character pairs
#define MAX_STRING_SIZE 10000  // Maximum size for the big string

// Reads a file and returns a file pointer
FILE *read_file(char* filename) {
    FILE *fptr;
    if ((fptr = fopen(filename, "r")) == NULL) {
        printf("Error! opening file: %s\n", filename);
        exit(1);
    }
    return fptr;
}

int load_cipher_table(char pairs[MAX_PAIRS][2], FILE *file) {
    int count = 0;
    char line[10];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%c,%c", &pairs[count][0], &pairs[count][1]) == 2) {
            count++;
        }
    }
    return count;
}

void load_ciphered_text(char *buffer, FILE *file) {
    size_t len = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        buffer[len++] = (char)ch;
    }
    buffer[len] = '\0';
}

int main() {
    // Open files
    FILE *ciphertable = read_file("ciphertable.csv");
    FILE *cipher = read_file("ciphered_info.txt");

    // Load cipher table
    char pairs[MAX_PAIRS][2];
    int pair_count = load_cipher_table(pairs, ciphertable);
    fclose(ciphertable);

    // Load ciphered text
    char *ciphered_text = (char *)malloc(MAX_STRING_SIZE);
    if (!ciphered_text) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    load_ciphered_text(ciphered_text, cipher);
    fclose(cipher);

    // Print loaded data (for verification)
    printf("Cipher Table:\n");
    for (int i = 0; i < pair_count; i++) {
        printf("%c -> %c\n", pairs[i][0], pairs[i][1]);
    }

    printf("\nCiphered Text:\n%s\n", ciphered_text);

    // Swap letters in ciphered text using the cipher table
    for (int i = 0; ciphered_text[i] != '\0'; i++) {
        for (int j = 0; j < pair_count; j++) {
            if (ciphered_text[i] == pairs[j][0]) {
                ciphered_text[i] = pairs[j][1];
                break;
            }
        }
    }

    printf("\nDeciphered Text:\n%s\n", ciphered_text);

    // Clean up
    free(ciphered_text);
    return 0;
}

