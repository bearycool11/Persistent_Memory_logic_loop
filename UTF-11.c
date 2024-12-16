#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for Unicode symbols with descriptions
struct UnicodeSymbol {
    wchar_t symbol;
    char *description;
};

// Function to add a symbol to the database
int add_symbol(struct UnicodeSymbol **database, int *count, wchar_t symbol, const char *description) {
    // Allocate memory for the new symbol and its description
    struct UnicodeSymbol *new_symbol = (struct UnicodeSymbol *)malloc(sizeof(struct UnicodeSymbol));
    if (new_symbol == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 1;
    }

    new_symbol->symbol = symbol;
    new_symbol->description = strdup(description); // Duplicate the description string

    // Reallocate the database array to accommodate the new symbol
    struct UnicodeSymbol *temp = (struct UnicodeSymbol *)realloc(*database, (*count + 1) * sizeof(struct UnicodeSymbol));
    if (temp == NULL) {
        fprintf(stderr, "Error: Memory reallocation failed.\n");
        free(new_symbol->description);
        free(new_symbol);
        return 1;
    }
    *database = temp;

    (*database)[*count] = *new_symbol;
    (*count)++;
    return 0;
}

// Function to search for a symbol by its code point
struct UnicodeSymbol *find_symbol_by_code_point(const struct UnicodeSymbol *database, int count, wchar_t symbol) {
    for (int i = 0; i < count; i++) {
        if (database[i].symbol == symbol) {
            return &database[i];
        }
    }
    return NULL;
}

// Function to display the database
void display_database(const struct UnicodeSymbol *database, int count) {
    for (int i = 0; i < count; i++) {
        wprintf(L"%lc - %s\n", database[i].symbol, database[i].description);
    }
}

// Predefined categories of Unicode symbols
const wchar_t musicalSymbols[] = {
    0x1D15D, 0x266A, 0x1D160, 0x1D161, 0x1D11E, 0x1D122, 0x1D12C, 0x1D12B, 0x1D12A, 0x1D13B,
    0x1D13C, 0x1D13D, 0x1D13E, 0x1D13F, 0x1D106, 0x1D107, 0x1D101, 0x1D102, 0x1D10F, 0x1D110,
    0x1D111, 0x1D112, 0x1D113, 0x1D114, 0x1D107, 0x1D108
};

const wchar_t latexMathSymbols[] = {
    0x1D6FC, 0x211D, 0x1D6A3, 0x1D6A4, 0x1D6B8, 0x1D6B9, 0x1D70B, 0x2124, 0x2115, 0x211A,
    0x2102, 0x1D6E2, 0x1D6E3, 0x2202, 0x2211, 0x220F, 0x222B, 0x2248, 0x2264, 0x2265, 0x221E
};

const wchar_t setTheorySymbols[] = {
    0x2208, 0x2209, 0x220B, 0x222A, 0x2229, 0x2286, 0x2282, 0x2287, 0x2283, 0x2205,
    0x2118, 0x2227, 0x2228, 0x00AC, 0x21D2, 0x21D4
};

// Function to add predefined symbols to the database
void add_predefined_symbols(struct UnicodeSymbol **database, int *count, const wchar_t *symbols, int symbol_count, const char *category_name) {
    for (int i = 0; i < symbol_count; i++) {
        wchar_t symbol = symbols[i];
        char description[256];
        snprintf(description, sizeof(description), "%s Symbol #%d", category_name, i + 1);
        add_symbol(database, count, symbol, description);
    }
}

int main() {
    // Ensure UTF-8 encoding by setting the locale
    if (setlocale(LC_ALL, "en_US.UTF-8") == NULL) {
        fprintf(stderr, "Failed to set locale to en_US.UTF-8\n");
        return 1;
    }

    // Unicode database and counter
    struct UnicodeSymbol *database = NULL;
    int count = 0;

    // Add predefined categories to the database
    add_predefined_symbols(&database, &count, musicalSymbols, sizeof(musicalSymbols) / sizeof(musicalSymbols[0]), "Musical");
    add_predefined_symbols(&database, &count, latexMathSymbols, sizeof(latexMathSymbols) / sizeof(latexMathSymbols[0]), "LaTeX Math");
    add_predefined_symbols(&database, &count, setTheorySymbols, sizeof(setTheorySymbols) / sizeof(setTheorySymbols[0]), "Set Theory");

    // Display the database
    wprintf(L"Unicode Symbol Database:\n");
    display_database(database, count);

    // Search for a symbol
    wchar_t search_symbol = 0x1D15D; // Example: Quarter note
    struct UnicodeSymbol *found_symbol = find_symbol_by_code_point(database, count, search_symbol);
    if (found_symbol != NULL) {
        wprintf(L"Found symbol: %lc - %s\n", found_symbol->symbol, found_symbol->description);
    } else {
        wprintf(L"Symbol not found.\n");
    }

    // Free the memory allocated for the database
    for (int i = 0; i < count; i++) {
        free(database[i].description);
    }
    free(database);

    return 0;
}
