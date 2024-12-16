#ifndef UTF_11_H
#define UTF_11_H

#include <wchar.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for Unicode symbols with descriptions
typedef struct {
    wchar_t symbol;
    char *description;
} UnicodeSymbol;

// Function prototypes for managing the Unicode database
int add_symbol(UnicodeSymbol **database, int *count, wchar_t symbol, const char *description);
UnicodeSymbol *find_symbol_by_code_point(const UnicodeSymbol *database, int count, wchar_t symbol);
void display_database(const UnicodeSymbol *database, int count);
void add_predefined_symbols(UnicodeSymbol **database, int *count, const wchar_t *symbols, int symbol_count, const char *category_name);

#endif // UTF_11_H
