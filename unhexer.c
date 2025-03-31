#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAX_TEXT_SIZE 10000

// Converts a single hex character to its integer value
int hex_char_to_int(char c) {
    c = toupper(c);
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return 10 + c - 'A';
    return -1; // Invalid hex character
}

// Converts two hex characters to a byte value
int hex_pair_to_byte(char a, char b) {
    int high = hex_char_to_int(a);
    int low = hex_char_to_int(b);
    if (high == -1 || low == -1) return -1;
    return (high << 4) | low;
}

// Prints characters, handling control codes and extended characters
void print_display_char(int val) {
    if ((val >= 0 && val <= 31) || val == 127) {
        // Convert control characters to ^X format
        printf("^%c", val + 64);
    } else if (val == 0x7F) {
        printf("<DEL>");
    } else if (val >= 32 && val <= 126) {
        // Print normal printable ASCII characters
        printf("%c", val);
    } else {
        // Show extended characters in hex format
        printf("[0x%02X]", val);
    }
}

int main() {
    setlocale(LC_ALL, "en_US.UTF-8");
    
    FILE *cipher = fopen("ciphered_info.txt", "r");
    if (!cipher) {
        printf("Error opening ciphertext file\n");
        return 1;
    }

    char hex_text[MAX_TEXT_SIZE];
    size_t len = 0;
    int ch;
    
    // Read the entire ciphertext file
    while ((ch = fgetc(cipher)) != EOF && len < MAX_TEXT_SIZE - 1) {
        if (isxdigit(ch)) { // Only keep hex characters
            hex_text[len++] = (char)ch;
        }
    }
    hex_text[len] = '\0';
    fclose(cipher);

    // Check for odd number of hex digits
    if (len % 2 != 0) {
        printf("Warning: Odd number of hex digits, last character '%c' ignored\n", hex_text[len-1]);
        len--; // Ignore the last character
    }

    printf("Hex Text:\n%s\n\n", hex_text);
    printf("Decoded Text:\n");

    // Process hex pairs
    for (size_t i = 0; i < len; i += 2) {
        int byte = hex_pair_to_byte(hex_text[i], hex_text[i+1]);
        if (byte == -1) {
            printf("\nInvalid hex pair: %c%c\n", hex_text[i], hex_text[i+1]);
            continue;
        }

        print_display_char(byte);
    }
    putchar('\n');

    return 0;
}